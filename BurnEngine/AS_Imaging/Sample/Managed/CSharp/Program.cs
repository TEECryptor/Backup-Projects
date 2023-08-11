using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Runtime.ConstrainedExecution;
using Microsoft.Win32.SafeHandles;

using Sonic.ManagedStorage;

namespace CSTestApp
{

    public class DelegateClass
    {

		private String GetStatusFromValue(System.UInt32 status)
		{
			if (status == Device.JobStatusAborted)
				return "Aborted";
			else if (status == Device.JobStatusBusyAborting)
				return "BusyAborting";
			else if (status == Device.JobStatusError)
				return "Error";
			else if (status == Device.JobStatusIdle)
				return "Idle";
			else if (status == Device.JobStatusPartialSuccess)
				return "Partial Success";
			else if (status == Device.JobStatusRunning)
				return "Running";
			else if (status == Device.JobStatusSuccess)
				return "Success";
			return "Unknown";
		}

		private String GetOperationTypeFromValue(System.UInt32 type)
		{
			if (type == Device.JobTypeAborting)
				return "Aborting";
			else if (type == Device.JobTypeComparing)
				return "Comparing";
			else if (type == Device.JobTypeErasing)
				return "Erasing";
			else if (type == Device.JobTypeFinalizing)
				return "Finalizing";
			else if (type == Device.JobTypeFormatting)
				return "Formatting";
			else if (type == Device.JobTypeInitializing)
				return "Initializing";
			else if (type == Device.JobTypeReading)
				return "Reading";
			else if (type == Device.JobTypeSeek)
				return "Seeking";
			else if (type == Device.JobTypeWriteFilemark)
				return "Writing Filemark";
			else if (type == Device.JobTypeWriting)
				return "Writing";
			
			return "Unknown";
		}
        private String GetVolumeOperationFromVal(System.UInt32 type)
        {
			if (type == Volume.ProgressTypeClose)
				return "Closing";
			else if (type == Volume.ProgressTypeFormat)
				return "Formatting";
			else if (type == Volume.ProgressTypeGeneral)
				return "General";
			else if (type == Volume.ProgressTypeMount)
				return "Mounting";
			else if (type == Volume.ProgressTypePlan)
				return "Planning";
			else if (type == Volume.ProgressTypeVerify)
				return "Verifying";
			else if (type == Volume.ProgressTypeWrite)
				return "Writing";

            return "Unknown";
        }
        public int DeviceCallback(System.IntPtr info, System.UInt32 infoDword, System.IntPtr infoPtr, System.IntPtr callerUse)
        {
            if (infoPtr != System.IntPtr.Zero)
            {
                Device.Progress details;// = new Device.Progress();
                try
                {
                    details = (Device.Progress)Marshal.PtrToStructure(infoPtr, typeof(Device.Progress));
				    int mgdsz = Marshal.SizeOf(typeof(Device.Progress));
				    if ((int)(details.ValidBytes) > 0 && (int)(details.ValidBytes) <= mgdsz)
				    {
                        if (details.TotalSectors != 0)
    					    Console.WriteLine("Detailed info is CurSector: {0}, Total: {1},", details.CurrentSector, details.TotalSectors);
                        String op=GetOperationTypeFromValue(details.CurrentOperation);
                        String status = GetStatusFromValue(details.ProgressStatus);
                        Console.WriteLine("Operation: {0}, Status: {1}", op, status);
				    }
                }
                catch (Exception e)
                {
                    Console.WriteLine("Exception thrown: {0}", e.ToString());
                }


            }
            Console.WriteLine("Completed: {0} %", infoDword);
            return 0;
        }
        public int VolumeCallback(System.IntPtr info, System.UInt32 infoDword, System.IntPtr infoPtr, System.IntPtr callerUse)
        {
            if (info != System.IntPtr.Zero)
            {
                Volume.Info updateType = (Volume.Info)(int)(Marshal.ReadInt32(info));

                if (updateType == Volume.Info.ProgressUpdate)
                {
                    if (infoPtr != System.IntPtr.Zero)
                    {
                        try
                        {
                            Volume.Progress details = new Volume.Progress();
                            details = (Volume.Progress)Marshal.PtrToStructure(infoPtr, typeof(Volume.Progress));
                            int mgdsz = Marshal.SizeOf(typeof(Volume.Progress));
                            if (details.ValidBytes > 0 && details.ValidBytes <= mgdsz)
                                Console.WriteLine("Detailed info is Device#: {0}, Status: {1}, CurSector: {2}, Total: {3}, Operation: {4}", details.DeviceHandle, GetStatusFromValue(details.ProgressStatus), details.CurrentSector, details.TotalSectors, GetVolumeOperationFromVal(details.ProgressType));

                        }
                        catch (Exception e)
                        {
                            Console.WriteLine("Exception thrown: {0}", e.ToString());
                        }

                    }
                    Console.WriteLine("Completed: {0} %", infoDword);
                }

            }
            return 0;
        }

