using Newtonsoft.Json.Linq;
using Newtonsoft.Json.Schema;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Advanced;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using System.Runtime.InteropServices;
using System.Text.Json;
using System.Text.Json.Nodes;

namespace UnitTests
{
    internal class Helpers
    {
        public List<string> PlayVideo(int handle, string type, bool stationary)
        {
            var scriptDir = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            var steps = 20;

            // Load the object image
            var objectImgPath = Path.Combine(scriptDir, "images", $"{type}.jpg");
            using var objectImg = Image.Load<Rgb24>(objectImgPath);

            var objWidth = objectImg.Width;
            var objHeight = objectImg.Height;

            var stepSize = (320 - objWidth) / steps;

            if (stationary)
            {
                stepSize = 0;
            }

            var timestamp = 0;
            var metadataList = new List<string>();

            for (int frameCount = 0; frameCount < steps; frameCount++)
            {
                using var canvas = new Image<Rgb24>(320, 320);

                var posX = stepSize * frameCount;
                var posY = 0;  // keep it at the top of the canvas

                canvas.Mutate(ctx => ctx.DrawImage(objectImg, new Point(posX, posY), 1f));

                timestamp += 100;

                int resp = 0;
                while (resp == 0)
                {
                    var memory = canvas.GetPixelMemoryGroup().Single();
                    var data = MemoryMarshal.AsBytes(memory.Span).ToArray();

                    resp = Cvedia.Securt.PushFrame(handle, data, canvas.Width, canvas.Height, (ulong)timestamp);
                }
            }

            for (int frameCount = 0; frameCount < 20; frameCount++)
            {
                using var canvas = new Image<Rgb24>(320, 320);
                timestamp += 10000;

                int resp = 0;
                while (resp == 0)
                {
                    var memory = canvas.GetPixelMemoryGroup().Single();
                    var data = MemoryMarshal.AsBytes(memory.Span).ToArray();

                    resp = Cvedia.Securt.PushFrame(handle, data, canvas.Width, canvas.Height, (ulong)timestamp);
                }
            }

            while (true)
            {
                string json;

                var ret = Cvedia.Securt.ConsumeEvents(handle, out json);

                if (ret == 0)
                {
                    break;
                }

                metadataList.Add(json);
            }

            return metadataList;
        }

        public bool CheckJsonTypes(List<string> jsonList, List<string> types)
        {
            List<string> jsonTypes = new List<string>();

            try
            {
                foreach (string json in jsonList)
                {
                    // Parse the JSON string into a JArray
                    JArray jsonArray = JArray.Parse(json);

                    // Iterate through each JObject in the JArray
                    foreach (JObject jsonObj in jsonArray)
                    {
                        // Extract the type of the current object
                        jsonTypes.Add(jsonObj["type"]?.ToString());
                    }
                }
            }
            catch (JsonException)
            {
                // Handle JSON parsing errors (optional)
                // For simplicity, returning false on parsing error
                return false;
            }

            return types.All(typeValue => jsonTypes.Contains(typeValue));
        }

        public bool CheckTrackHasEvents(List<string> jsonList)
        {
            foreach (var jsonEntry in jsonList)
            {
                JArray jsonArray = JArray.Parse(jsonEntry);
                foreach (JObject jsonObj in jsonArray)
                {
                    if (jsonObj["type"]?.ToString() != "track")
                    {
                        continue;
                    }

                    var jsonStr = jsonObj["object"]?.ToString();
                    try
                    {
                        var data = JObject.Parse(jsonStr);
                        var events = data["events"] as JArray;
                        if (events != null && events.Count >= 1)
                        {
                            return true;
                        }
                    }
                    catch (JsonException)
                    {
                        // JSON is not valid, continue to the next one
                        continue;
                    }
                }
            }
                        
            return false;
        }

        public void AssertNoEvents(List<string> metadataList, List<string> eventTypes)
        {
            foreach (var eventType in eventTypes)
            {
                if (CheckJsonTypes(metadataList, new List<string> { eventType }))
                {
                    throw new InvalidOperationException($"Found unexpected event type: {eventType}");
                }
            }
        }

        public void AssertEvents(List<string> metadataList, List<string> eventTypes, string schemaBaseUrl = "https://bin.cvedia.com/schema")
        {
            foreach (var eventType in eventTypes)
            {
                if (!CheckJsonTypes(metadataList, new List<string> { eventType }))
                {
                    throw new InvalidOperationException($"Event type {eventType} not found in metadata");
                }
                if (!ValidateJsonObjects(metadataList, eventType, $"{schemaBaseUrl}/{eventType}-v1.json"))
                {
                    throw new InvalidOperationException($"Failed to validate JSON objects for event type {eventType}");
                }
            }
        }

        private bool ValidateJsonObjects(List<string> jsonStrings, string idToCheck, string schemaUrl)
        {
            JSchema schema;
            using (var httpClient = new HttpClient())
            {
                var schemaJson = httpClient.GetStringAsync(schemaUrl).Result;
                schema = JSchema.Parse(schemaJson);
            }

            foreach (var jsonString in jsonStrings)
            {
                JArray jsonArray = JArray.Parse(jsonString);
                foreach (JObject jsonObj in jsonArray)
                {
                    if (jsonObj["type"]?.ToString() == idToCheck)
                    {
                        var jsonStr = jsonObj["object"]?.ToString();
                        try
                        {
                            var data = JObject.Parse(jsonStr);
                            if (!data.IsValid(schema, out IList<string> validationErrors))
                            {
                                Console.WriteLine($"Event with $id {idToCheck} is not valid. Error: {string.Join(", ", validationErrors)}");
                                return false;
                            }
                        }
                        catch (JsonException)
                        {
                            // JSON is not valid, continue to the next one
                            continue;
                        }
                    }                    
                }
            }

            return true;
        }
    }
}
