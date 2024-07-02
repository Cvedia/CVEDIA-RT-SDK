using Cvedia;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json.Schema;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Advanced;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;

namespace UnitTests
{

    [TestClass]
    public class SecuRT_Test
    {
        public int handle;
        private readonly Helpers helpers = new Helpers();

        [TestInitialize]
        public void TestSetup()
        {
            string dllDirectory = @"C:\CVEDIA\redist";
            string solutionsDir = @"C:\CVEDIA\redist\solutions";
            
            var tmpVar = Environment.GetEnvironmentVariable("CVEDIART_HOME");
            if (tmpVar != null) {
                dllDirectory = tmpVar;
            }
            tmpVar = Environment.GetEnvironmentVariable("SOLUTIONS_DIR");
            if (tmpVar != null) {
                solutionsDir = tmpVar;
            }
            
            var envSep = ";";
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux)) {
                envSep = ":";
            }
            Environment.SetEnvironmentVariable("PATH", Environment.GetEnvironmentVariable("PATH") + envSep + dllDirectory);

            Cvedia.System.SetRtHomeDir(dllDirectory);
            int ret = Cvedia.Rt.Initialize();
            if (ret < 0)
            {
                Assert.Fail("Failed to initialize CVEDIA-RT");
            }
            Cvedia.Solutions.Discover(solutionsDir, 1);
                                                  
