using Cvedia;
using CommandLine;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;

namespace RtspUrlFetcher
{
    public class Options
    {
        [Option('i', "input", Required = true, HelpText = "RTSP streams to connect to. RTSP passwords should use URI escape chars instead of special chars.", Separator = ' ')]
        public IEnumerable<string> InputUrls { get; set; } = new List<string>();

        [Option('r', "rt-location", Default = ".", HelpText = "Location of the CVEDIA-RT installation.")]
        public string RTLocation { get; set; } = "";

        [Option('s', "solutions-dir", Default = ".", HelpText = "Location of the CVEDIA-RT solutions.")]
        public string SolutionsDir { get; set; } = "";

        [Option('o', "output", Default = "127.0.0.1:8554", HelpText = "Host and port of the RTSP output server.")]
        public string OutputServer { get; set; } = "";

        [Option('m', "metadata", HelpText = "Write metadata to disk on the given location.")]
        public string? Metadata { get; set; }
    }

    class RtspDemo
    {
        private static List<string> urls = new List<string>();
        private static string rtHomeDir = ".";
        private static string solutionsDir = "";
        private static string rtspHost = "127.0.0.1:8554";
        private static string? metadataDir = null;

        // map of uri to instance handle
        private static Dictionary<string, int> instances = new();

        static void Main(string[] args)
        {
            bool parseError = false;

            Parser.Default.ParseArguments<Options>(args)
                   .WithParsed(opts =>
                   {
                       urls.AddRange(opts.InputUrls);
                       rtHomeDir = opts.RTLocation;
                       rtspHost = opts.OutputServer;
                       metadataDir = opts.Metadata;
                       solutionsDir = opts.SolutionsDir;
                   })
                   .WithNotParsed(errs =>
                   {
                       Console.WriteLine("Error parsing arguments.");
                       parseError = true;
                   });

            if (parseError) return;

            if (metadataDir != null)
            {
                if (!Directory.Exists(metadataDir))
                {
                    Directory.CreateDirectory(metadataDir);
                }
            }

            // add CVEDIA-RT to the PATH because of DLL dependencies
            Environment.SetEnvironmentVariable("PATH", Environment.GetEnvironmentVariable("PATH") + ";" + rtHomeDir);

            // set the CVEDIA-RT home directory
            Cvedia.System.SetRtHomeDir(rtHomeDir);

            // initialize RT
            int ret = Cvedia.Rt.Initialize();

            // discover all available solutions in the CVEDIA-RT home directory
            Cvedia.Solutions.Discover(solutionsDir, 1);

            bool errorOccured = false;
            foreach (string url in urls) {
                if( !StartInstance(url)) {
                    errorOccured = true;
                    break;
                }
            }

            if (!errorOccured) {
                Console.WriteLine("\nPress 'q' to exit.");

                while (!Console.KeyAvailable || Console.ReadKey(intercept: true).KeyChar != 'q')
                {
                    if (metadataDir != null) 
                        urls.ForEach(HandleMetadata);
                    System.Threading.Thread.Sleep(100);
                }

                urls.ForEach(StopInstance);
            }

            Cvedia.Rt.Shutdown();
        }

        private static bool StartInstance(string uri)
        {
            int index = urls.IndexOf(uri) + 1;

            // create a new context
            int handle = Cvedia.Core.CreateContext("", "");
            instances[uri] = handle;

            // create a new instance with a given ID and name
            Cvedia.Securt.CreateSecurtInstance(handle, $"instance_{index}");
            
            Cvedia.Core.SetAutoRestart(handle, 0);
            Cvedia.Core.SetAutoStart(handle, 1);
            
            Cvedia.Core.UpdateInstanceOptions(handle, $"instance_{index}", $"instance_{index}",
                0, 1, 2, 2, 1, 5, 1, 0, 0, 0, 1, 0);

            // connect to the input RTSP stream
            int ret = Cvedia.Core.SetInputToRtsp(handle, uri);

            if (ret == 0) {
                return false;
            }

            string rtspMapping = $"rtsp://{rtspHost}/{index}";
            // enable the RTSP output stream
            if (Cvedia.Core.EnableRtspOutput(handle, rtspMapping) > 0)
            {
                Console.WriteLine($"Mapped {uri} to {rtspMapping}");
            }

            float[] areaPts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            // create a full frame area that detects Person, Vehicle and Animal and triggers Enter and Exit events            
            Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", areaPts, areaPts.Length, new int[] { 1, 2, 3, 4 }, 4, 0, 3, new double[] {1.0, 1.0, 1.0, 1.0});

            return true;
        }

        private static void HandleMetadata(string uri)
        {
            int handle = instances[uri];

            // convert all special characters to underscores
            uri = Regex.Replace(uri, "[^a-zA-Z0-9_]+", "_");

            // loop until metadata is null
            while (true)
            {
                // consume metadata from the instance. 0 is returned if no metadata is available
                string json;

                var ret = Cvedia.Core.ConsumeEvents(handle, out json);
                if (ret != 0)
                {
                    File.AppendAllText($"{metadataDir}/{uri}.json", json + "\n");                    
                }
                else 
                {
                    break;
                }
            }
        }
        private static void StopInstance(string uri)
        {
            int handle = instances[uri];

            // stop and destroy the instance
            Cvedia.Core.Stop(handle);
            Cvedia.Securt.DestroyInstance(handle);
        }
    }
}