        public int StreamCallback(System.Int64 pos, byte[] pBuffer,
                                                System.Int32 bytesRequested,
                                                System.IntPtr pBytesGiven, System.IntPtr callerUse)
        {
            try
            {
                using (SafeFileHandle safehdl = new SafeFileHandle(callerUse, false))
                {
                    using (FileStream strm = new FileStream(safehdl, FileAccess.Read))
                    {
                        using (BinaryReader bsr = new BinaryReader(strm))
                        {
                            strm.Seek(pos, SeekOrigin.Begin);
                            pBuffer = bsr.ReadBytes(bytesRequested);
                            Console.WriteLine("Streaming data from pos {0} ", pos);
                            Marshal.WriteInt32(pBytesGiven, bytesRequested);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                // write zeroes to buffer and continue on error
                byte[] zeroes = new byte[(int)bytesRequested];
                zeroes.CopyTo(pBuffer, 0);
                Marshal.WriteInt32(pBytesGiven, bytesRequested);
            }

            return 0;
        }


    };

    public class FileStreamList : CriticalFinalizerObject
    {
        List<BinaryReader> readers = new List<BinaryReader>();
        public void AddStream(BinaryReader rdr)
        {
            readers.Add(rdr);
            FileStream strm = getFileStream(rdr.BaseStream);
            if (strm != null)
            {
                GC.SuppressFinalize(strm);

                GC.SuppressFinalize(strm.SafeFileHandle);
            }

        }

        static FileStream getFileStream(Stream strm)
        {
            if (strm is FileStream)
                return (FileStream)strm;
            else
                return null;
        }

        ~FileStreamList()
        {
            foreach (BinaryReader rdr in readers)
            {
                FileStream strm = getFileStream(rdr.BaseStream);
                SafeFileHandle hdl = null;
                if (strm != null)
                    hdl = strm.SafeFileHandle;

                rdr.Close();
                if (hdl != null)
                    hdl.Close();
            }

        }
    };

    public class Program
    {
        const UInt32 OTHER_Execute = 0x00000001;
        const UInt32 OTHER_Write   = 0x00000002;
        const UInt32 OTHER_Read    = 0x00000004;
        const UInt32 OTHER_ChAttr  = 0x00000008;
        const UInt32 OTHER_Delete  = 0x00000010;
        const UInt32 GROUP_Execute = 0x00000020;
        const UInt32 GROUP_Write   = 0x00000040;
        const UInt32 GROUP_Read    = 0x00000080;
        const UInt32 GROUP_ChAttr  = 0x00000100;
        const UInt32 GROUP_Delete  = 0x00000200;
        const UInt32 OWNER_Execute = 0x00000400;
        const UInt32 OWNER_Write   = 0x00000800;
        const UInt32 OWNER_Read    = 0x00001000;
        const UInt32 OWNER_ChAttr  = 0x00002000;
        const UInt32 OWNER_Delete = 0x00004000;

        static Decimal pctDone = 0; 
        private static FileStreamList myList = new FileStreamList();

        UInt32 FSType;
        UInt32 VolAccessMode;
        UInt32 operation;
        UInt32 deviceIndex;
        String cmdDevicePath;
        String cmdSrcPath;
        String cmdDstPath;
        Device.EraseType EraseOption;
        UInt32 VerifyOption;
        Volume.FlushBorderOptions VolFlushOption;
        static Device[] destDeviceList;
        static String[] destDevicePaths;
        Volume.FormatType VolFormatType;
        bool isStreaming = false;
        Volume.StreamCallback streamcb = null;
        UInt32 AudioNumTracks = 0;
        System.Diagnostics.TextWriterTraceListener traceListener;
        FileStream traceStream;
        SDKProperties sdk;
        bool isCallingBack = true;
        UInt32 fileTypeForFileDevice = Device.FormatTypeOpticalDetect;
		Program(){
			FSType = Volume.FSUDF15;
			VolAccessMode = Volume.Mastering;
			operation = 99;// default to no operation (user didn't give cmd line)
			deviceIndex = 1;
			cmdDevicePath = ""; cmdSrcPath = ""; cmdDstPath = "";
            EraseOption = Device.EraseType.Quick;
            VerifyOption = 0;
            VolFlushOption = Volume.FlushBorderOptions.Close;
            destDevicePaths = new String[64];
            VolFormatType = 0;
            sdk = new SDKProperties();
            traceStream = File.OpenWrite("CSharpTrace.txt");
            if (traceStream.CanWrite)
            {
                traceListener = new System.Diagnostics.TextWriterTraceListener(traceStream);
                System.Diagnostics.Trace.Listeners.Add(traceListener);

            }
        }

        ~Program()
        {
            System.Diagnostics.Trace.Listeners.Remove(traceListener);
            traceStream.Close();
            sdk.Dispose();
        }
        private String GetStringFromDriverType(System.UInt32 val)
        {
            if (val == SDKProperties.Driver_Best)
                return "Best";
            else if (val == SDKProperties.Driver_Any)
                return "Any";
            else if (val == SDKProperties.Driver_Imapi)
                return "Imapi";
            else if (val == SDKProperties.Driver_Pxhelp)
                return "Pxhelp";

            return "";
        }
        private void OutputUTF8Array(System.ValueType[] propertyStringArray)
        {
            if (propertyStringArray != null)
            {
                UnicodeEncoding enc = new UnicodeEncoding();
                System.Byte[] byteString = new System.Byte[propertyStringArray.Length];
                //byteString = enc.GetBytes(propertyStringArray, 0, propertyStringArray.Length);
                propertyStringArray.CopyTo(byteString, 0);
                int idx = 0;
                while (idx < byteString.Length && byteString[idx] != 0)
                    Console.Write("{0}", (char)byteString[idx++]);
                Console.WriteLine();
            }

        }
        private void OutputIntArray(System.ValueType[] propertyIntArray)
        {
            if (propertyIntArray != null)
            {
                System.UInt32[] intArray = new System.UInt32[propertyIntArray.Length];
                propertyIntArray.CopyTo(intArray, 0);
                int idx = 0;
                while (idx < intArray.Length)
                    Console.Write("{0}", (int)intArray[idx++]);
                Console.WriteLine();
            }
        }
        private void OutputUTF8String(String propertyString)
        {
            if (propertyString != null)
            {
                UnicodeEncoding enc = new UnicodeEncoding();
                System.Byte[] byteString = new System.Byte[propertyString.Length];
                byteString = enc.GetBytes(propertyString.ToCharArray(), 0, propertyString.Length);

                int idx = 0;
                while (idx < byteString.Length && byteString[idx] != 0)
                    Console.Write("{0}", (char)byteString[idx++]);
                Console.WriteLine();
            }

        }

		void GetSDKProps()
		{
            String currentLogFolder = sdk.LogFolder;
            Console.Write("Current logfolder is:");
            OutputUTF8String(currentLogFolder);

            UInt32 driver = sdk.Driver;
            Console.WriteLine("Driver is: {0}", GetStringFromDriverType(driver));
			sdk.LogFolder="c:\\log";
            String newLogFolder = sdk.LogFolder;
            Console.Write("New logfolder is:");
            OutputUTF8String(newLogFolder);
            sdk.LogFolder = currentLogFolder;
            Console.Write("Current logfolder is:");
            OutputUTF8String(currentLogFolder);
            sdk.Dispose();

		}
        private String GetStringFromMediaState(System.UInt32 val)
        {
            String str= "";

            if ((val & Device.MediaIsErasable) != 0)
                str += "Erasable ";
            if ((val & Device.MediaIsAppendable) != 0)
                str += "Appendable ";
            if ((val & Device.MediaIsBlank) != 0)
                str += "Blank ";
            if ((val & Device.MediaIsInvisible) != 0)
                str += "Invisible ";
            if ((val & Device.MediaIsProtected) != 0)
                str += "Protected ";
            if ((val & Device.MediaIsReadable) != 0)
                str += "Readable ";
            if ((val & Device.MediaIsRewritable) != 0)
                str += "Rewritable ";
            if ((val & Device.MediaIsWritable) != 0)
                str += "Writable ";

            return str;
        }
        private String GetStringFromMediaType(System.UInt64 val)
        {
            if (val == Device.MediaIsBdr)
                    return "BDR";
                else if (val == Device.MediaIsBdrom)
                    return "BDRom";
                else if (val == Device.MediaIsCdr)
                    return "CDR";
                else if (val == Device.MediaIsCdRom)
                    return "CDRom";
                else if (val == Device.MediaIsCdrw)
                    return "CDRW";
                else if (val == Device.MediaIsDdCdr)
                    return "DDCDR";
                else if (val == Device.MediaIsDdCdrom)
                    return "DDCDRom";
                else if (val == Device.MediaIsDdCdrw)
                    return "DDCDRW";
                else if (val == Device.MediaIsDvdpr)
                    return "DVDPR";
                else if (val == Device.MediaIsDvdpr9)
                    return "DVDPR9";
                else if (val == Device.MediaIsDvdprw)
                    return "DVDPRW";
                else if (val == Device.MediaIsDvdr)
                    return "DVDR";
                else if (val == Device.MediaIsDvdr9)
                    return "DVDR9";
                else if (val == Device.MediaIsDvdRam)
                    return "DVDRAM";
                else if (val == Device.MediaIsDvdRom)
                    return "DVDROM";
                else if (val == Device.MediaIsDvdrw)
                    return "DVDRW";
                else if (val == Device.MediaIsHdDvdr)
                    return "HDDVDR";
                else if (val == Device.MediaIsHdDvdRom)
                    return "HDDVDRom";
                else if (val == Device.MediaIsHdDvdrw)
                    return "HDDVDRW";
                else                    
                    return "Unknown media type";
  
        }
        private static String GetStringFromUDFPerms(System.UInt32 perms)
        {
            //rwxcd - we will show permissions in this format
            // read, write, exec, chattr, delete for owner,group,others

            String retval="";
            if ((perms & Program.OWNER_Read) != 0)
                retval += "r";
            else
                retval += "-";
            if ((perms & Program.OWNER_Write) != 0)
                retval += "w";
            else
                retval += "-";
            if ((perms & Program.OWNER_Execute) != 0)
                retval += "x";
            else
                retval += "-";
            if ((perms & Program.OWNER_ChAttr) != 0)
                retval += "c";
            else
                retval = "-";
            if ((perms & Program.OWNER_Delete) != 0)
                retval += "d";
            else
                retval += "-";
            if ((perms & Program.GROUP_Read) != 0)
                retval += "r";
            else
                retval += "-";
            if ((perms & Program.GROUP_Write) != 0)
                retval += "w";
            else
                retval += "-";
            if ((perms & Program.GROUP_Execute) != 0)
                retval += "x";
            else
                retval += "-";
            if ((perms & Program.GROUP_ChAttr) != 0)
                retval += "c";
            else
                retval = "-";
            if ((perms & Program.GROUP_Delete) != 0)
                retval += "d";
            else
                retval += "-";
            if ((perms & Program.OTHER_Read) != 0)
                retval += "r";
            else
                retval += "-";
            if ((perms & Program.OTHER_Write) != 0)
                retval += "w";
            else
                retval += "-";
            if ((perms & Program.OTHER_Execute) != 0)
                retval += "x";
            else
                retval += "-";
            if ((perms & Program.OTHER_ChAttr) != 0)
                retval += "c";
            else
                retval = "-";
            if ((perms & Program.OTHER_Delete) != 0)
                retval += "d";
            else
                retval += "-";

            return retval;
        }
        System.Int32 GetFileInfo(Volume.FormatType format, UInt32 fsType, ref RTFile file)
        {
//            System.Int32 err = 0;
            System.UInt32 uintVal = 0;
            System.UInt64 ulongVal = 0;
            String filename = file.Name;
            Console.Write("Filename from media is: ");
            OutputUTF8String(filename);
            ulongVal = file.Size;
            Console.WriteLine("Filesize is: {0}", ulongVal);
            uintVal = file.LBA;
            Console.WriteLine("LBA is: {0}", uintVal);
            ulongVal = file.CurrentPosition;
            Console.WriteLine("Current position is: {0}", ulongVal);
            uintVal = file.Realtime;
            Console.WriteLine("Realtime is: {0}", uintVal);
            if (fsType == Volume.FSUDF102
                || fsType == Volume.FSUDF15
                || fsType == Volume.FSUDF2
                || fsType == Volume.FSUDF201
                || fsType == Volume.FSUDF25
                || fsType == Volume.FSUDF26
                || fsType == Volume.FSBridge)
            {
                uintVal = file.UDFPermissions;
                Console.WriteLine("UDFPermissions is: {0}", GetStringFromUDFPerms(uintVal));
            }
            if (format == Volume.FormatType.Type_DVD)
            {
                RTFile.CgmsInfo info = file.Cgms;
                if (info != null)
                    Console.WriteLine("CGMS is {0}", info.ToString());
            }

            Volume.DateTime date = file.DateCreated;
            if (date != null)
                Console.WriteLine("DateCreated is {0}", GetStringFromVolDate(date));
            Volume.DateTime datemod = file.DateModified;
            if (datemod != null)
                Console.WriteLine("DateModified is {0}", GetStringFromVolDate(datemod));
            RTFile.MacExtHFSType macType = file.MacExtHFS;
            if (macType != null)
                Console.WriteLine("MacExtHFS is {0}", macType.ToString());
            RTFile.MacExtProDosType macProDos = file.MacExtProDos;
            if (macProDos != null)
                Console.WriteLine("MacExtProDos is {0}", macProDos.ToString());

            return 0;
        }
        String GetStringFromVolDate(Volume.DateTime date)
        {
            if (date != null)
            {
                String retVal = date.month.ToString() +"/"+ date.day+"/"+ date.year+"  "+
                                                            date.hour + ":" + date.minute + ":" + date.second;

                System.DateTime ourtime = new System.DateTime(date.year, date.month, date.day,
                                                date.hour, date.minute, date.second);

                return ourtime.ToString();
            }
            return "";
        }
        String GetStringFromVolProt(Volume.ContentProtectionType val)
        {
            if (val == Volume.ContentProtectionType.AACS)
                return "AACS";
            else if (val == Volume.ContentProtectionType.CSS)
                return "CSS";
            else
                return "None";
        }
        String GetStringFromVolState(System.UInt32 val)
        {
            String str = "";
            if ((val & Volume.FileIsAppendable) == 1)
                str += "Appendable ";
            if ((val & Volume.FileIsDirty) == 1)
                str += "Dirty ";
            if ((val & Volume.FileIsMounted) == 1)
                str += "Mounted ";

            return str;
        }

        System.Int32 GetVolumeInfo(ref Volume vol)
        {
//            System.Int32 err = 0;
            System.UInt32 uintVal = 0;
            System.UInt64 ulongVal = 0;
            String appID = vol.ApplicationID;
            Console.Write("Application ID: ");
            OutputUTF8String(appID);

            uintVal = vol.BlockSize;
            Console.WriteLine("BlockSize is: {0}", uintVal);
            uintVal = vol.EstimatedFreeBlocks;
            Console.WriteLine("EstimatedFreeBlocks is: {0}", uintVal);
            uintVal = vol.EstimatedUsedBlocks;
            Console.WriteLine("EstimatedUsedBlocks is: {0}", uintVal);
            uintVal = vol.FastWrite;
            Console.WriteLine("FastWrite is: {0}", uintVal);
            uintVal = vol.FreeBlocks;
            Console.WriteLine("FreeBlocks is: {0}", uintVal);
            uintVal = vol.NumFiles;
            Console.WriteLine("NumFiles is: {0}", uintVal);
            uintVal = vol.NumFolders;
            Console.WriteLine("NumFolders is: {0}", uintVal);
            Volume.ContentProtectionType prot = vol.Protection;
            Console.WriteLine("Protection: {0}", GetStringFromVolProt(prot));

            uintVal = vol.VolumeState;
            Console.WriteLine("VolumeState: {0}", GetStringFromVolState(uintVal));
            String volName = vol.VolumeName;
            Console.Write("VolumeName: ");
            OutputUTF8String(volName);
            Volume.DateTime date = vol.DateCreated;
            Console.WriteLine("Volume date Created: {0}", GetStringFromVolDate(date));

            date = vol.DateModified;
            Console.WriteLine("Volume date Modified: {0}", GetStringFromVolDate(date));

            return 0;
        }
        System.Int32 GetMediaInfo(ref Device mgr)
        {
            System.Int32 err = 0;

            Device.MediaProp_Type ourType = Device.MediaProp_Type.MedProp_Num_Sessions;
            foreach (String ourStr in Enum.GetNames(ourType.GetType()))
            {

                ourType = (Device.MediaProp_Type)Enum.Parse(typeof(Device.MediaProp_Type), ourStr);
                int val = (int)ourType;

                Console.Write("Prop is: {0}  ", ourStr);

                Object obj = new Object();
                obj = mgr.GetMediaObjectProperty(ourType);

                if (ourType == Device.MediaProp_Type.MedProp_State)
                    Console.WriteLine("Value is: {0}", GetStringFromMediaState((System.UInt32)obj));
                else if (ourType == Device.MediaProp_Type.MedProp_Kind)
                    Console.WriteLine("Value is: {0}", GetStringFromMediaType((System.UInt64)obj));
                else if (ourType == Device.MediaProp_Type.MedProp_CurRead_Hundredth_X ||
                    (ourType == Device.MediaProp_Type.MedProp_CurWrite_Hundredth_X))
                {
                    Console.WriteLine("Value in X is: {0}X", ((System.UInt32)obj)/100);
                }
                else if (ourType == Device.MediaProp_Type.MedProp_Manufacturer)
                {
                    System.ValueType[] byteArray = new System.ValueType[256];
                    byteArray = ((System.ValueType[])obj);
                    OutputUTF8Array(byteArray);
                }
                else if (ourType == Device.MediaProp_Type.MedProp_WriteList_DataRates ||
                    (ourType == Device.MediaProp_Type.MedProp_WriteList_Hundredth_X))
                {
                    System.ValueType[] intArr = new System.ValueType[10];
                    intArr = ((System.ValueType[])obj);

                    OutputIntArray(intArr);
                }
                else
                    System.Console.WriteLine("Value is: {0}", obj);
            }
            /*
            System.UInt32 uintVal = 0;
            System.UInt64 longVal = 0;
            uintVal = mgr.MediaAuthenticationType;
            Console.WriteLine("Media Auth is: {0}", uintVal);
            longVal = mgr.MediaBytesFree;
            Console.WriteLine("BytesFree is: {0}", longVal);
            longVal = mgr.MediaBytesUsed;
            Console.WriteLine("BytesUsed is: {0}", longVal);
            uintVal = mgr.MediaCopyProtection;
            Console.WriteLine("Copy Prot is: {0}", uintVal);
            uintVal = mgr.MediaCurReadSpeed;
            Console.WriteLine("ReadSpeed is: {0}", uintVal);
            uintVal = mgr.MediaCurReadSpeedHundrethX;
            Console.WriteLine("ReadSpeed in X is: {0}X", (uintVal/100));
            uintVal = mgr.MediaCurrentState;
            Console.WriteLine("IO State is: {0}", GetStringFromMediaState(uintVal));
            uintVal = mgr.MediaCurWriteSpeed;
            Console.WriteLine("WriteSpeed is: {0}", uintVal);
            uintVal = mgr.MediaCurWriteSpeedHundrethX;
            Console.WriteLine("WriteSpeed100thX is: {0}", uintVal);
            uintVal = mgr.MediaDataMode;
            Console.WriteLine("DataMOde is: {0}", uintVal);
            uintVal = mgr.MediaFileFormat;
            Console.WriteLine("FileFormat is: {0}", uintVal);
            longVal = mgr.MediaKind;
            Console.WriteLine("Kind is: {0}", GetStringFromMediaType(longVal));
            longVal = mgr.MediaLayerJumpAddress;
            Console.WriteLine("LJAddr is: {0}", longVal);
            Console.Write("Manufacturer is:");
            OutputUTF8Array(mgr.MediaManufacturer);
            
            Console.WriteLine();
            longVal = mgr.MediaMaxLayerJumpAddress;
            Console.WriteLine("MaxLJ Addr is: {0}", longVal);
            longVal = mgr.MediaMinLayerJumpAddress;
            Console.WriteLine("Min LJ Addr is: {0}", longVal);
            uintVal = mgr.MediaNumLayers;
            Console.WriteLine("NumLayers is: {0}", uintVal);
            uintVal = mgr.MediaNumSessions;
            Console.WriteLine("NumSessions is: {0}", uintVal);
            uintVal = mgr.MediaNumTracks;
            Console.WriteLine("NumTracks is: {0}", uintVal);

            uintVal = mgr.MediaRecordMode;
            bool IsTestMode = mgr.MediaTestWrite;
            Console.WriteLine("TestMode is {0}", IsTestMode);
            int arrSize = 0;
            if (mgr.MediaWriteSpeedList.GetLength(0) > 0)
                arrSize = mgr.MediaWriteSpeedList.GetLength(0);
            if (arrSize > 0)
            {
                System.UInt32[] uintArr = new System.UInt32[arrSize];
                mgr.MediaWriteSpeedList.CopyTo(uintArr, 0);
                int x = 0;
                while (x < arrSize)
                {
                    Console.WriteLine("WriteSpeed List #{0} is: {1}", x, uintArr[x]);
                    x++;
                }
            }
            if (mgr.MediaWriteSpeedListHundrethX.GetLength(0) > 0)
                arrSize = mgr.MediaWriteSpeedListHundrethX.GetLength(0);
            if (arrSize > 0)
            {
                System.UInt32[] uintArrX = new System.UInt32[10];
                mgr.MediaWriteSpeedListHundrethX.CopyTo(uintArrX, 0);
                int x = 0;
                while (x < arrSize)
                {
                    Console.WriteLine("WriteSpeed in X List #{0} is: {1}X", x, uintArrX[x] / 100);
                    x++;
                }
            }
            */
            return 0;

        }

        System.Int32 GetDeviceInfo(ref Device mgr)
		{
			System.Int32 err = 0;

			err = mgr.TrayControl(Device.TrayCtrl.CloseSync);

            String devName = mgr.DeviceName;
            String devVendor = mgr.DeviceVendor;
            String devProd = mgr.DeviceProduct;
            String devVersion = mgr.DeviceVersion;
            String devSerialNum = mgr.DeviceSerialNo;

			Console.WriteLine("Device name is: {0}",devName.ToString());
            Console.WriteLine("DeviceVendor is: {0}", devVendor.ToString());
            Console.WriteLine("DeviceProduct is: {0}", devProd.ToString());
            Console.WriteLine("DeviceVersion is: {0}", devVersion.ToString());
            Console.WriteLine("DeviceSerialNo is: {0}", devSerialNum.ToString());

            Device.BusData info = new Device.BusData();
            info = mgr.DeviceBusInfo;
            System.UInt32 DevType = mgr.DeviceType;
            Device.State st = mgr.DeviceState;

			if (st == Device.State.Ready)
			{
				Console.WriteLine("State is Ready");
			}
			else
				err = mgr.TrayControl(Device.TrayCtrl.Open);

            Device.DevProp_Type ourType = Device.DevProp_Type.DevProp_AuthenTypes;
            foreach (String ourStr in Enum.GetNames(ourType.GetType()))
            {

                ourType = (Device.DevProp_Type)Enum.Parse(typeof(Device.DevProp_Type), ourStr);
                int val = (int)ourType;

                Console.Write("Prop is: {0}  ", ourStr);

                Object obj =  new Object();
                obj = mgr.GetDeviceObjectProperty(ourType);

                System.Console.WriteLine("Value is: {0}", obj.ToString());
            }
			return err;


		}
        System.Int32 doWriteFormat(ref Device mgr, ref String srcPath)
        {
            Format mgdFormat = new Format(mgr);
            Volume.InfoCallback cb = null;
            if (isCallingBack)
                cb = new Volume.InfoCallback(new DelegateClass().VolumeCallback);
            Int32 err = mgdFormat.Open(cb, System.IntPtr.Zero);

            err = mgdFormat.Create(FSType, VolFormatType, "FormatCSTestApp", VolAccessMode, cb, System.IntPtr.Zero);

            switch (VolFormatType)
            {
                case Volume.FormatType.Type_AudioCD:
                    {
                        Audio audioCD = new Audio(mgdFormat);
                        DirectoryInfo dirInfo = new DirectoryInfo(srcPath);
                        if (!dirInfo.Exists)
                            return -1;
                        FileInfo[] subfileList = dirInfo.GetFiles();
                        for (System.UInt32 i = 0; i < subfileList.Length; i++)
                        {
                            String audioFileName = subfileList[i].FullName;
                            Audio.FileType fileType = Audio.FileType.AutoDetect;
                            if (isStreaming)
                                // streaming only does PCM for now
                                fileType = Audio.FileType.PCM;
                            else
                            {
                                // get file type based on extension of file
                                String upperExtension = (subfileList[i].Extension.Substring(1)).ToUpper();
                                if (upperExtension == "WAV")
                                   fileType = Audio.FileType.WAV;
                                else if (upperExtension == "RAW")
                                   fileType = Audio.FileType.PCM;
                                else if (upperExtension == "MP3")
                                   fileType = Audio.FileType.MP3;

                            }

                            if (isStreaming)
                            {
                                doWriteAudioFileFromStream(ref mgdFormat, ref audioCD, ref audioFileName, ref fileType);
                            }
                            else
                            {
                                System.UInt32 trk = 0;
                                err = audioCD.AddAudioTrackFromFile(audioFileName, fileType, out trk);
                                System.Console.WriteLine("Added track {0} as filename {1}", trk, audioFileName);
                            }
                        }
                    }
                    break;
                case Volume.FormatType.Type_DVD:
                    {
                        err = mgdFormat.CreateEntryFromHD(srcPath, "/VIDEO_TS", RTFile.CreateParents);
                        if (err == StorageError.Fatal)
                            return err;
                    }
                    break;
            }

            err = mgdFormat.Prepare(VolFlushOption, VerifyOption, cb, System.IntPtr.Zero);

            err = mgdFormat.Flush(VolFlushOption, VerifyOption, cb, System.IntPtr.Zero);

            err = mgdFormat.Close();

            return err;
        }

		System.Int32 doWriteVolume(ref Device mgr, ref String srcPath, ref String destPath)
		{
   			Volume mgdVol = new Volume(mgr);

			System.UInt32 fstype=0;
            Volume.InfoCallback cb = null;
            if (isCallingBack)
                cb = new Volume.InfoCallback(new DelegateClass().VolumeCallback);
			Int32 err = mgdVol.Open(cb, System.IntPtr.Zero);
            fstype = mgdVol.FileSystemType;


			if (fstype == 0)
				err = mgdVol.Create(FSType, "TestMgdVolume", VolAccessMode, cb, System.IntPtr.Zero);
			else
                err = mgdVol.Mount(VolAccessMode, cb, System.IntPtr.Zero);

            if (VolAccessMode == Volume.Mastering && isStreaming)
                streamcb = new Volume.StreamCallback(new DelegateClass().StreamCallback);
           // if (VolAccessMode == Volume.RTTD)
           // {
                err = TraverseDirectory(ref mgdVol, ref srcPath, ref destPath);
                //Volume.FlushBorderOptions options = Volume.FlushBorderOptions.Close;
                //err = mgdVol.Flush(options, Volume.FlushVerify, cb, System.IntPtr.Zero);
           // }
           // else
           // {
           //     err = mgdVol.CreateEntryFromHD(srcPath, destPath, 0);
                /*  MOVE THESE OPTIONS TO COMMON PLACE SO WE CAN TRAVERSE AND SELECT WRITING OPTION
                // just for files test for now
                FileInfo srcfileTest = new FileInfo(srcPath);
                if (!srcfileTest.Exists)
                    return -1;
                System.UInt64 streamLen = (System.UInt64)srcfileTest.Length;
                FileStream Fstream = new FileStream(srcPath,FileMode.Open, FileAccess.Read);
                BinaryReader rdr = new BinaryReader(Fstream);
                myList.AddStream(rdr);

                SafeFileHandle handle = Fstream.SafeFileHandle;
                Volume.StreamCallback streamcb = new Volume.StreamCallback(new DelegateClass().StreamCallback);
                IntPtr ptr = Fstream.SafeFileHandle.DangerousGetHandle();

                err = mgdVol.CreateFileFromStream(destPath, streamLen, streamcb, ptr, RTFile.Create_Overwrite | RTFile.Create_Parents);
                */
               

         //   }

            if (destDevicePaths[0] == null)
            {
                // in case extra option for multiple targets wasn't supplied, set destDevicePaths to 1st arg (dest device)
                destDevicePaths[0] = cmdDevicePath;
            }
            else
            {
                destDeviceList = new Device[destDevicePaths.GetLength(0)];
                for (int i = 0; i < destDevicePaths.GetLength(0); i++)
                {
                    destDeviceList[i] = new Device();
                    destDeviceList[i].Open(UInt32.Parse(destDevicePaths[i]), "");
                }
            }

            if (destDeviceList != null)
                err = mgdVol.SetDevices(0, (UInt32)destDeviceList.GetLength(0), destDeviceList, cb, System.IntPtr.Zero);

            Volume.FlushBorderOptions options = Volume.FlushBorderOptions.Close;
            err = mgdVol.Flush(options, Volume.FlushVerify, cb, System.IntPtr.Zero);
            if (destDeviceList != null)
            {
                for (int i = 0; i < destDevicePaths.GetLength(0); i++)
                {
                    destDeviceList[i].Close();
                }
            }
			err = mgdVol.Close();

            mgdVol.Dispose();

			return err;
		}

		Int32 doCopy(ref Device mgr, ref String dstPath)
		{

			Int32 err = 0;

            UInt32 numAddlDests = 0;
            while (destDevicePaths[numAddlDests] != null)
                numAddlDests++;

           
            if (destDevicePaths[0] == null)
            {
                // in case extra option for multiple targets wasn't supplied, set destDevicePaths to 1st arg (dest device)
                destDevicePaths[0] = dstPath;
                numAddlDests = 1;
            }
            Device[] destArrayList = new Device[numAddlDests];
            for (int i = 0; i < numAddlDests; i++)
            {
                UInt32 destIndex = 0;
                try
                {
                    destIndex = UInt32.Parse(destDevicePaths[i]);
                }
                catch
                {
                    destIndex = 0;
                }


                destArrayList[i] = new Device();
                destArrayList[i].Open(destIndex, dstPath);
                String devName = destArrayList[i].DeviceName;
                String devVendor = destArrayList[i].DeviceVendor;
                String devProd = destArrayList[i].DeviceProduct;
                String devVersion = destArrayList[i].DeviceVersion;
                String devSerialNum = destArrayList[i].DeviceSerialNo;

            }

            Device.Callback cb = null;
            if (isCallingBack)
                cb = new Device.Callback(new DelegateClass().DeviceCallback);

            err = mgr.Copy(fileTypeForFileDevice, Device.CopyFlagsWrite, 0, numAddlDests, destArrayList, cb, System.IntPtr.Zero);

            foreach (Device ourDev in destArrayList)
                ourDev.Dispose();

			return err;
		}

		Int32 doReadFile(ref Volume mgdVol, ref String Path)
		{
			System.Int32 err = 0;

            //GetVolumeInfo(ref mgdVol);
			
            RTFile mgdFile = new RTFile(mgdVol);

			RTFile.OpenMode access = RTFile.OpenMode.Read;
			mgdFile.Open(Path, access);

            GetFileInfo(mgdVol.VolumeFormatType, mgdVol.FileSystemType, ref mgdFile);

			System.UInt64 size = mgdFile.Size;
			System.UInt32 readSize = 32768;
			System.UInt64 curPos = 0;
            System.UInt32 numread = (System.UInt32)0;

            pctDone = 0;
			while (curPos < size)
			{
				if (readSize > (size - curPos))
					readSize = (System.UInt32 )(size - curPos);
				Byte[] byteArray = new Byte[readSize];
				mgdFile.Read(readSize, byteArray, out numread);

				curPos += readSize;
                UInt64 fileCurrentPos = mgdFile.CurrentPosition;
                if (fileCurrentPos != curPos)
                    Console.WriteLine("position out of sync: OurPos={0}, MediaPos={1}", curPos, fileCurrentPos);

                if (pctDone != (curPos * 100) / size)
                {
                    pctDone = curPos * 100 / size;

                    if ((pctDone % 10) == 0)
                    {
                        Console.Write("Currentpos = {0}  ", fileCurrentPos);
                        Console.WriteLine("Read {0} %", pctDone.ToString());
                    }
                }
			}
			mgdFile.Close();

            mgdFile.Dispose();


			return err;
		}
		bool Equivalent(Byte[] lhs,
						Byte[] rhs)
		{
			bool equivalent = true;

			if (lhs != rhs)
			{
				if (null == lhs || 
					null == rhs ||
					lhs.Length != rhs.Length)
				{
					equivalent = false;
				}
				else
				{
					for (int index = 0; equivalent && index != lhs.Length; ++index)
					{
						equivalent = lhs[index] == rhs[index];
					}
				}
			}

			return equivalent;
		}

		Int32 doWriteFile(ref Volume mgdVol, String srcPath, ref String destPath)
		{

			Int32 err = 0;
			RTFile mgdFile = new RTFile(mgdVol);
            FileInfo srcfileTest = new FileInfo(srcPath);

            if (!srcfileTest.Exists)
                return -1;

			FileStream Fstream = new FileStream(srcPath, FileMode.Open);
			BinaryReader bsr = new BinaryReader(Fstream);
			
			RTFile.OpenMode access = RTFile.OpenMode.Append;

			Console.WriteLine("Writing to file: {0}",destPath);
			err = mgdFile.Open(destPath, access);


            System.UInt64 srcSize = (UInt64)Fstream.Length; // this can't return 64bit length??

//			System.UInt64 size = mgdFile.FileSize[srcPath];
			UInt32 readSize = 32768;
			System.UInt64 curPos = 0;
            pctDone = 0;
            System.UInt32 numwrote = (System.UInt32)0;
			while (curPos < srcSize)
			{
				if ((UInt64)readSize > (srcSize - curPos))
					readSize = (UInt32 )(srcSize - curPos);

				byte[] srcbyteArray = new byte[readSize];
				srcbyteArray = bsr.ReadBytes((Int32)readSize);

				//array<unsigned char>^ byteArray = gcnew array<unsigned char>(readSize);
//				array<unsigned char>^ byteArray = gcnew array<unsigned char>(readSize);
				err = mgdFile.Write(readSize, srcbyteArray, out numwrote);

				//if (!Equivalent(byteArray, srcbyteArray))
				//	break;


				//Console.WriteLine("Wrote {0}", curPos.ToString(), " to {0}", (curPos+readSize).ToString());
				curPos += readSize;
                if (pctDone != (curPos * 100) / srcSize) 
                    pctDone = curPos * 100 / srcSize;

				if ((pctDone % 10) == 0)
					Console.WriteLine("Wrote {0} %", pctDone.ToString());

			}

			err = mgdFile.Close();
            mgdFile.Dispose();

			bsr.Close();
			Fstream.Close();

			return err;
		}

		Int32 doVerifyFile(ref Volume mgdVol, ref String srcPath, ref String destPath)
		{

			Int32 err = 0;
			RTFile mgdFile = new RTFile(mgdVol);
            FileInfo srcfileTest = new FileInfo(destPath);

            if (!srcfileTest.Exists)
                return -1;
			FileStream Fstream = new FileStream(destPath, FileMode.Open);
			BinaryReader bsr = new BinaryReader(Fstream);
			
			RTFile.OpenMode access = RTFile.OpenMode.Read;
			err = mgdFile.Open(srcPath, access);

			
			System.UInt64 srcSize = (UInt64)Fstream.Length;   // this can't return 64bit length??

            err = GetFileInfo(mgdVol.VolumeFormatType, mgdVol.FileSystemType, ref mgdFile);

            System.UInt64 size = mgdFile.Size;
			System.UInt32 readSize = 32768;
			System.UInt64 curPos = 0;
            pctDone = 0;
            System.UInt32 numread = (System.UInt32)0;
			while (curPos < size)
			{
				if (readSize > (size - curPos))
					readSize = (System.UInt32 )(size - curPos);

				byte[] srcbyteArray = new byte[readSize];
				srcbyteArray = bsr.ReadBytes((int)readSize);

				//array<unsigned char>^ byteArray = gcnew array<unsigned char>(readSize);
				byte[] byteArray = new byte[readSize];
				mgdFile.Read(readSize, byteArray, out numread);

				if (!Equivalent(byteArray, srcbyteArray))
					break;

				curPos += readSize;

                if (pctDone != (curPos * 100) / size)
                {
                    pctDone = curPos * 100 / size;

                    if ((pctDone % 10) == 0)
                        Console.WriteLine("Verified {0} %", pctDone.ToString());
                }
			}
			err = mgdFile.Close();
            mgdFile.Dispose();

			bsr.Close();
			Fstream.Close();

			return err;
		}
        Int32 doWriteAudioFileFromStream(ref Format mgdFormat, ref Audio mgdAudio, ref String srcPath, ref Audio.FileType fileType)
        {
            FileInfo srcfileTest = new FileInfo(srcPath);
            if (!srcfileTest.Exists)
                return -1;
            System.UInt64 streamLen = (System.UInt64)srcfileTest.Length;
            FileStream Fstream = new FileStream(srcPath, FileMode.Open, FileAccess.Read);
            BinaryReader rdr = new BinaryReader(Fstream);
            myList.AddStream(rdr);

            SafeFileHandle handle = Fstream.SafeFileHandle;

            IntPtr ptr = Fstream.SafeFileHandle.DangerousGetHandle();

            System.UInt32 trk = 0;
            Int32 err = mgdAudio.AddAudioTrackFromStream(streamLen, streamcb, ptr, fileType, out trk);
            return err;
        }
        Int32 doWriteFileFromStream(ref Volume mgdVol, ref String srcPath, ref String destPath)
        {
            FileInfo srcfileTest = new FileInfo(srcPath);
            if (!srcfileTest.Exists)
                return -1;
            System.UInt64 streamLen = (System.UInt64)srcfileTest.Length;
            FileStream Fstream = new FileStream(srcPath, FileMode.Open, FileAccess.Read);
            BinaryReader rdr = new BinaryReader(Fstream);
            myList.AddStream(rdr);

            SafeFileHandle handle = Fstream.SafeFileHandle;
            
            IntPtr ptr = Fstream.SafeFileHandle.DangerousGetHandle();

            Int32 err = mgdVol.CreateFileFromStream(destPath, streamLen, streamcb, ptr, RTFile.CreateOverwrite | RTFile.CreateParents);

            return err;
        }
        Int32 doWriteFileFromHD(ref Volume mgdVol, ref String srcPath, ref String destPath)
        {
            Int32 err = mgdVol.CreateEntryFromHD(srcPath, destPath, RTFile.CreateParents|RTFile.CreateOverwrite);
            return err;
        }
		Int32 TraverseDirectory(ref Volume mgdVol, ref String rootPath, ref String destPath)
		{

			UInt64 numChildren = 0;

			System.Int32 err = 0;
			String curPath = rootPath;
            String delim = "\\";
			bool isFile = false;
			if (operation != 0)
			{
                // traversing for reading/verifying
				if (mgdVol.get_EntryKind(rootPath) == RTFile.FileIsDir)
				{
					numChildren = mgdVol.get_EntrySize(curPath);
					isFile = false;
				}
				else
					isFile = true;	

				if (isFile)
				{
					// root provided was a file
					Console.WriteLine("Found file: {0}", curPath);
					if (operation == 3)
						// read file
						err = doReadFile(ref mgdVol, ref curPath);
					else if (operation == 2)
						// verifyFile
						err = doVerifyFile(ref mgdVol, ref curPath, ref destPath);
					//else if (operation == 0)
					//	err = doWriteFile(mgdVol, curPath, destPath);
					else
						Console.WriteLine("Failed to traverse - invalid operation");
				}

				for (System.Int32 i=0; i<(System.UInt32)numChildren; i++)
				{
					String  newPath = "";
                    newPath = mgdVol.get_Entry(curPath, i);
                    String  newDestPath = "";
                    if (destPath != "")
                    {
                        // get just filename from media fullpath
                        String newFileNamePath = newPath.Substring(newPath.LastIndexOf('/'));
                        newDestPath = destPath + newFileNamePath;
                        // make all slashes match delim
                        if (newDestPath.Contains("/") && delim != "/")
                            newDestPath = newDestPath.Replace('/', '\\');
                    }
                    else
                        newDestPath = "";

					if (mgdVol.get_EntryKind(newPath) == RTFile.FileIsFile)
						isFile = true;
					else
						isFile = false;
					if (isFile)
					{
						Console.WriteLine("Found file: {0}", newPath);
						if (operation == 3)
							// read file
							err = doReadFile(ref mgdVol, ref newPath);
						else if (operation == 2)
							// verifyFile
							err = doVerifyFile(ref mgdVol, ref newPath, ref newDestPath);
						else
							Console.WriteLine("Failed to traverse - invalid operation");
					}
					else
					{
						err = TraverseDirectory(ref mgdVol, ref newPath, ref newDestPath);
					}
				}
			}
			else
			{
                // traversing for writing

                DirectoryInfo dirInfo = new DirectoryInfo(rootPath);
				if (!dirInfo.Exists)
				{
					// check if it is just a file
					FileInfo finfo = new FileInfo(rootPath);
					if (!finfo.Exists)
						return -1;  // error path is neither file nor dir

                    if (VolAccessMode == Volume.Mastering)
                    {
                        if (isStreaming)
                            err = doWriteFileFromStream(ref mgdVol, ref rootPath, ref destPath);
                        else
                            err = doWriteFileFromHD(ref mgdVol, ref rootPath, ref destPath);
                    }
                    else if (VolAccessMode == Volume.RTTD)
                    {
                        err = doWriteFile(ref mgdVol, rootPath, ref destPath);
                    }

				}
				else
				{
					// traverse all of a directory's entries at this level and recurse subdirs
					DirectoryInfo[] subdirList = dirInfo.GetDirectories();
					FileInfo[] subfileList = dirInfo.GetFiles();

					String newPath = "";

					for (int i=0; i<subfileList.Length; i++)
					{
						newPath = subfileList[i].ToString();		// TODO - fix for file and / or dir
						String newDestPath = destPath + "\\" + newPath;
						Console.WriteLine("Found file: {0}", newPath);
                        String newDestName = subfileList[i].FullName;
                        if (VolAccessMode == Volume.Mastering)
                        {
                            if (isStreaming)
                                err = doWriteFileFromStream(ref mgdVol, ref newDestName, ref newDestPath);
                            else
                                err = doWriteFileFromHD(ref mgdVol, ref newDestName, ref newDestPath);
                        }
                        else if (VolAccessMode == Volume.RTTD)
                        {
                            err = doWriteFile(ref mgdVol, newDestName, ref newDestPath);
                        }
					}

					for (int i=0; i<subdirList.Length; i++)
					{
						newPath = subfileList[i].ToString();		// TODO - fix for file and / or dir
						String newDestPath = destPath + "\\" + newPath;
						// CreateEntry for this directory path
						err = mgdVol.CreateEntry(newDestPath, RTFile.FileIsDir, RTFile.CreateParents|RTFile.CreateOverwrite);
						err = TraverseDirectory(ref mgdVol, ref newPath, ref newDestPath);
					}
				}
			}
			return err;
		}

		System.Int32 doRead(ref Device mgr)
		{

			System.Int32 err = 0;
			Volume mgdVol = new Volume(mgr);

			System.UInt32 fstype=0;
            Volume.InfoCallback cb = new Volume.InfoCallback(new DelegateClass().VolumeCallback);
			err = mgdVol.Open(cb, System.IntPtr.Zero);
            fstype = mgdVol.FileSystemType;


            err = mgdVol.Mount(Volume.ReadOnly, cb, System.IntPtr.Zero);
            GetVolumeInfo(ref mgdVol);
			String destPath = "/";
			UInt32 kind = mgdVol.get_EntryKind(destPath);
            if (kind == RTFile.FileIsDir)
            {
                String empty = "";
                TraverseDirectory(ref mgdVol, ref destPath, ref empty);
            }

			err = mgdVol.Close();
            mgdVol.Dispose();
			return err;

		}


		System.Int32 doVerify(ref Device mgr, ref String srcPath, ref String destPath)
		{

			System.Int32 err = 0;
			Volume mgdVol = new Volume(mgr);

			System.UInt32 fstype=0;
			// native open calls to srcpath
            Volume.InfoCallback cb = new Volume.InfoCallback(new DelegateClass().VolumeCallback);
            err = mgdVol.Open(cb, System.IntPtr.Zero);
            fstype = mgdVol.FileSystemType;


            err = mgdVol.Mount(Volume.ReadOnly, cb, System.IntPtr.Zero);

			UInt32 kind = mgdVol.get_EntryKind(destPath);
            if (kind == RTFile.FileIsFile)
                TraverseDirectory(ref mgdVol, ref destPath, ref srcPath);
            else
            {
                // add in dir traversing volume calls from /destPath
                TraverseDirectory(ref mgdVol, ref destPath, ref srcPath);
                // add in file stuff to read each file

                // add in native file stuff to compare to each file read

                // close native calls
            }

			err = mgdVol.Close();
            mgdVol.Dispose();
			return err;

		}
        Int32 doExtractVolumeToHD(ref Device dev, ref String srcPath, ref String destPath)
        {
            Int32 err = 0;

            Format mgdFormat = new Format(dev);
            Volume.InfoCallback cb = null;
            if (isCallingBack)
                cb = new Volume.InfoCallback(new DelegateClass().VolumeCallback);
            err = mgdFormat.Open(cb, System.IntPtr.Zero);
            if (err != StorageError.None)
            {
                DisplayError(err);
                return err;
            }
            err = mgdFormat.Mount(Volume.ReadOnly, cb, System.IntPtr.Zero);
            if (err != StorageError.None)
                return err;
            UInt64 numFiles = mgdFormat.get_EntrySize(srcPath);
            Audio audioCD = new Audio(mgdFormat);
            for (UInt32 i = 1; i <= (UInt32)numFiles; i++)
            {
                String thisFileName = "ExtractedTrack" + i.ToString() + ".wav";
                String destFilePath = destPath + "\\" + thisFileName;
                Audio.TrackEncoderSettings encodeSettings = new Audio.TrackEncoderSettings();
                encodeSettings.type = Audio.FileType.WAV;
                encodeSettings.structSizeBytes = (ushort)Marshal.SizeOf(encodeSettings);

                Audio.TrackTags tags = new Audio.TrackTags();

                Int32[] someIndex = new Int32[1];

                err = audioCD.ExtractAudioTrackToFile(i, destFilePath, 0, 0, -1, tags, encodeSettings, cb, System.IntPtr.Zero, someIndex);
                if (err != StorageError.None)
                    return err;
            }

            err = mgdFormat.Close();
            if (err != StorageError.None)
                return err;

            return err;
        }
        public static void DisplayError(int err)
    {
    	StorageError ourErr = new StorageError(err);
    	
	   String ErrorString="";

		if (err == StorageError.None)
			ErrorString = "None";
		else if (err == StorageError.BufferOverflow)
				ErrorString = "BufferOverflow";
		else if (err == StorageError.DataMiscompare)
				ErrorString = "DataMiscompare";
		else if (err == StorageError.DeviceError)
				ErrorString = "DeviceError";
		else if (err == StorageError.DeviceInUse)
				ErrorString = "DeviceInUse";
		else if (err == StorageError.DeviceInQuickUse)
				ErrorString = "DeviceInQuickUse";
		else if (err == StorageError.DeviceSenseError)
				ErrorString = "DeviceSenseError";
		else if (err == StorageError.DeviceNotReady)
				ErrorString = "DeviceNotReady";
		else if (err == StorageError.DeviceSelected)
				ErrorString = "DeviceSelected";
		else if (err == StorageError.Fatal)
				ErrorString = "Fatal";
		else if (err == StorageError.FileExists)
				ErrorString = "FileExists";
		else if (err == StorageError.FileNotFound)
				ErrorString = "FileNotFound";
		else if (err == StorageError.FileOpenFailed)
				ErrorString = "FileOpenFailed";
		else if (err ==  StorageError.FolderExists)
				ErrorString = "FolderExists";
		else if (err == StorageError.FolderNotFound)
				ErrorString = "FolderNotFound";
		else if (err == StorageError.FullEraseNeeded)
				ErrorString = "FullEraseNeeded";
		else if (err == StorageError.InvalidAccess)
				ErrorString = "InvalidAccess";
		else if (err == StorageError.InvalidFile)
				ErrorString = "InvalidFile";
		else if (err == StorageError.InvalidFolder)
				ErrorString = "InvalidFolder";
		else if (err == StorageError.InvalidHandle)
				ErrorString = "InvalidHandle";
		else if (err == StorageError.InvalidIndex)
				ErrorString = "InvalidIndex";
		else if (err == StorageError.InvalidLanguageCode)
				ErrorString = "InvalidLanguageCode";
		else if (err == StorageError.InvalidMediaType)
				ErrorString = "InvalidMediaType";
		else if (err == StorageError.InvalidMetadata)
				ErrorString = "InvalidMetadata";
		else if (err == StorageError.InvalidMetadataName)
				ErrorString = "InvalidMetadataName";
		else if (err == StorageError.InvalidOperation)
				ErrorString = "InvalidOperation";
		else if (err == StorageError.InvalidParameter)
				ErrorString = "InvalidParameter";
		else if (err == StorageError.InvalidProjectType)
				ErrorString = "InvalidProjectType";
		else if (err == StorageError.InvalidSequence)
				ErrorString = "InvalidSequence";
		else if (err == StorageError.InvalidStringType)
				ErrorString = "InvalidStringType";
		else if (err == StorageError.InvalidTime)
				ErrorString = "InvalidTime";
		else if (err == StorageError.InvalidTimeUnit)
				ErrorString = "InvalidTimeUnit";
		else if (err == StorageError.InvalidTrackType)
				ErrorString = "InvalidTrackType";
		else if (err == StorageError.MediaFull)
				ErrorString = "MediaFull";
		else if (err == StorageError.MetadataNotFound)
				ErrorString = "MetadataNotFound";
		else if (err == StorageError.MetadataTypeMismatch)
				ErrorString = "MetadataTypeMismatch";
		else if (err == StorageError.NotEnoughStorage)
				ErrorString = "NotEnoughStorage";
		else if (err == StorageError.NotFound)
				ErrorString = "NotFound";
		else if (err == StorageError.NotImplemented)
				ErrorString = "NotImplemented";
		else if (err == StorageError.NotInitialized)
				ErrorString = "NotInitialized";
		else if (err == StorageError.NotSupported)
				ErrorString = "NotSupported";
		else if (err == StorageError.ReadError)
				ErrorString = "ReadError";
		else if (err == StorageError.ReentrantCall)
				ErrorString = "ReentrantCall";
		else if (err == StorageError.StreamCallbackError)
				ErrorString = "StreamCallbackError";
		else if (err == StorageError.TrackCountOverflow)
				ErrorString = "TrackCountOverflow";
		else if (err == StorageError.UnsupportedVersion)
				ErrorString = "UnsupportedVersion";
		else if (err == StorageError.UserCancelled)
				ErrorString = "UserCancelled";
		else if (err == StorageError.WriteError)
				ErrorString = "WriteError";
		else
	           Console.WriteLine("Missing error text in app: {0}", err);



       Console.WriteLine("Error received: {0}, errorCode is:{1}", ErrorString, err);
    }

        void OutputCmdLineHelp()
        {


            // add more args parsing here for add'l arguments
            // current syntax:  <device> <FSType> <VolAccessMode> <src> <dst>
            // where
            //      <device> is device index as # from 1...N, or a path to a file
            //      <src>    is the source fullpath
            //      <dst>    is the destination fullpath
            //      <FSType> is a constant from the Volume fs consts
            //      <VolAccess> is consts for ReadOnly, Mastering, RTTD        
            //      <operation> is the current operation to run
            //                  list currently includes:
            //                      0 - write
            //                      1 - copy
            //                      2 - verify
            //                      3 - read
            //						4 - erase
            //      <options>
            //                  FlushBorder=1    for FlushBorderOptions.BorderOpen
            //                  FlushBorder=2    for FlushBorderOptions.BorderFinal
            //                  FlushBorder=3    for FlushBorderOptions.BorderClosed
            //
            //                  FlushVerify=1    for FlushVerify turned on
            //                  FlushVerify=0    for FlushVerify turned off
            //
            //                  EraseOption=0    for QuickErase
            //                  EraseOption=1    for FullErase


        }
        void ParseArgs(string[] args)
		{

			Console.WriteLine("Found {0} arguments in command line.  Cmd Line is:", args.Length);
            foreach (String thisArg in args)
            {
                Console.Write("{0} ",thisArg);
            }
            Console.WriteLine();
				// add more args parsing here for add'l arguments
				// current syntax:  <device> <FSType> <VolAccessMode> <src> <dst>
				// where
				//      <DestDevice> is device index as # from 1...N, or a path to a file
				//      <SrcPath>    is the source fullpath
				//      <DstPath>    is the destination fullpath
				//      <FSType> is a constant from the Volume fs consts
				//      <VolAccess> is consts for ReadOnly, Mastering, RTTD        
				//      <Operation> is the current operation to run
				//                  list currently includes:
				//                      0 - write
				//                      1 - copy
				//                      2 - verify
				//                      3 - read
			    //						4 - erase
                //                      5 - write format
                //                      6 - extract volume to hdd
                //      <Options>
                //                  FlushBorder=1    for FlushBorderOptions.BorderOpen
                //                  FlushBorder=2    for FlushBorderOptions.BorderFinal
                //                  FlushBorder=3    for FlushBorderOptions.BorderClosed
                //
                //                  FlushVerify=1    for FlushVerify turned on
                //                  FlushVerify=0    for FlushVerify turned off
                //
                //                  EraseOption=0    for QuickErase
                //                  EraseOption=1    for FullErase
                //
                //                  StreamData=1    for using Streamed source files, default is from HDD
                //                  
                //                  Format=<formatType>, where <formatType> can be any of the following:
                //                         DVD, AudioCD, HDDVD, DDP, DDP2, DDP3, BDCMF

            int numArguments = 0;
            if (args.Length > 0)
                numArguments = args.Length;
            foreach (String thisArg in args)
            {
                Console.Write("{0} ", thisArg);

                if (thisArg.StartsWith("DestDevice"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    try
                    {
                        deviceIndex = UInt32.Parse(option);
                        cmdDevicePath = option;
                    }
                    catch (SystemException e)
                    {
                        Console.WriteLine("{0}", e.ToString());

                        // must be a file device path
                        deviceIndex = 0;
                        cmdDevicePath = option;

                        // if cmdDevicePath = "", then we're assuming a null device

                    }
                }
                else if (thisArg.StartsWith("SrcPath"))
                {
                    cmdSrcPath = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                }
                else if (thisArg.StartsWith("DstPath"))
                {
                    cmdDstPath = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                }
                else if (thisArg.StartsWith("FSType"))
                {
                    FSType = UInt32.Parse(thisArg.Substring(thisArg.LastIndexOf('=') + 1));
                }
                else if (thisArg.StartsWith("VolAccessMode"))
                {
                    VolAccessMode = UInt32.Parse(thisArg.Substring(thisArg.LastIndexOf('=') + 1));
                }
                else if (thisArg.StartsWith("Operation"))
                {
                    operation = UInt32.Parse(thisArg.Substring(thisArg.LastIndexOf('=') + 1));
                }
                else if (thisArg.StartsWith("FlushBorder"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    if (addlOpt == 1)
                        VolFlushOption = Volume.FlushBorderOptions.Open;
                    else if (addlOpt == 3)
                        VolFlushOption = Volume.FlushBorderOptions.Final;
                    // otherwise continue with default (Border_Close)
                }
                else if (thisArg.StartsWith("FlushVerify"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    if (addlOpt == 1)
                        VerifyOption = 1;
                    // otherwise we'll keep default for no verify

                }
                else if (thisArg.StartsWith("EraseOption") &&
                        operation == 4)
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    if (addlOpt == 1)
                        EraseOption = Device.EraseType.Long;
                    // otherwise we'll keep default for Quick_Erase
                }
                else if (thisArg.StartsWith("MultipleTargets") &&
                    (operation == 0 || operation == 1))
                {
                    int addlLen = thisArg.Length;
                    int foundIdx = thisArg.IndexOf("=");
                    String option = "";
                    option = thisArg.Substring(foundIdx + 1);
                    //UInt32 numTargets = 0;
                    char[] targetseps = new char[3];

                    targetseps[0] = ' ';
                    targetseps[1] = ';';
                    targetseps[2] = ',';
                    destDevicePaths = option.Split(targetseps, 64);
                }
                else if (thisArg.StartsWith("StreamData") && operation == 0)
                {
                    int lastIdx = thisArg.LastIndexOf('=');
                    int len = thisArg.Length;
                    String option = thisArg.Substring(lastIdx + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    if (addlOpt == 1)
                        isStreaming = true;
                    else
                        isStreaming = false;
                    // otherwise we'll keep default for reading from HDD and not 
                    // use callback for streaming data from client

                }
                else if (thisArg.StartsWith("Format") &&
                   operation == 5)
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    VolFormatType = (Volume.FormatType)addlOpt;

                }
                else if (thisArg.StartsWith("LogPath"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    sdk.SetSDKObjectProperty(SDKProperties.SDKProperty_Type.LogPath, option);
                }
                else if (thisArg.StartsWith("DriverType"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    sdk.SetSDKObjectProperty(SDKProperties.SDKProperty_Type.Driver, addlOpt);
                }
                else if (thisArg.StartsWith("Callbacks"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    if (addlOpt == (UInt32)0)
                        isCallingBack = false;
                    else
                        isCallingBack = true;
                }
                else if (thisArg.StartsWith("FileFormatType"))
                {
                    String option = thisArg.Substring(thisArg.LastIndexOf('=') + 1);
                    UInt32 addlOpt = UInt32.Parse(option);
                    if (addlOpt == (UInt32)6)
                        fileTypeForFileDevice = Device.FormatTypeGI;
                }
                    
            }
        }

/*
				cmdDevicePath = args[0];

		        if (cmdDevicePath == "1" || cmdDevicePath == "2" || cmdDevicePath == "3")
					deviceIndex = UInt32.Parse(cmdDevicePath);
				else
					deviceIndex = 0;
				if (args.Length > 2)
				{
					cmdSrcPath = args[1];
					cmdDstPath = args[2];
				}
				if (args.Length > 3)
					FSType = UInt32.Parse(args[3]);
				if (args.Length > 4)
					VolAccessMode = UInt32.Parse(args[4]);
				if (args.Length > 5)            
					operation = UInt32.Parse(args[5]);
                if (args.Length > 6)
                {
                    String addlOptions = args[6];
                    char[] seps = new char[3];

                    seps[0] = ';';
                    seps[1] = ',';
                    String [] addlOptionsList = addlOptions.Split(seps, 64, StringSplitOptions.RemoveEmptyEntries);

                    foreach (String addlOptionName in addlOptionsList)
                    {

                        if (addlOptionName.StartsWith("FlushBorder"))
                        {
                            String option = addlOptionName.Substring(addlOptionName.LastIndexOf('=') + 1);
                            UInt32 addlOpt = UInt32.Parse(option);
                            if (addlOpt == 1)
                                VolFlushOption = Volume.FlushBorderOptions.Open;
                            else if (addlOpt == 3)
                                VolFlushOption = Volume.FlushBorderOptions.Final;
                            // otherwise continue with default (Border_Close)
                        }
                        else if (addlOptionName.StartsWith("FlushVerify"))
                        {
                            String option = addlOptionName.Substring(addlOptionName.LastIndexOf('=') + 1);
                            UInt32 addlOpt = UInt32.Parse(option);
                            if (addlOpt == 1)
                                VerifyOption = 1;
                            // otherwise we'll keep default for no verify

                        }
                        else if (addlOptionName.StartsWith("EraseOption") &&
                                operation == 4)
                        {
                            String option = addlOptionName.Substring(addlOptions.LastIndexOf('=') + 1);
                            UInt32 addlOpt = UInt32.Parse(option);
                            if (addlOpt == 1)
                                EraseOption = Device.EraseType.Long;
                            // otherwise we'll keep default for Quick_Erase
                        }
                        else if (addlOptionName.StartsWith("MultipleTargets") &&
                            (operation == 0 || operation == 1))
                        {
                            int addlLen = addlOptionName.Length;
                            int foundIdx = addlOptionName.IndexOf("=");
                            String option = "";
                            option = addlOptionName.Substring(foundIdx + 1);
                            UInt32 numTargets = 0;
                            char[] targetseps = new char[3];

                            targetseps[0] = ' ';
                            targetseps[1] = ';';
                            targetseps[2] = ',';
                            destDevicePaths = option.Split(targetseps, 64);
                        }
                        else if (addlOptionName.StartsWith("StreamData") && operation == 0)
                        {
                            int lastIdx = addlOptionName.LastIndexOf('=');
                            int len = addlOptionName.Length;
                            String option = addlOptionName.Substring(lastIdx + 1);
                            UInt32 addlOpt = UInt32.Parse(option);
                            if (addlOpt == 1)
                                isStreaming = true;
                            else
                                isStreaming = false;
                            // otherwise we'll keep default for reading from HDD and not 
                            // use callback for streaming data from client

                        }
                        else if (addlOptionName.StartsWith("Format") &&
                           operation == 5)
                        {
                            String option = addlOptionName.Substring(addlOptionName.LastIndexOf('=') + 1);
                            UInt32 addlOpt = UInt32.Parse(option);
                            VolFormatType = (Volume.FormatType)addlOpt;

                            // otherwise we'll keep default for Quick_Erase
                        }
            //        }
           //     }  


		}
*/
        static void Main(string[] args)
        {
            Program app = new Program();

	        if (args.Length > 1)
		        app.ParseArgs(args);
	        else
	        {
                app.OutputCmdLineHelp();
		        app.cmdDevicePath = "";
		        app.deviceIndex = 1;
	        }

            // don't send in the OS index as device string if its a number
            //if (app.cmdDevicePath != "" && app.deviceIndex == UInt32.Parse(app.cmdDevicePath))
            //    app.cmdDevicePath = "";

           // if (app.deviceIndex != 0)
           // 	 app.cmdDevicePath = null;
            app.GetSDKProps();
	        Device mgr = new Device();
            System.UInt32 flags = Device.ScanAll;
            UInt32 count = mgr.get_DeviceCount(flags);
            app.GetSDKProps();
            mgr.Open(app.deviceIndex, app.cmdDevicePath);  // source for read/verify/copy 
                                                                          // dest for mastering
            String currUsedApp = "";
            mgr.ExclusiveAccess(Device.ExclusiveAccessType.Obtain, "CSTestApp", out currUsedApp);

            mgr.ExclusiveAccess(Device.ExclusiveAccessType.Query, "", out currUsedApp);
            Int32 err = 0;
	        err = app.GetDeviceInfo(ref mgr);
            err = app.GetMediaInfo(ref mgr);

	        if (app.operation == 0)
	        {
		        // do master volume
		        err = app.doWriteVolume(ref mgr, ref app.cmdSrcPath, ref app.cmdDstPath);
	        }
	        else if (app.operation == 1)
	        {
		        err = app.doCopy(ref mgr, ref app.cmdDstPath);
	        }
	        else if (app.operation == 2)
	        {
		        err = app.doVerify(ref mgr, ref app.cmdSrcPath, ref app.cmdDstPath);
	        }
	        else if (app.operation == 3)
	        {
		        err = app.doRead(ref mgr);
	        }
            else if (app.operation == 4)
            {
                Device.Callback cb = null;
                if (app.isCallingBack)
                    cb = new Device.Callback(new DelegateClass().DeviceCallback);
                err = mgr.EraseMedia(app.EraseOption, cb, System.IntPtr.Zero);
                err = app.GetMediaInfo(ref mgr);
            }
            else if (app.operation == 5)
            {
                app.doWriteFormat(ref mgr, ref app.cmdSrcPath);
            }
            else if (app.operation == 6)
            {
                app.doExtractVolumeToHD(ref mgr, ref app.cmdSrcPath, ref app.cmdDstPath);
            }

            mgr.ExclusiveAccess(Device.ExclusiveAccessType.Release, "CSTestApp", out currUsedApp);
            mgr.Close();

            mgr.Dispose();
            

        }
    }


}
