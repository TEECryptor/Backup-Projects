using System;
using System.Collections.Generic;
using System.Text;
using Sonic.ManagedStorage;

namespace ProTestApp
{
    class Program
    {
        static void Main(string[] args)
        {

            String imagePath="";
            String xmlPath="";
            UInt32 numDests = 1;
            if (args.Length > 0)
                imagePath = args[0];
            if (args.Length > 1)
                xmlPath = args[1];
            if (args.Length > 2)
                numDests = System.UInt32.Parse(args[2]);
            UInt32[] destIndexList = new UInt32[numDests];
            destIndexList[0] = 1; // default to optical device 1
            if (args.Length > 2+numDests)
                for (int i = 0; i < numDests; i++)
                    destIndexList[i] = UInt32.Parse(args[3 + i]);

            if (imagePath != "")
            {
                Device srcDev = new Device();
                System.UInt32 numDevices = srcDev.get_DeviceCount(Device.ScanAll);
                int err = srcDev.Open(0, imagePath);
                if (err == StorageError.None)
                {

                    Device[] destList = new Device[numDests];
                    for (int i = 0; i < numDests; i++)
                    {
                        destList[i] = new Device();
                        destList[i].Open(destIndexList[i], null);
                        destList[i].XMLPath = xmlPath;
                    }
                    err = srcDev.Copy(Device.FormatTypeOpticalDetect, Device.CopyFlags_QflixPro, 
                        0, numDests, destList, null, System.IntPtr.Zero);
                    if (err != StorageError.None)
                        System.Console.WriteLine("Copy error {0}", err);
                    for (int i = 0; i < numDests; i++)
                    {
                        destList[i].Close();
                        destList[i] = null;
                    }
                    srcDev.Close();
                }


            }




        }
    }
}