            handle = Cvedia.Securt.CreateContext("3d26f5d0-f72b-8029-54db-e849514cd3a4", "");
            Cvedia.Securt.CreateInstance(handle, "3d26f5d0-f72b-8029-54db-e849514cd3a4");
            Cvedia.Securt.UpdateInstanceOptions(handle, 1, 2, 2, 1, 10, 1, 0, 0, 0);
            Cvedia.Securt.SetInputToManual(handle);
            Cvedia.Securt.SetBlockingReadaheadQueue(handle, 1);
            Cvedia.Securt.Start(handle);
        }

        [TestMethod]
        public void TestGetInstanceId()
        {
            Assert.AreEqual(Cvedia.Securt.GetInstanceId(handle), "3d26f5d0-f72b-8029-54db-e849514cd3a4");
        }

        [TestMethod]
        public void TestGetInstanceName()
        {
            Assert.AreEqual(Cvedia.Securt.GetInstanceName(handle), "3d26f5d0-f72b-8029-54db-e849514cd3a4");
        }

        [TestMethod]
        public void TestGetFrameRateLimit()
        {
            Assert.AreEqual(Cvedia.Securt.GetFrameRateLimit(handle), 10);
        }

        [TestMethod]
        public void TestSetFrameRateLimit()
        {
            Cvedia.Securt.SetFrameRateLimit(handle, 10);
            Assert.AreEqual(Cvedia.Securt.GetFrameRateLimit(handle), 10);
        }

        [TestMethod]
        public void TestGetRecommendedFrameRate()
        {
            Assert.AreEqual(Cvedia.Securt.GetRecommendedFrameRate(handle), 10);
        }

        [TestMethod]
        public void TestGetFramesProcessedNone()
        {
            Assert.AreEqual(Cvedia.Securt.GetFramesProcessed(handle), 0);
        }

        [TestMethod]
        public void TestGetFramesProcessedVideo()
        {
            var metadataList = helpers.PlayVideo(handle, "person", true);

            Assert.AreEqual(Cvedia.Securt.GetFramesProcessed(handle), 40);
        }

        [TestMethod]
        public void TestNeedData()
        {
            Assert.AreEqual(Cvedia.Securt.NeedData(handle, 10), 1);
        }

        [TestMethod]
        public void TestCreateAreaTwice()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, -1);
        }

        [TestMethod]
        public void TestDeleteAreaTwice()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            int areaDeleted = Cvedia.Securt.DeleteArea(handle, "area_id");
            Assert.AreEqual(areaDeleted, 1);

            areaDeleted = Cvedia.Securt.DeleteArea(handle, "area_id");
            Assert.AreEqual(areaDeleted, -1);
        }

        [TestMethod]
        public void TestCreateAreaDelete()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            Thread.Sleep(1000);

            int areaDeleted = Cvedia.Securt.DeleteArea(handle, "area_id");
            Assert.AreEqual(areaDeleted, 1);

            areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);
        }

        [TestMethod]
        public void TestCreateAreaPersonEnter()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            // Assuming 'play_video' is a method available in your C# environment and returns List<string>.
            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            Assert.AreNotEqual(metadataList.Count, 0);

            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-exit" });
            helpers.AssertEvents(metadataList, new List<string> { "event-area-enter", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateAreaPersonEnterStationaryIgnore()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", true);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-enter", "event-area-exit", "track", "best-crop" });
        }

        [TestMethod]
        public void TestCreateAreaPersonEnterStationaryInclude()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", true);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-exit" });
            helpers.AssertEvents(metadataList, new List<string> { "event-area-enter", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateAreaPersonWrongClass()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 3 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-enter", "event-area-exit", "track", "best-crop" });
        }

        [TestMethod]
        public void TestCreateAreaVehicle()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 3 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "vehicle", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-exit" });
            helpers.AssertEvents(metadataList, new List<string> { "event-area-enter", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateAreaPersonExit()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 2, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-enter" });
            helpers.AssertEvents(metadataList, new List<string> { "event-area-exit", "best-crop" });
            Assert.IsFalse(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateAreaPersonEnterExit()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 3, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-area-enter", "event-area-exit", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateIntrusionAreaPerson()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateIntrusionArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-exit", "event-area-enter" });
            helpers.AssertEvents(metadataList, new List<string> { "event-intrusion", "event-intrusion-end", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestIntrusionMotionAreaInclude()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateIntrusionArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            int motionCreated = Cvedia.Securt.SetMotionArea(handle, pts, pts.Length);
            Assert.AreEqual(motionCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-exit", "event-area-enter" });
            helpers.AssertEvents(metadataList, new List<string> { "event-intrusion", "event-intrusion-end", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestIntrusionMotionAreaExclude()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateIntrusionArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            float[] motionPts = new float[] { 0.0f, 0.9f, 1.0f, 0.9f, 1.0f, 1.0f, 0.0f, 1.0f };
            int motionCreated = Cvedia.Securt.SetMotionArea(handle, motionPts, motionPts.Length);
            Assert.AreEqual(motionCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-area-exit", "event-area-enter", "event-intrusion", "track", "best-crop" });
        }

        [TestMethod]
        public void TestCreateIntrusionAreaPersonOutside()
        {
            float[] pts = new float[] { 0.0f, 0.9f, 1.0f, 0.9f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateIntrusionArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-loitering", "event-loitering-end", "track", "best-crop" });
        }

        [TestMethod]
        public void TestCreateLoiteringAreaPersonMoving()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateLoiteringArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-loitering", "event-loitering-end", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateLoiteringAreaPersonStationary()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateLoiteringArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", true);
            helpers.AssertEvents(metadataList, new List<string> { "event-loitering", "event-loitering-end", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateLoiteringAreaVehicleMoving()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateLoiteringArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 3 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "vehicle", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-loitering", "event-loitering-end", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateLoiteringAreaVehicleStationary()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateLoiteringArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 3 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "vehicle", true);
            helpers.AssertEvents(metadataList, new List<string> { "event-loitering", "event-loitering-end", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateLoiteringAreaPersonLong()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateLoiteringArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 60, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-loitering", "event-loitering-end", "track", "best-crop" });
        }

        [TestMethod]
        public void TestCreateCrowdingAreaPersonLow()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrowdingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-crowd-detection", "track" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateCrowdingAreaPersonHigh()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateCrowdingArea(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 60, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertNoEvents(metadataList, new List<string> { "event-crowd-detection", "track", "best-crop" });
        }

        [TestMethod]
        public void TestCreateLineCrossingPerson()
        {
            float[] pts = new float[] { 0.4f, 0.0f, 0.4f, 1.0f };

            int areaCreated = Cvedia.Securt.CreateLineCrossing(handle, "area_id", "area_name", pts, pts.Length, new int[] { 1 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-line-crossing", "track", "best-crop" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateLinePlusAreaPerson()
        {
            float[] linePts = new float[] { 0.4f, 0.0f, 0.4f, 1.0f };
            float[] areaPts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int lineCreated = Cvedia.Securt.CreateLineCrossing(handle, "area_id", "area_name", linePts, linePts.Length, new int[] { 1 }, 1, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(lineCreated, 1);

            int areaCreated = Cvedia.Securt.CreateCrossingArea(handle, "area_id", "area_name", areaPts, areaPts.Length, new int[] { 1 }, 1, 0, 3, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-line-crossing", "track", "best-crop", "event-area-enter", "event-area-exit" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        public void TestCreateIntrusionPlusAreaPerson()
        {
            float[] pts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            int areaCreated1 = Cvedia.Securt.CreateIntrusionArea(handle, "area_id1", "area_name", pts, pts.Length, new int[] { 1 }, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated1, 1);

            int areaCreated2 = Cvedia.Securt.CreateCrossingArea(handle, "area_id2", "area_name", pts, pts.Length, new int[] { 1 }, 1, 0, 3, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated2, 1);

            List<string> metadataList = helpers.PlayVideo(handle, "person", false);
            helpers.AssertEvents(metadataList, new List<string> { "event-area-enter", "event-area-exit", "track", "best-crop", "event-intrusion", "event-intrusion-end" });
            Assert.IsTrue(helpers.CheckTrackHasEvents(metadataList));
        }

        [TestMethod]
        [Ignore]
        public void TestConnectRtspWorking()
        {
            int instance = Cvedia.Securt.CreateContext("my_instance_id2", "");
            Cvedia.Securt.CreateInstance(instance, "my_instance_name");
            Cvedia.Securt.Start(instance);

            Cvedia.Securt.SetFrameRateLimit(instance, 15);

            int ret = Cvedia.Securt.SetInputToRtsp(instance, "rtsp://192.168.1.99:8554/loitering2");
            Assert.AreEqual(ret, 1);

            ret = Cvedia.Securt.EnableRtspOutput(instance, "rtsp://192.168.1.69:8554/test");
            Assert.AreEqual(ret, 1);

            float[] areaPts = new float[] { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
            int areaCreated = Cvedia.Securt.CreateCrossingArea(instance, "area_id", "area_name", areaPts, areaPts.Length, new int[] { 1 }, 1, 0, 1, new double[] { 1.0, 1.0, 1.0, 1.0 });
            Assert.AreEqual(areaCreated, 1);

            Thread.Sleep(10);

            // Call the consume_metadata method
            List<string> metadataList = new List<string>();
            while (true)
            {
                string json;

                var res = Cvedia.Securt.ConsumeEvents(instance, out json);
                if (res == 0)
                {
                    break;
                }

                metadataList.Add(json);
            }

            Assert.IsTrue(metadataList.Count > 0);

            Cvedia.Securt.Stop(instance);
            Cvedia.Securt.DestroyInstance(instance);
        }

        [TestCleanup]
        public void TestTeardown()
        {
            Cvedia.Securt.Stop(handle);
            Cvedia.Securt.DestroyInstance(handle);

            Cvedia.Rt.Shutdown();
        }
    }
}