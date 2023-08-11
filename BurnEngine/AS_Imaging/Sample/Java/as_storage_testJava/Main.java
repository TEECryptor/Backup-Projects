/*
 * Main.java
 *
 * Created on June 28, 2006, 3:20 PM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package com.sonic.as_storage_testJava;


import java.io.IOException;
import java.io.File;
import java.io.FileDescriptor;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.FileInputStream;
//import java.io.FileOutputStream;
import java.nio.channels.FileChannel;
import java.nio.ByteBuffer;
import java.util.logging.*;
import java.util.Vector;
import java.util.Date;
import java.util.Calendar;
import java.lang.Exception;
import com.sonic.as_storage_java.*;
import java.lang.System;



/**
 * This class tests basic read/write/copy/verify functions of as_storage_java API.
 *
 * @author vijaya_lashkari
 */
public class Main {


  private static String GetProgressStatusString(int status)
    {
        switch(status)
        {
            case Device.JobStatus_Aborted:
                   return "Aborted";
            case Device.JobStatus_BusyAborting:
                   return "Busy Aborting";
            case Device.JobStatus_Error:
                   return "Error";
            case Device.JobStatus_Idle:
                   return "Idle";
            case Device.JobStatus_PartialSuccess:
                   return "Partial Success";
            case Device.JobStatus_Running:
                   return "Running";
            case Device.JobStatus_Success:
                   return "Success";
            default:
                return "None";
        }
    }
    
    private static String GetProgressOpString(int operation)
    {
        switch(operation)
        {
            case Device.JobType_Aborting:
                return "Aborting";
            case Device.JobType_Comparing:
                return "Comparing";
            case Device.JobType_Erasing:
                return "Erasing";
            case Device.JobType_Finalizing:
                return "Finalizing";
            case Device.JobType_Initializing:
                return "Initializing";
            case Device.JobType_Reading:
                return "Reading";
            case Device.JobType_Seek:
                return "Seeking";
            case Device.JobType_WriteFilemark:
                return "Write mark";
            case Device.JobType_Writing:
                return "Writing";
            default:
                return "None";
        }
    }	
	public class DeviceEventListenerTest implements AS_Storage_EventListener.DeviceEventListener
	{
		public void deviceEventProcessed(AS_Storage_JavaEvent ev)
		{
			if (((AS_Storage_JavaEvent)ev).EventType == AS_Storage_JavaEvent.JavaEventType.DevicePnPCallbackEvent) // Pnp EVent
				System.out.println("\t\t received event from device" + ((AS_Storage_JavaEvent)ev).EventInfo);
			else if (((AS_Storage_JavaEvent)ev).EventType == AS_Storage_JavaEvent.JavaEventType.DeviceBriefProgressEvent) // Progress notify event
				System.out.println("Pct complete is: " + ((AS_Storage_JavaEvent)ev).EventInfo);	
			else if (((AS_Storage_JavaEvent)ev).EventType == AS_Storage_JavaEvent.JavaEventType.DeviceDetailedProgressEvent) // Detailed Progress notify event
			{
				String status = GetProgressStatusString(ev.ProgressStatus);
				String op = GetProgressOpString(ev.CurrentOperation);
		        System.out.printf("Handle is: %d\n", ((AS_Storage_JavaEvent)ev).DeviceInfo);
		        System.out.printf("Status is: %s\n", status);
		        System.out.printf("Operation is: %s\n", op);
		        System.out.printf("currentSector is: %d\n", ev.CurrentSector);
		        System.out.printf("totalSectors is: %d\n", ev.ProgressStatus);
			}
		}
	}
	
    public class FileStreamListing
    {
        Vector<FileInputStream> readers = new Vector<FileInputStream>();
        Vector<FileDescriptor>  fileDesList = new Vector<FileDescriptor>();  
        public int AddStream(FileInputStream rdr)
        {
            readers.add(rdr);
        	try
        	{            
            FileChannel strm = getFileChannel(rdr.getFD());
            if (strm != null)
            {
            	// if there is a future way to suppress finalize on this strm, we can inhibit GC
            }
        	}
        	catch (Exception e)
        	{
        		e.printStackTrace();
        	}
        	
        	return readers.size();

        }
        public int AddFileDesc(FileDescriptor fd)
        {
        	fileDesList.add(fd);
        	return fileDesList.size();
        }
        public FileChannel getFileChannelFromIndex(int index)
        {
        	return readers.elementAt(index).getChannel();
        }
        public FileChannel getFileChannel(FileDescriptor fd)
        {
        	try
        	{
	           for (FileInputStream rdr : readers)
	           {
	        	   if (rdr.getFD() == fd)
	        		   return rdr.getChannel();
	           }
        	}
        	catch (Exception e)
        	{
        		e.printStackTrace();
        	}
        	
           return null;
        	
        }
        public FileInputStream getFileInputStream(FileDescriptor fd)
        {
        	try
        	{
	           for (FileInputStream rdr : readers)
	           {
	        	   if (rdr.getFD() == fd)
	        		   return rdr;
	           }
        	}
        	catch (Exception e)
        	{
        		e.printStackTrace();
        	}
        	
           return null;
        }
        protected void RemoveStream(FileDescriptor fd)
        {
        	try
        	{
	           for (FileInputStream rdr : readers)
	           {
	        	   if (rdr.getFD() == fd)
	        	   {
	        		   FileChannel strm = getFileChannel(rdr.getFD());
	        		   strm.close();
	        		   rdr.close();
	        		   readers.remove(rdr);
	        		   fileDesList.remove(rdr.getFD());
	        		   return;
	        	   }
	           }
        	}
        	catch (Exception e)
        	{
        		e.printStackTrace();
        	}
        	
        }
        protected void finalize()
        {
        	try
        	{
	            for (FileInputStream rdr : readers)
	            {
	            	RemoveStream(rdr.getFD());
	            }
	        }
        	catch (IOException e)
        	{
        		e.printStackTrace();
        	}

        }
    }	
	public class VolumeEventListenerTest implements AS_Storage_EventListener.VolumeEventListener
	{
		byte[] VolumeRetrievalBuffer = new byte[4096];
		public int volumeEventProcessed(AS_Storage_JavaEvent ev, int handleInfo, int currentSector, int totalSectors, int progressStatus, int currentOperation)
		{
			int doCancel = 0;
			
			if (((AS_Storage_JavaEvent)ev).EventType == AS_Storage_JavaEvent.JavaEventType.VolumeBriefProgressEvent) // Brief progress event
				System.out.println("Pct complete is: " + ((AS_Storage_JavaEvent)ev).EventInfo);	
			else if (((AS_Storage_JavaEvent)ev).EventType == AS_Storage_JavaEvent.JavaEventType.VolumeDetailedProgressEvent) // Detailed progress event
			{
				System.out.println("Pct complete is: " + ((AS_Storage_JavaEvent)ev).EventInfo);	
				String status = GetProgressStatusString(progressStatus);
				String op = GetVolProgressOpString(currentOperation);
			    System.out.printf("Operation is: %s\n", op);
			    System.out.printf("currentSector is: %d\n", (currentSector));
			    System.out.printf("totalSectors is: %d\n", (totalSectors));
			    System.out.printf("Status is %s\n", status);
			    System.out.printf("Device Info is %d\n", (handleInfo));
			}
			
			return doCancel;
		}
		
		public byte[] volumeEventRetrieveData(AS_Storage_JavaEvent event, long pos, int bytesRequested, Object userData)
		{
			byte[] retVal = null;
			ByteBuffer buf=null;
			try
			{
				if (bytesRequested != 4096)
					retVal = new byte[bytesRequested];
				else
					retVal = VolumeRetrievalBuffer;
				buf = ByteBuffer.wrap(retVal);
				if (userData != null)
				{
					//myList.getFileChannelFromIndex((Integer)userData);
					FileDescriptor fd = (FileDescriptor)userData;
					if (fd.valid())
					{
						FileChannel channel = myList.getFileChannel(fd);
						int numread = channel.read(buf, pos);
						if (numread != bytesRequested)
							System.out.printf("Failed to read requested amount of bytes from pos: %d\n", pos);
					//	else
					//		System.out.printf("Read %d bytes from file handle %d at pos %d\n", numread, event.EventInfo, pos);
					}
					else
						System.out.println("failed to get valid file descriptor from userData");
				}
				else
				{
					// other than sending in FD to userdata, we can keep a list of descriptors and use them
					FileDescriptor fd = myList.fileDesList.get(0);
					if (fd.valid())
					{
						FileChannel channel = myList.getFileChannel(fd);
						int numread = 0;
						if (channel != null)	
							numread = channel.read(buf, pos);
						else
							System.out.println("failed to get valid file stream from file descriptor");
						if (numread != bytesRequested)
							System.out.printf("Failed to read requested amount of bytes from pos: %d\n", pos);
						else
							System.out.printf("Read %d bytes from file at pos %d\n", numread, pos);
					}
					else
						System.out.println("failed to get valid file descriptor from global list");
					
				
				}
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}

			if (buf.hasArray())
				return buf.array();
			else
			{
				System.out.println("failed to get wrapped buffer - lets waste time copying buffer here");
				buf.get(retVal, 0, retVal.length);

			}
			return retVal;
							
				
		}
	};	

    private static String GetVolProgressOpString(int operation)
    {
        switch(operation)
        {
            case Volume.ProgressType_General:
                return "General";
            case Volume.ProgressType_Format:
                return "Formatting";
            case Volume.ProgressType_Mount:
                return "Mounting";
            case Volume.ProgressType_Plan:
                return "Planning";
            case Volume.ProgressType_Write:
                return "Writing";
            case Volume.ProgressType_Close:
                return "Closing";
            case Volume.ProgressType_Verify:
                return "Verifying";
            default:
                return "None";
        }
    }
    static
    {
        System.out.println("TEMP : " + System.getProperty("java.io.tmpdir"));
	    System.out.println("PATH : " + System.getProperty("java.library.path"));
	    System.out.println("CLASSPATH : " + System.getProperty("java.class.path"));
	    System.out.println("CURRENT DIR: " + System.getProperty("user.dir"));

        System.loadLibrary("AS_Storage_JNI_w32");
    }
    public static String devicePath;
    public static String srcPath;
    public static String dstPath;
    public static int FSType;
    public static int VolAccessMode;
    public static int operation;
    public static boolean isStreaming;
    public static Volume.Flush_Border_Options VolFlushOption;
    public static Device.Erase_Type EraseOption;
    public static int VerifyOption;
    private static FileStreamListing myList;
    public static String[] destDevicePaths;
    public static Device[] destDeviceList;
    public static long VolumeFreeSpace;
	public static  Volume.Format_Type VolFormatType;
	public static SDKProperties sdkProp;
	public static boolean isCallingBack;
	public int AudioNumTracks = 0;
	public Audio.EffectType AudioEffectType = Audio.EffectType.Effect_FadeIn;
	//public System.Diagnostics.TextWriterTraceListener traceListener;
    public FileHandler traceStream;
    public Logger logger;
    /** Creates a new instance of Main */
    public Main() {
        srcPath="";
        dstPath="";
        devicePath="1";   // default to 1st optical device
        FSType = (1<<11); // default to UDF 1.5
        VolAccessMode = Volume.Mastering; //Volume.RTTD;
        VolFlushOption = Volume.Flush_Border_Options.Border_Close;
        operation = 0;    // default to write src to dst
        EraseOption = Device.Erase_Type.Quick_Erase;
        VerifyOption = 0;
        myList = new FileStreamListing();
        destDevicePaths = null;
        destDeviceList = null;
        isStreaming = false;
        VolumeFreeSpace = 0;
        isCallingBack = true;

    }

    public static void getVolumeInfo(Volume vol)
    {
    	Volume.Property[] availTypes = Volume.Property.values();
    	for (Volume.Property ourType : availTypes)
    	{
    		System.out.println(ourType);
    		Object propVal = vol.GetVolumeObjectProperty(ourType);
    		if (ourType == Volume.Property.Prop_DateCreated ||
    				ourType == Volume.Property.Prop_DateModified)
    		{
    			System.out.println(GetVolumeDateTimeString((Volume.DateTime)propVal));
    		}
    		else if (ourType == Volume.Property.Prop_Format_Type)
    		{
    			System.out.println(GetVolumeFormatTypeString((Integer)propVal));
    		}
    		else if (ourType == Volume.Property.Prop_FS_Type)
    		{
    			System.out.println(GetVolumeFSTypeString((Integer)propVal));
    		}
    		else if (ourType == Volume.Property.Prop_Delimiter)
    		{
    			System.out.println((char)((Byte)propVal).intValue());
    		}
         	else
         		System.out.println(propVal);
    	}
    }
    public static void getDeviceInfo(Device device)
    {
//    	 this code will provide all the possible enum values for props
        Device.DevProp_Type[] availTypes = Device.DevProp_Type.values(); 
        for (Device.DevProp_Type ourType : availTypes)
        {
         	System.out.println(ourType);
         	Object devicePropVal = device.GetDeviceObjectProperty(ourType);
         	if (devicePropVal != null)
         	{
	         	if (ourType == Device.DevProp_Type.DevProp_TrayStatus)
	         		System.out.println(GetDeviceTrayStatusString((Integer)devicePropVal));
	         	else if (ourType == Device.DevProp_Type.DevProp_AuthenTypes)
	         		System.out.println(GetDeviceAuthString((Integer)devicePropVal));
	         	else if (ourType == Device.DevProp_Type.DevProp_CopyProtection)
	         		System.out.println(GetDeviceCopyProtString((Integer)devicePropVal));
	         	else if (ourType == Device.DevProp_Type.DevProp_DevType)
	         		System.out.println(GetDeviceTypeString((Integer)devicePropVal));
	         	else if (ourType == Device.DevProp_Type.DevProp_MedTypesCapable)
	         		System.out.println(GetDeviceMedCapableString((Long)devicePropVal));
	         	else if (ourType == Device.DevProp_Type.DevProp_Bus_Info)
	         	{
					Device.BusInfo inf = device.new BusInfo();
					inf = (Device.BusInfo)devicePropVal;
					System.out.printf("BusKind: %s, Adapter: %d, Target: %d, Lun: %d\n", GetDeviceBusKind(inf.busKind), inf.adapterID, inf.targetID, inf.lunID);
	         	}
	         	else
	         		System.out.println(devicePropVal);
         	}	
         		
        }
    }   
    public static void getDeviceState(Device device)
    {
    	Device.DeviceState readyState = device.GetDeviceState();
        if (readyState != Device.DeviceState.State_Ready)
            System.out.println("Device not ready");
        else
            System.out.println("Device is ready");       
    }
    
    public static boolean IsNum(String str)
    {
        String digits = "0123456789x-";
        boolean isNum = false;
        for (int i=0;i<str.length(); i++)
        {
            char c = str.charAt(i);
            if (digits.indexOf(c) != -1)
                isNum = true;
        }        
        return isNum;
    }

    private static String GetDeviceBusKind(int busKind)
    {
    	String retVal = "";
    	switch(busKind)
    	{
    	case Device.ATAPI:
    		retVal = "ATAPI";
    		break;
    	case Device.Bus_1394:
    		retVal = "1394";
    		break;    		
    	case Device.Bus_SCSI:
    		retVal = "SCSI";
    		break;    		
    	case Device.Bus_USB:
    		retVal = "USB";
    		break;    		
    	case Device.Bus_USB2:
    		retVal = "USB2";
    		break;    		
    	case Device.Bus_Unknown:
    	default:
    		retVal = "Unknown";
		break;    	
    	};
    	return retVal;
    }
    public static int getTrackInfo(Device device, int trackNum)
    {
        int err = 0;
		System.out.println("Getting track info for Track #" + trackNum + ":");
//      this code will provide all the possible enum values for props
        Device.TrackProp_Type[] availTypes = Device.TrackProp_Type.values(); 
        for (Device.TrackProp_Type ourType : availTypes)
        {
        	System.out.println(ourType);
        	Object trackPropVal = device.GetTrackObjectProperty(ourType, trackNum);
        	if (trackPropVal != null)
        	{
        		System.out.println(trackPropVal);
        	}
        }
        
        return err;
    }
    public static int getMediaInfo(Device device)
    {       
        int err = 0;
        // this code will provide all the possible enum values for props
        Device.MediaProp_Type[] availTypes = Device.MediaProp_Type.values(); 
        for (Device.MediaProp_Type ourType : availTypes)
        {
        	System.out.println(ourType);
        	Object mediaPropVal = device.GetMediaObjectProperty(ourType);
        	if (mediaPropVal != null)
        	{
	        	if (ourType == Device.MediaProp_Type.MedProp_State)
	        		System.out.println(GetMediaStateString((Integer)mediaPropVal));
	        	else if (ourType == Device.MediaProp_Type.MedProp_CopyProtection)
	        		System.out.println(GetDeviceCopyProtString((Integer)mediaPropVal));
	        	else if (ourType == Device.MediaProp_Type.MedProp_Kind)
	        		System.out.println(GetMediaTypeString((Long)mediaPropVal));
	        	else if (ourType == Device.MediaProp_Type.MedProp_Manufacturer)
	        	{
	        		// lets print out bytes of a byteArray here
	                Byte[] manufacturer = new Byte[48];
	                manufacturer = (Byte[])mediaPropVal;
	                System.out.printf("Manufacturer is:\n");
	                for (byte bit : manufacturer)
	                {
	                	if (bit == 0)
	                		break;
	                	System.out.printf("%c", bit);
	                }
	                System.out.println();
	        		
	        	}
	        	else if ((ourType == Device.MediaProp_Type.MedProp_WriteList_DataRates) ||
	        			(ourType == Device.MediaProp_Type.MedProp_WriteList_Hundredth_X)) 
	        		
	        	{
	        		Integer[] writeList = new Integer[128];
		            writeList = (Integer[])mediaPropVal;
		            for (int rate : writeList)
		            {
		            	if (rate == 0)
		            		break;
		            	System.out.printf("Write Rate: %d\n", rate);
		            }
		            System.out.println();
	        	}
	        	else if ((ourType == Device.MediaProp_Type.MedProp_Num_Tracks))
	        	{
	        		System.out.println(mediaPropVal);
	        		int numTrks = ((Integer)mediaPropVal).intValue();
	        		for (int i=1;i<=numTrks;i++)
	        			getTrackInfo(device, i);
	        	}
	        	else
	        		System.out.println(mediaPropVal);
	       	}
        		
        }
        
        return err;
    }
    public static int doWriteVolume(Device dev, VolumeEventListenerTest volCB, Main m)
    {
        Volume vol = new Volume();

        int err = vol.OpenVolume(dev, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        int filesystemtype = vol.GetVolumeIntProperty(Volume.Property.Prop_FS_Type);
        if (filesystemtype == 0)
        {
            err = vol.CreateVolume(FSType, "JavaJNI Test App", VolAccessMode, volCB, null);
            if (err != 0)
            {
                DisplayError(err);
                vol.CloseVolume();
                return err;
            }
        }
        else
        {
            err = vol.MountVolume(VolAccessMode, volCB, null);
            if (err != 0)
            {
                DisplayError(err);
                vol.CloseVolume();
                return err;
            }

        }
    	// set our global freespace so we can keep track of volume size
    	VolumeFreeSpace = vol.GetVolumeLongProperty(Volume.Property.Prop_FreeBlocks) * vol.GetVolumeLongProperty(Volume.Property.Prop_BlockSize);
    	System.out.printf("Volume freespace is %d\n", VolumeFreeSpace);
        switch (VolAccessMode)
        {
            case Volume.Mastering:
            {
                if (srcPath == "" || dstPath == "")
                {
                    try
                    {
                            System.out.println("Enter source path to copy:");
                            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
                            srcPath = in.readLine();

                            System.out.println("Enter dest path to copy to (could be dest drive index):");
                            dstPath = in.readLine();

                    }
                    catch(IOException e)
                    {
                            System.out.println("Failed to read input string");
                    }                        	

                }
                else
                {
                    File f = new File(srcPath);
                    if (!f.exists())
                    {
                        // if srcPath doesn't exist - assume we want to create a new path on dest volume
                        int EntryType = RTFile.File_IsFile;
                        int CreateFlags = RTFile.Create_Parents | RTFile.Create_Overwrite;
                        err = vol.CreateEntry(srcPath, EntryType, CreateFlags);
                        if (err != 0)
                        {
                            DisplayError(err);
                            vol.CloseVolume();
                            return err;
                        }
                    }
                    else
                    {
                   		err = visitAllDirsAndFiles(vol, f, m);
                   		if (err != 0)
                   		{
                   			DisplayError(err);
                   			vol.CloseVolume();
                   			return err;
                   			
                   		}
                    }
                }
            }
            break;
            case Volume.RTTD:
            {
            	err = visitAllDirsAndFiles(vol, new File(srcPath), m);
           		if (err != 0)
           		{
           			DisplayError(err);
           			vol.CloseVolume();
           			return err;
           			
           		}            	
            }
            break;
        };

        if (destDevicePaths == null)
        {
        	destDevicePaths = new String[1];
        	destDevicePaths[0] = devicePath;
        }
        else
        {
        	destDeviceList = new Device[destDevicePaths.length];
        	for (int i=0; i< destDevicePaths.length; ++i)
        	{
        		destDeviceList[i] = new Device();
        		destDeviceList[i].OpenDevice(Integer.parseInt(destDevicePaths[i]), "");
        	}
        	
        	if (destDeviceList != null)
        	{
        		err = vol.SetDevices(0, destDeviceList.length, destDeviceList, volCB, null);
           		if (err != 0)
           		{
           			DisplayError(err);
           			vol.CloseVolume();
           			return err;
           			
           		}
        	}
        	
        }

        err = vol.PrepareFlushVolume(VolFlushOption, volCB, null, VerifyOption);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        
        err =  vol.FlushVolume(VolFlushOption, volCB, null, VerifyOption);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        
        if (destDeviceList != null)
        {
	        for (int i=0; i<destDeviceList.length;++i)
	        {
	        	destDeviceList[i].CloseDevice();
	        }
        }
        
        err = vol.CloseVolume();
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }

        return err;
    }
    
	public static int doWriteAudioFileFromStream(Format mgdFormat, Audio mgdAudio, String srcPath, Audio.FileType fileType, Main m)
    {
        File srcfileTest = new File(srcPath);
        if (!srcfileTest.exists())
            return -1;
        long streamLen = (long)srcfileTest.length();
		int err = 0;
		try
		{
			FileInputStream Fstream = new FileInputStream(srcPath);
			//BinaryReader rdr = new BinaryReader(Fstream);
			myList.AddStream(Fstream);

			//SafeFileHandle handle = Fstream.SafeFileHandle;

			//int ptr=System.IntPtr.Zero;// = Fstream.SafeFileHandle.DangerousGetHandle();
			int trk = 0;
			VolumeEventListenerTest streamCB = m.new VolumeEventListenerTest();
			err = mgdAudio.Format_AddAudioTrackFromStream(streamLen, streamCB, (Object)Fstream.getFD(), fileType);
			if (err != 0)
            {
                DisplayError(err);
            }
		}
		catch(Exception e)
		{

		}

        return err;
    }
    public static int doCopy(Device dev, DeviceEventListenerTest devCB)
    {
        if (dstPath == "")
        {
            System.out.println("Didn't find source or dest path for copy, exiting!");
            return -1;
        }
        // copy operation takes device param as dest (file or device), srcPath param as source (file or device)

//              System.out.printf("src device handle is %d\n" ,dev.GetHandle());


        int destDeviceIndex = 0;
//                System.out.printf("dest device handle is %d\n", destDevice.GetHandle());
        if (IsNum(dstPath))
        {
            destDeviceIndex = Integer.parseInt(dstPath);
            if (destDeviceIndex == 0)
            	dstPath = "";
            else
            	dstPath = null;
        }

        int arrLength = 1;
        if (destDevicePaths != null)
        	arrLength = destDevicePaths.length;
        
        destDeviceList = new Device[arrLength];
        if (destDevicePaths == null)
        {
        	destDevicePaths = new String[1];
        	destDeviceList[0] = new Device();
        	destDevicePaths[0] = dstPath;
        	int err = destDeviceList[0].OpenDevice(destDeviceIndex, dstPath);
            if (err != 0)
            {
                DisplayError(err);
                return err;
            }
            getDeviceInfo(dev);
            getDeviceInfo(destDeviceList[0]);            
        }
        else
        {
        	for (int i=0; i< destDevicePaths.length; ++i)
        	{
        		destDeviceList[i] = new Device();
        		destDeviceList[i].OpenDevice(Integer.parseInt(destDevicePaths[i]), "");
                getDeviceInfo(dev);
                getDeviceInfo(destDeviceList[i]);
        	}
        }
       // System.out.printf("src device handle is %d\n", dev.GetHandle());
       // for (Device hdlDev : destDeviceList)
       // 	System.out.printf("dest device handle is %d\n", hdlDev.GetHandle());


        int format = Device.FormatType_GI;
        String drivePath =(String)(dev.GetDeviceObjectProperty(Device.DevProp_Type.DevProp_DrivePath)); 
        if (drivePath.endsWith(".gi"))
        	format = Device.FormatType_GI;
        else
        	format = Device.FormatType_OpticalDetect;

        int flags = Device.CopyFlags_Write;
        int numDests = destDeviceList.length;

        Object userData = null;
        int err = dev.Copy(format, flags, 0, numDests, destDeviceList, devCB, userData);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        for (Device destHdl : destDeviceList)
        {	
	        err = destHdl.CloseDevice();
	        if (err != 0)
	        {
	            DisplayError(err);
	            return err;
	        }
    	}
        return err;

    }
    public static int doVerify(Device dev, DeviceEventListenerTest devCB)
    {
        // verify
        boolean verifyVolume = false;
        File srcFile = new File(srcPath);
        if (srcFile.isDirectory())
            verifyVolume = true;

        int err = 0;
        if (verifyVolume)
        {

            // verify with Volume apis
        }
        else
        {
            // verify with device copy api

            int destDeviceIndex = 0;
//                System.out.printf("dest device handle is %d\n", destDevice.GetHandle());
            if (IsNum(dstPath))
            {
                destDeviceIndex = Integer.parseInt(dstPath);
                dstPath = "";
            }
            int arrLength = 1;
            if (destDevicePaths != null)
            	arrLength = destDevicePaths.length;
            
            destDeviceList = new Device[arrLength];
            if (destDevicePaths == null)
            {
            	destDevicePaths = new String[1];
            	destDeviceList[0] = new Device();
            	destDevicePaths[0] = dstPath;
            	err = destDeviceList[0].OpenDevice(destDeviceIndex, dstPath);
                if (err != 0)
                {
                    DisplayError(err);
                    return err;
                }
                getDeviceInfo(dev);
                getDeviceInfo(destDeviceList[0]);            
            }
            else
            {
            	for (int i=0; i< destDevicePaths.length; ++i)
            	{
            		destDeviceList[i] = new Device();
            		destDeviceList[i].OpenDevice(Integer.parseInt(destDevicePaths[i]), "");
                    getDeviceInfo(dev);
                    getDeviceInfo(destDeviceList[0]);
            	}
            }
            int format = Device.FormatType_OpticalDetect;
            int flags = Device.CopyFlags_Compare;
            Object userData = null;
            int numDests = destDeviceList.length;
            err = dev.Copy(format, flags, 0, numDests, destDeviceList, devCB, userData);
            if (err != 0)
            {
                DisplayError(err);
                return err;
            }
            for (Device devHdl : destDeviceList)
            {
	            err = devHdl.CloseDevice();
	            if (err != 0)
	            {
	                DisplayError(err);
	                return err;
	            }
            }

        }

           return err;
    }
    public static int doErase(Device device, DeviceEventListenerTest devCB)
    {   	
        Object userData = null;    	
        int err = device.EraseMedia(EraseOption, devCB, userData);
        if (err != 0)
        {
            DisplayError(err);
        }
        return err;
        
    }
    public static void DisplayError(int err)
    {
    	StorageError ourErr = new StorageError(err);
    	
	   String ErrorString="";
	   switch (err)
	   {
	      case StorageError.None:
	         ErrorString = "None";
	         break;
	      case StorageError.BufferOverflow:
	         ErrorString = "BufferOverflow";
	         break;
	      case StorageError.DataMiscompare:
	         ErrorString = "DataMiscompare";
	         break;
	      case StorageError.DeviceError:
	         ErrorString = "DeviceError";
	         break;
		   case StorageError.DeviceInUse:
	 		   ErrorString = "DeviceInUse";
	 		   break;
	      case StorageError.DeviceInQuickUse:
	         ErrorString = "DeviceInQuickUse";
	         break;
	      case StorageError.DeviceSenseError:
	         ErrorString = "DeviceSenseError";
	         break;
	      case StorageError.DeviceNotReady:
	         ErrorString = "DeviceNotReady";
	         break;
	      case StorageError.DeviceSelected:
	         ErrorString = "DeviceSelected";
	         break;
	      case StorageError.Fatal:
	         ErrorString = "Fatal";
	         break;
	      case StorageError.FileExists:
	         ErrorString = "FileExists";
	         break;
	      case StorageError.FileNotFound:
	         ErrorString = "FileNotFound";
	         break;
	      case StorageError.FileOpenFailed:
	         ErrorString = "FileOpenFailed";
	         break;
	      case StorageError.FolderExists:
	         ErrorString = "FolderExists";
	         break;
	      case StorageError.FolderNotFound:
	         ErrorString = "FolderNotFound";
	         break;
	      case StorageError.FullEraseNeeded:
	         ErrorString = "FullEraseNeeded";
	         break;
	      case StorageError.InvalidAccess:
	         ErrorString = "InvalidAccess";
	         break;
	      case StorageError.InvalidFile:
	         ErrorString = "InvalidFile";
	         break;
	      case StorageError.InvalidFolder:
	         ErrorString = "InvalidFolder";
	         break;
	      case StorageError.InvalidHandle:
	         ErrorString = "InvalidHandle";
	         break;
	      case StorageError.InvalidIndex:
	         ErrorString = "InvalidIndex";
	         break;
	      case StorageError.InvalidLanguageCode:
	         ErrorString = "InvalidLanguageCode";
	         break;
	      case StorageError.InvalidMediaType:
	         ErrorString = "InvalidMediaType";
	         break;
	      case StorageError.InvalidMetadata:
	         ErrorString = "InvalidMetadata";
	         break;
	      case StorageError.InvalidMetadataName:
	         ErrorString = "InvalidMetadataName";
	         break;
	      case StorageError.InvalidOperation:
	         ErrorString = "InvalidOperation";
	         break;
	      case StorageError.InvalidParameter:
	         ErrorString = "InvalidParameter";
	         break;
	      case StorageError.InvalidProjectType:
	         ErrorString = "InvalidProjectType";
	         break;
	      case StorageError.InvalidSequence:
	         ErrorString = "InvalidSequence";
	         break;
	      case StorageError.InvalidStringType:
	         ErrorString = "InvalidStringType";
	         break;
	      case StorageError.InvalidTime:
	         ErrorString = "InvalidTime";
	         break;
	      case StorageError.InvalidTimeUnit:
	         ErrorString = "InvalidTimeUnit";
	         break;
	      case StorageError.InvalidTrackType:
	         ErrorString = "InvalidTrackType";
	         break;
	      case StorageError.MediaFull:
	         ErrorString = "MediaFull";
	         break;
	      case StorageError.MetadataNotFound:
	         ErrorString = "MetadataNotFound";
	         break;
	      case StorageError.MetadataTypeMismatch:
	         ErrorString = "MetadataTypeMismatch";
	         break;
	      case StorageError.NotEnoughStorage:
	         ErrorString = "NotEnoughStorage";
	         break;
	      case StorageError.NotFound:
	         ErrorString = "NotFound";
	         break;
	      case StorageError.NotImplemented:
	         ErrorString = "NotImplemented";
	         break;
	      case StorageError.NotInitialized:
	         ErrorString = "NotInitialized";
	         break;
	      case StorageError.NotSupported:
	         ErrorString = "NotSupported";
	         break;
	      case StorageError.ReadError:
	         ErrorString = "ReadError";
	         break;
	      case StorageError.ReentrantCall:
	         ErrorString = "ReentrantCall";
	         break;
	      case StorageError.StreamCallbackError:
	         ErrorString = "StreamCallbackError";
	         break;
	      case StorageError.TrackCountOverflow:
	         ErrorString = "TrackCountOverflow";
	         break;
	      case StorageError.UnsupportedVersion:
	         ErrorString = "UnsupportedVersion";
	         break;
	      case StorageError.UserCancelled:
	         ErrorString = "UserCancelled";
	         break;
	      case StorageError.WriteError:
	         ErrorString = "WriteError";
	         break;
	      default:
	         {   
	         System.out.printf("Missing error text in app: [%d]\n", err);

	         }
	         break;
	   };

    	
        System.out.printf("Error received: [ %s ], errorCode is:[%d]\n",ErrorString, err);
    }
    
    public static int doReadVolume(Device dev, VolumeEventListenerTest volCB)
    {
        // read operation
        Volume vol = new Volume();
        int err = vol.OpenVolume(dev, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        err = vol.MountVolume(Volume.ReadOnly, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        getVolumeInfo(vol);
        // read volume contents
        err = ReadVolume(vol, srcPath, true);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        err = vol.CloseVolume();
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }

        return err;

    }
	public static int doWriteFormat(Device mgr, String srcPath, VolumeEventListenerTest volCB, Main m)
    {
        Format mgdFormat = new Format();
//        Volume.InfoCallback cb = new Volume.InfoCallback(new DelegateClass().VolumeCallback);
        int err = mgdFormat.Open(mgr, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        err = mgdFormat.Create(FSType, VolFormatType, "FormatCSTestApp", VolAccessMode, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        if (VolFormatType == Volume.Format_Type.Type_AudioCD)
        {
            Audio audioCD = new Audio(mgdFormat);
            File dirInfo = new File(srcPath);
            if (!dirInfo.exists())
                return -1;
            File[] subfileList = dirInfo.listFiles();
            for (int i = 0; i < subfileList.length; i++)
            {
                String audioFileName = subfileList[i].getAbsolutePath();
                Audio.FileType fileType = Audio.FileType.File_AutoDetect;
                if (isStreaming)
                    // streaming only does PCM for now
                    fileType = Audio.FileType.File_PCM;
                else
                {
                    // get file type based on extension of file
					int extPos = audioFileName.lastIndexOf('.');
					String ext = "";
					if (extPos >= 0)
						ext = audioFileName.substring(extPos+1);
                    if (ext.compareToIgnoreCase("wav") == 0)
                       fileType = Audio.FileType.File_WAV;
                    else if (ext.compareToIgnoreCase("RAW") == 0)
                       fileType = Audio.FileType.File_PCM;
                    else if (ext.compareToIgnoreCase("MP3") == 0)
                       fileType = Audio.FileType.File_MP3;
                    else
                    	audioFileName = ""; // set filename to empty since this is not a valid audio source

                }

                if (audioFileName != "")
                {
	                if (isStreaming)
	                {
	                    doWriteAudioFileFromStream(mgdFormat, audioCD, audioFileName, fileType, m);
	                }
	                else
	                {
	                    int trk = 0;
	                    err = audioCD.Format_AddAudioTrackFromFile(audioFileName, fileType);
	                    if (err != 0)
	                    {
	                        DisplayError(err);
	                        return err;
	                    }
	                    else
	                    	System.out.printf("Added track %d as filename %s\n", trk, audioFileName);
	                }
                }
            }
        }

        else if (VolFormatType == Volume.Format_Type.Type_DVD)
        {
            err = mgdFormat.CreateEntryFromHD(srcPath, "/VIDEO_TS", RTFile.Create_Parents);
            if (err != 0)
            {
                DisplayError(err);
                return err;
            }
        }

        err = mgdFormat.PrepareFlushVolume(VolFlushOption, volCB, null, VerifyOption);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        err = mgdFormat.FlushVolume(VolFlushOption, volCB, null, VerifyOption);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        err = mgdFormat.Close();
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        return err;
    }
	public static int doExtractVolumeToHD(Device dev, String srcPath, String destPath, VolumeEventListenerTest volCB )
    {
        int err = 0;

        Format mgdFormat = new Format();
        err = mgdFormat.Open(dev, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        err = mgdFormat.MountVolume(Volume.ReadOnly, volCB, null);
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        Long numFiles = mgdFormat.GetEntryNumChildren(srcPath);
        Audio audioCD = new Audio(mgdFormat);
        for (Integer i = 1; i <= numFiles.intValue(); i++)
        {
            String thisFileName = "ExtractedTrack" + i.toString() + ".wav";
            String destFilePath = destPath + "\\" + thisFileName;
            Audio.Track_EncoderSettings encodeSettings = audioCD.new Track_EncoderSettings();
            encodeSettings.destFileType = Audio.FileType.File_WAV;
            encodeSettings.structSizeBytes = 0;

            Audio.Track_Tags tags = audioCD.new Track_Tags();
            tags.Album = "ALbumName";
            tags.Title = "TitleName";
            tags.Track = "TrackName";

			int[] someIndex = new int[1];
			err = audioCD.Format_ExtractAudioTrackToFile(i.intValue(), destFilePath, 0, 0, -1, tags, encodeSettings, volCB, null);
			if (err != 0)
				DisplayError(err);
        }

        err = mgdFormat.Close();
        if (err != 0)
        {
            DisplayError(err);
            return err;
        }
        return err;
    }
	public static void GetSDKProperties()
	{
		SDKProperties.SettingType[] settingsList = SDKProperties.SettingType.values();
		for (SDKProperties.SettingType ourType :settingsList)
		{
			System.out.println(ourType);
        	Object mediaPropVal = sdkProp.GetSDKObjectProperty(ourType);
        	if (mediaPropVal != null)
        	{
        		if (ourType == SDKProperties.SettingType.Prop_Driver)
        			System.out.println(GetDriverTypeString((Integer)mediaPropVal));
        		else
        			System.out.println(mediaPropVal);
        	}
		}
	}
    public static void ParseArgs(String[] argsList)
    {
        System.out.printf("Found %d arguments in command line\n", argsList.length);
        // add more args parsing here for add'l arguments
        // current syntax:  <device> <src> <dst> <FSType> <VolAccessMode> <operation> [Options=<options1>=<option1Val>;...<optionsN>=<optionsNVal>;]
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
        //                      4 - erase
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
        //
        //					StreamData=1     for streaming from client stream callback
        //
        //					MultipleTargets=1,2,c:\test.iso for SetDeviceTarget to optical or files
        //
        //					Format=Type_DVD  for AS_Format of DVD
        //					Format=Type_Audio for AS_Format of Audio
        //
        //                  DriverType=92    for using IMAPI transport
        //
        //					LogPath=c:\log   for logging to custom location

        if (argsList.length > 0)
            devicePath = argsList[0];
        
        if (argsList.length > 2)
        {
            srcPath = argsList[1];
            dstPath = argsList[2];
        }
        if (argsList.length > 3)
                FSType = Integer.parseInt(argsList[3]);
        if (argsList.length > 4)
            VolAccessMode = Integer.parseInt(argsList[4]);
        if (argsList.length > 5)            
            operation = Integer.parseInt(argsList[5]);
        if (argsList.length > 6)
        {
            //String addlOptions = argsList[6];
            String addlOptions = argsList[6];
            String addlseps = "[;,\\s]";
            String [] addlOptionsList = new String[48]; // lets start with 48 additional options
            addlOptionsList = addlOptions.split(addlseps);

            
            for (int i = 0; i < addlOptionsList.length;i++ )
			{
				String addlOptionName = addlOptionsList[i];
				
	            if (addlOptionName.startsWith("FlushBorder"))
	            {
	                String option = addlOptionName.substring(addlOptionName.lastIndexOf('=')+1);
	                int addlOpt = Integer.parseInt(option);
	                if (addlOpt == 1)
	                    VolFlushOption = Volume.Flush_Border_Options.Border_Open;
	                else if (addlOpt == 3)
	                    VolFlushOption = Volume.Flush_Border_Options.Border_Final;
	                // otherwise continue with default (Border_Close)
	            }
	            else if (addlOptionName.startsWith("FlushVerify"))
	            {
	                String option = addlOptionName.substring(addlOptionName.lastIndexOf('=')+1);
	                int addlOpt = Integer.parseInt(option);
	                if (addlOpt == 1)
	                    VerifyOption = 1;
	                // otherwise we'll keep default for no verify
	         
	            }
	            else if (addlOptionName.startsWith("EraseOption") && 
	                    operation == 4)
	            {
	                String option = addlOptionName.substring(addlOptionName.lastIndexOf('=')+1);
	                int addlOpt = Integer.parseInt(option);
	                if (addlOpt == 1)
	                    EraseOption = Device.Erase_Type.Long_Erase;
	                // otherwise we'll keep default for Quick_Erase
	            }
	            else if (addlOptionName.startsWith("MultipleTargets") &&
	            		operation == 0 || operation == 1)
	            {
	            	int foundIdx = addlOptionName.indexOf("=");
	            	String option = "";
	            	option = addlOptionName.substring(foundIdx+1);
	            	String seps = "[;,\\s]";
	            	String[] test = new String[64];
	            	test = option.split(seps, 64);
	            	destDevicePaths = new String[test.length];
	            	destDevicePaths = option.split(seps,test.length);
	            }
	            else if (addlOptionName.startsWith("StreamData") && operation == 0 )
	            {
	                String option = addlOptionName.substring(addlOptionName.lastIndexOf('=')+1, addlOptions.length());
	                int addlOpt = Integer.parseInt(option);
	                if (addlOpt == 1)
	                    isStreaming = true;
	                else
	                	isStreaming = false;
	                // otherwise we'll keep default for reading from HDD and not 
	                // use callback for streaming data from client
	
	            }
	            else if (addlOptionName.startsWith("Format") &&
						operation == 5)
				{
					String option = addlOptionName.substring(addlOptionName.lastIndexOf('=') + 1);
					Volume.Format_Type[] possibleTypes = Volume.Format_Type.values();
					Volume.Format_Type outType;
					int index = 0;
					while (index < possibleTypes.length)
					{
						outType = possibleTypes[index++];
						String currentTypeStr = outType.toString();
						if (currentTypeStr.equalsIgnoreCase(option))
						{
							VolFormatType = outType;
							break;
						}
					}
				}
	            else if (addlOptionName.startsWith("LogPath"))
	            {
	            	String option = addlOptionName.substring(addlOptionName.lastIndexOf('=') + 1);
	            	sdkProp.SetSDKObjectProperty(SDKProperties.SettingType.Prop_LogPath, option);
	            }
	            else if (addlOptionName.startsWith("DriverType"))
	            {
	            	String option = addlOptionName.substring(addlOptionName.lastIndexOf('=') + 1);
	            	int addlOpt = Integer.parseInt(option);
	                sdkProp.SetSDKObjectProperty(SDKProperties.SettingType.Prop_Driver, addlOpt);
	            }
	            else if (addlOptionName.startsWith("Callbacks"))
	            {
	            	String option = addlOptionName.substring(addlOptionName.lastIndexOf('=') + 1);
	            	int addlOpt = Integer.parseInt(option);
	            	if (addlOpt == 0)
	            		isCallingBack = false;
	            	else
	            		isCallingBack = true;
	            }

			}
        }        
        
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        Main m = new Main();
        m.logger = Logger.getLogger("com.sonic.as_storage_java");
        try
        {
	        m.traceStream = new FileHandler("CSharpTrace.txt");
	        if (m.traceStream != null)
	        {
	        	m.traceStream.setFormatter(new SimpleFormatter());
	        	m.logger.addHandler(m.traceStream);
	        	m.logger.setLevel(Level.FINEST);
	            //traceListener = new System.Diagnostics.TextWriterTraceListener(traceStream);
	           // System.Diagnostics.Trace.Listeners.Add(traceListener);
	
	        }
        }
        catch(IOException e)
        {
        	e.printStackTrace();
        }
        
        String userDir = System.getProperty("user.dir");
        System.out.println(userDir);
        sdkProp = new SDKProperties();
        GetSDKProperties();
        if (args.length > 0)
        {
            ParseArgs(args);
        }
        else
        {
            try
            {
                    System.out.println("Enter drive index:");
                    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
                    devicePath = in.readLine();
            }
            catch(IOException e)
            {
                    System.out.println("Failed to read input string");
            }
        }
        int deviceIndex = 0;
        if (IsNum(devicePath))
        {
            deviceIndex = Integer.parseInt(devicePath);
            if (deviceIndex != 0)
            	devicePath = null;
            else
            	devicePath = "";
        }
                        
        try
        {
            Device dev = new Device();
            DeviceEventListenerTest devCB = null;
            VolumeEventListenerTest volumeCB = null;
            if (isCallingBack)
            {
            	devCB = m.new DeviceEventListenerTest();
            	volumeCB = m.new VolumeEventListenerTest();
            }
            /*dev.SetCallback(Device.SetCallbackType.Callback_Register,0, devCB, 0, 0); 
            for (int idfd=0; idfd<100;idfd++)
            {
            	if ((idfd%10)==0)
            			idfd = 0;
            	Thread.sleep(10000);
            }
            */
            int numDevices = dev.GetDeviceCount(Device.Scan_All);
            System.out.printf("Found %d optical devices\n", numDevices);
            GetSDKProperties();
            // open up the source device (index or path).
            int err = dev.OpenDevice(deviceIndex, devicePath);
            if (err != 0)
            {
                DisplayError(err);
                return;
            }

            err = dev.TrayControl(Device.TrayControls.Tray_Control_Close_Sync);
            if (err != StorageError.None)
            	DisplayError(err);
            err = dev.ExclusiveAccess(Device.ExclusiveAccess_Type.ExclusiveAccess_Obtain, "AS_Storage_JavaTest");
            if (err != StorageError.None)
            {
            	String currUsedApp = dev.GetAppInUse();
            	System.out.println("Device currently being used by: " + currUsedApp);
            	DisplayError(err);
            }
            else
            {
            	getDeviceInfo(dev);
	            getDeviceState(dev);
	            err = getMediaInfo(dev);
	            if (err != 0)
	            {
	                DisplayError(err);
	            }
	
	            if (operation == 0)
	            {
	                err = doWriteVolume(dev, volumeCB, m);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }
	            else if (operation == 1)
	            {
	                err = doCopy(dev, devCB);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }
	            else if (operation == 2)
	            {
	                err = doVerify(dev, devCB);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }
	            else if (operation == 3)
	            {
	                err = doReadVolume(dev, volumeCB);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }
	            else if (operation == 4)
	            {
	                err = doErase(dev, devCB);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }
	            
	            else if (operation == 5)
	            {
	                err = doWriteFormat(dev, srcPath, volumeCB, m);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }
	    		else if (operation == 6)
	            {
	                err = doExtractVolumeToHD(dev, srcPath, dstPath, volumeCB);
	                if (err != 0)
	                {
	                    DisplayError(err);
	                }
	            }

	            err = dev.ExclusiveAccess(Device.ExclusiveAccess_Type.ExclusiveAccess_Release, "AS_Storage_JavaTest");
	            if (err != 0)
	            {
	                DisplayError(err);
	            }
            }
            
            err = dev.CloseDevice();
            if (err != 0)
            {
                DisplayError(err);
            }

        }
        catch (Exception e)
        {
            e.printStackTrace(); 

        }
        
        try
        {
       		m.logger.removeHandler(m.traceStream);
    		m.traceStream.flush();
    		m.traceStream.close();
        }
        catch (Exception e)
        {
        	e.printStackTrace();
        }
        
        //System.gc();
        
    }

    public static int ReadVolume(Volume vol, String path, boolean readContents)
    {

        int err = 0;
        
        long numChildren = vol.GetEntryNumChildren(path);
        
        for (int index = 0; index < numChildren; index++)
        {
            String nextPath = vol.GetEntry(path, index);
            System.out.printf("Entry is %s\n", nextPath);
            int fileType = vol.GetEntryType(nextPath);
            if (fileType == RTFile.File_IsDir)
            {
                System.out.printf("Dir Entry is %s\n", nextPath);
                err = ReadVolume(vol, nextPath, readContents);
            }
            else
            {
                err = GetFileInfo(vol, nextPath, readContents);
            }
        }
        return err;
    }
    public static int GetFileInfo(Volume vol, String path, boolean readContents)
    {
        int err = 0;
        
        RTFile ourFile = new RTFile();
        err = ourFile.OpenFile(vol, path, RTFile.OpenMode.Read);
        
        err = GetFileProperties(vol, ourFile);
        
        if (readContents)
        {
            err = ReadFile(vol, ourFile, 0);
        }
        
        err = ourFile.CloseFile();
        
        return err;
        
    }
    public static int GetFileProperties(Volume vol, RTFile file)
    {
        int err = 0;
        
        String fileName = (String)file.GetFileObjectProperty(RTFile.Property.Prop_Name);
        System.out.printf("File Name is: %s\n", fileName);
        
     	RTFile.Property[] availTypes = RTFile.Property.values();
    	for (RTFile.Property ourType : availTypes)
    	{
    		System.out.println(ourType);
    		Object propVal = file.GetFileObjectProperty(ourType);
    		
    		if (ourType == RTFile.Property.Prop_DateCreated || 
    				ourType == RTFile.Property.Prop_DateModified)
    		{
    			System.out.println(GetVolumeDateTimeString((Volume.DateTime)propVal));
    		}
    		else
    			System.out.println(propVal);
    	}
        
        return err;
    }
    public static int ReadFile(Volume vol, RTFile file, long pos)
    {
        
        int err = 0;
        long fileSize = (Long)file.GetFileObjectProperty(RTFile.Property.Prop_Size);
        if (fileSize == 0)
        	return err;
        
        if (pos > 0)
        {
            err = file.SeekFile(pos);
        }
        
        long curPos = (Long)file.GetFileObjectProperty(RTFile.Property.Prop_CurrentPos);

        int readSize = 0x20*2048;
        if (fileSize < readSize)
            readSize = (int)fileSize;
        
        while (curPos <= fileSize)
        {
            byte[] buffer = file.ReadFile(readSize);
            curPos += readSize;
           
        }
        
        return err;
    }
    public static int visitAllDirsAndFiles(Volume vol, File dir, Main m) 
    {
    	int err = 0;
        if (dir.isDirectory()) 
        {
            String[] children = dir.list();
            for (int i=0; i<children.length; i++) 
            {
                String filename = children[i];
                String name;

                try
                {
                    name = dir.getCanonicalPath();
                }
                catch(IOException e)
                {
                    System.out.println("Failed to get canonicalPath " + dir.toString()+ " ");
                    e.printStackTrace(); 
                    return -1;
                }
                int p = name.indexOf(':');
                String newname = name;
                try
                {

                    if (p != -1)
                    {
                    	newname = name.substring(p + 1);
                    }
                    long spaceAvail = VolumeFreeSpace;
                    System.out.println("Volume space available is: " + (spaceAvail) + " bytes");
                    if (spaceAvail < 2048)
                    {
                    	System.out.println("Failed to add more directories (at least 2k required)");
                    	System.out.println("DirSz = 2048 bytes, spaceRemaining = " + (spaceAvail) + " bytes");
                        return err;
                    }
                    
                    VolumeFreeSpace -= 2048; // 2k for this directory entry

                }
                catch(Exception e)
                {
                        System.out.println("Failed to add directory " + newname + "  ");
                        e.printStackTrace(); 
                }

                err = visitAllDirsAndFiles(vol, new File(dir, filename), m);
           		if (err != 0)
           		{
           			DisplayError(err);
           			vol.CloseVolume();
           			return err;
           			
           		}
            }
            try
            {
                err = vol.CreateEntry(dir.getCanonicalPath(), RTFile.File_IsDir, RTFile.Create_Parents);
           		if (err == StorageError.FileExists || err == StorageError.FolderExists)
           			err = 0; // allow passthru for dirs already created
           		
           		if (err != 0)
           		{
           			DisplayError(err);
           			vol.CloseVolume();
           			return err;
           			
           		}
            }
            catch (Exception e)
            {
            }
        }
        else
        {
            RTFile file = new RTFile();
            InputStream inputStream = null; 
            try 
            { 
                String name = dir.getCanonicalPath(); 
                int p = name.indexOf(':'); 
                String newname = name; 
                if (p != -1) 
                { 
                        newname = name.substring(p + 1); 
                } 
//              vol.CreateEntry(newname, RTFile.File_IsFile, RTFile.Create_Parents); 

                System.out.println("Java file path:" + newname); 

                long fileSize = dir.length();
                long spaceAvail = VolumeFreeSpace;
                System.out.println("Volume space available is: " + (spaceAvail) + " bytes");
                if (spaceAvail < fileSize)
                {
                	System.out.println("Failed to add more files");
                	System.out.println("FileSz = " + fileSize + " bytes, spaceRemaining = " + spaceAvail + " bytes");
                	return err;
                }
                if (fileSize == 0)
                {
                	// use createEntry for 0k files
                	err = vol.CreateEntry(newname, RTFile.File_IsFile, RTFile.Create_Parents);
                }
                else if (VolAccessMode == Volume.Mastering && isStreaming)
                {
	                try
	                {
	                	FileInputStream Fstream = new FileInputStream(dir);
	                    long streamLen = Fstream.getChannel().size();
	                    int curIndex = myList.AddStream(Fstream);
	                    VolumeEventListenerTest streamCB = m.new VolumeEventListenerTest();
	                    err = vol.CreateFileFromStream(dir.getCanonicalPath(), streamLen, streamCB, myList.readers.elementAt(curIndex-1).getFD(), RTFile.Create_Overwrite | RTFile.Create_Parents);
                   		if (err != 0)
                   		{
                   			DisplayError(err);
                   			vol.CloseVolume();
                   			return err;
                   			
                   		}	                    
	                	VolumeFreeSpace -= dir.length();					                              
	                }
	                catch (Exception e)
	                {
	                	e.printStackTrace();
	                }
                }
                else if (VolAccessMode == vol.Mastering)
                {
                	err = vol.CreateEntryFromHD(dir.getCanonicalPath(), newname,  RTFile.Create_Parents);
                	VolumeFreeSpace -= dir.length();
               		if (err != 0)
               		{
               			DisplayError(err);
               			vol.CloseVolume();
               			return err;
               			
               		}
                }
                else if (VolAccessMode == vol.RTTD)
                {

	                file.OpenFile(vol, newname, RTFile.OpenMode.Append); 
	                inputStream = new FileInputStream(dir); 
	                byte[] bytes = new byte[2048]; 
	                int len = 0; 
	                long totalRead = 0;
	                double pct, lastpct = 0;
	                while ((len = inputStream.read(bytes)) != -1) 
	                { 
	                    if (bytes.length != len) 
	                    { 
	                        byte[] b = new byte[len]; 
	                        for (int i=0; i<len; i++) 
	                                b[i] = bytes[i]; 
	                        err = file.WriteFile(len, b); 
	                        if (err != 0)
	                            return err;
	                    } 
	                    else 
	                    {
	                    	err = file.WriteFile(len, bytes);
	                   		if (err != 0)
	                   		{
	                   			DisplayError(err);
	                   			file.CloseFile();
	                   			vol.CloseVolume();
	                   			return err;
	                   			
	                   		}
	                    }
	
	                    totalRead += len;
	                    if (totalRead > 2048)
	                    {
	                        pct = (totalRead*100/fileSize);
	                        if ((lastpct != pct) && ((pct %10) == 0))
	                        {
	                            System.out.printf("Writing completed: %f\n ",pct );
	
	                            lastpct = pct;
	                        }
	
	                    }
	              //      bytes = new byte[2048]; 
	                } 
	                
	                err = file.CloseFile(); 
               		if (err != 0)
               		{
               			DisplayError(err);
               			vol.CloseVolume();
               			return err;
               			
               		}
	                VolumeFreeSpace -= totalRead;
                }

            } 
            catch (Exception e)
            {
                e.printStackTrace(); 

            }
            finally {
                } 
            try { 
                    if (inputStream != null) 
                            inputStream.close();

            } catch (Exception e) { 
                    e.printStackTrace(); 
            } 
        }
        
        long currentVolSize = vol.GetVolumeLongProperty(Volume.Property.Prop_FreeBlocks)*vol.GetVolumeLongProperty(Volume.Property.Prop_BlockSize);
        System.out.println("SDK says: " + currentVolSize + " space has been Used");
        System.out.println("Client says: " + VolumeFreeSpace + " space has been Used");
        
        return err;
    }        
    public static String GetMediaStateString(int medState)
    {
        String medStr = "";
        if ((medState & Device.MediaIsReadable) == Device.MediaIsReadable)
            medStr += "Readable ";
        if ((medState & Device.MediaIsAppendable) == Device.MediaIsAppendable)
            medStr += "Appendable ";
        if ((medState & Device.MediaIsBlank) == Device.MediaIsBlank)
            medStr += "Blank ";
        if ((medState & Device.MediaIsErasable) == Device.MediaIsErasable)
            medStr += "Eraseable ";
        if ((medState & Device.MediaIsWritable) == Device.MediaIsWritable)
            medStr += "Writable ";
        if ((medState & Device.MediaIsInvisible) == Device.MediaIsInvisible)
            medStr += "Invisible ";
        if ((medState & Device.MediaIsRewritable) == Device.MediaIsRewritable)
            medStr += "Rewritable ";
        if ((medState & Device.MediaIsProtected) == Device.MediaIsProtected)
            medStr += "Protected ";
        
        return medStr;
    }
    
    public static String GetMediaTypeString(long medType)
    {
        if (medType == Device.MediaIsBdr)
                return "BDR";
        else if (medType == Device.MediaIsBdre)
                return "BDRE";
        else if (medType == Device.MediaIsBdrom)
                return "BDROM";
        else if (medType == Device.MediaIsCdRom)
                return "CDRom";
        else  if (medType == Device.MediaIsCdr)
            return "CDR";
        else  if (medType == Device.MediaIsCdrw)
            return "CDRW";
        else  if (medType == Device.MediaIsDvdRam)
            return "DVD-RAM";
        else if (medType == Device.MediaIsDvdRom)
            return "DVD-Rom";
        else if (medType == Device.MediaIsDvdpr)
            return "DVD+R";
        else if (medType == Device.MediaIsDvdpr9)
            return "DVD+R9";
        else if (medType == Device.MediaIsDvdprw)
            return "DVD+RW";
        else if (medType == Device.MediaIsDvdr)
            return "DVD-R";
        else if (medType == Device.MediaIsDvdr9)
            return "DVD-R9";
        else if (medType == Device.MediaIsDvdrw)
            return "DVD-RW";
        else if (medType == Device.MediaIsHdDvdRom)
            return "HD DVD-Rom";
        else if (medType == Device.MediaIsHdDvdr)
            return "HD DVD-R";
        else if (medType == Device.MediaIsHdDvdrw)
            return "HD DVD-RW";

        return "";
    }


    public static String GetDeviceMedCapableString(long medType)
    {
        String retVal = "";
        if ((medType & Device.MediaIsBdr) == Device.MediaIsBdr)
                retVal += "BDR ";
        if ((medType & Device.MediaIsBdre) ==Device.MediaIsBdre)
                retVal +="BDRE ";
        if ((medType & Device.MediaIsBdrom) == Device.MediaIsBdrom)
                retVal +="BDROM ";
        if ((medType & Device.MediaIsCdRom) == Device.MediaIsCdRom)
                retVal +="CDRom ";
        if ((medType & Device.MediaIsCdr) == Device.MediaIsCdr)
            retVal +="CDR ";
        if ((medType & Device.MediaIsCdrw) == Device.MediaIsCdrw)
            retVal +="CDRW ";
        if ((medType & Device.MediaIsDvdRam) == Device.MediaIsDvdRam)
            retVal +="DVD-RAM ";
        if ((medType & Device.MediaIsDvdRom) == Device.MediaIsDvdRom)
            retVal +="DVD-Rom ";
        if ((medType & Device.MediaIsDvdpr) == Device.MediaIsDvdpr)
            retVal +="DVD+R ";
        if ((medType & Device.MediaIsDvdpr9) == Device.MediaIsDvdpr9)
            retVal +="DVD+R9 ";
        if ((medType & Device.MediaIsDvdprw) == Device.MediaIsDvdprw)
            retVal +="DVD+RW ";
        if ((medType & Device.MediaIsDvdr) == Device.MediaIsDvdr)
            retVal +="DVD-R ";
        if ((medType & Device.MediaIsDvdr9) == Device.MediaIsDvdr9)
            retVal +="DVD-R9 ";
        if ((medType & Device.MediaIsDvdrw) == Device.MediaIsDvdrw)
            retVal +="DVD-RW ";
        if ((medType & Device.MediaIsHdDvdRom) == Device.MediaIsHdDvdRom)
            retVal +="HD DVD-Rom ";
        if ((medType & Device.MediaIsHdDvdr) == Device.MediaIsHdDvdr)
            retVal +="HD DVD-R ";
        if ((medType & Device.MediaIsHdDvdrw) == Device.MediaIsHdDvdrw)
            retVal +="HD DVD-RW ";        
        return retVal;
    }
    public static String GetDeviceAuthString(int value)
    {
        String retVal = "";
        if ((value & Device.Authentication_AACS) == Device.Authentication_AACS)
            retVal += "AACS ";
        if ((value & Device.Authentication_CPPM ) == Device.Authentication_CPPM)
            retVal += "CPPM ";
        if ((value & Device.Authentication_CPRM ) == Device.Authentication_CPRM)
            retVal += "CPRM ";
        if ((value & Device.Authentication_None) == Device.Authentication_None)
            retVal += "None ";
        return retVal;
    }
    public static String GetVolumeFSTypeString(int val)
    {
    	String retVal = "";
    	if ((val & Volume.FS_Audio) == Volume.FS_Audio)
    		retVal += "FS_Audio";
    	else if ((val & Volume.FS_Bridge) == Volume.FS_Bridge)
    		retVal += "FS_Bridge";
    	else if ((val & Volume.FS_Default) == Volume.FS_Default)
    		retVal += "FS_Default";
    	else if ((val & Volume.FS_ISO) == Volume.FS_ISO)
    		retVal += "FS_ISO";
    	else if ((val & Volume.FS_ISO_Level_1) == Volume.FS_ISO_Level_1)
    		retVal += "FS_ISO_Level_1";
    	else if ((val & Volume.FS_ISO_Level_2) == Volume.FS_ISO_Level_2)
    		retVal += "FS_ISO_Level_2";
    	else if ((val & Volume.FS_ISO_Level_2_Long) == Volume.FS_ISO_Level_2_Long)
    		retVal += "FS_ISO_Level_2_Long";
    	else if ((val & Volume.FS_ISO_Versionless) == Volume.FS_ISO_Versionless)
    		retVal += "FS_ISO_Versionless";
    	else if ((val & Volume.FS_Joliet) == Volume.FS_Joliet)
    		retVal += "FS_Joliet";
    	else if ((val & Volume.FS_MODE2) == Volume.FS_MODE2)
    		retVal += "FS_MODE2";
    	else if ((val & Volume.FS_None) == Volume.FS_None)
    		retVal += "FS_None";
    	else if ((val & Volume.FS_TDF) == Volume.FS_TDF)
    		retVal += "FS_TDF";
    	else if ((val & Volume.FS_UDF102) == Volume.FS_UDF102)
    		retVal += "FS_UDF102";
    	else if ((val & Volume.FS_UDF15) == Volume.FS_UDF15)
    		retVal += "FS_UDF15";
    	else if ((val & Volume.FS_UDF2) == Volume.FS_UDF2)
    		retVal += "FS_UDF2";
    	else if ((val & Volume.FS_UDF25) == Volume.FS_UDF25)
    		retVal += "FS_UDF25";
    	else if ((val & Volume.FS_UDF26) == Volume.FS_UDF26)
    		retVal += "FS_UDF26";
    	else if ((val & Volume.FS_VRT) == Volume.FS_VRT)
    		retVal += "FS_Audio";
    	
    	return retVal;
    }
    public static String GetVolumeFormatTypeString(int value)
    {
    	String retVal = "";
    	if (value == Volume.Format_Type.Type_AudioCD.value())
    		retVal = "Type_AudioCD";
    	else if (value == Volume.Format_Type.Type_BDAV.value())
    		retVal = "Type_BDAV";
    	else if (value == Volume.Format_Type.Type_BDMV.value())
    		retVal = "Type_BDMV";
    	else if (value == Volume.Format_Type.Type_CMF10_BD.value())
    		retVal = "Type_CMF10_BD";
    	else if (value == Volume.Format_Type.Type_CMF20_HD.value())
    		retVal = "Type_CMF20_HD";
    	else if (value == Volume.Format_Type.Type_Data.value())
    		retVal = "Type_Data";
    	else if (value == Volume.Format_Type.Type_DDP20.value())
    		retVal = "Type_DDP20";
    	else if (value == Volume.Format_Type.Type_DDP21.value())
    		retVal = "Type_DDP21";
    	else if (value == Volume.Format_Type.Type_DDP30.value())
    		retVal = "Type_DDP30";
    	else if (value == Volume.Format_Type.Type_DVD.value())
    		retVal = "Type_DVD";
    	else if (value == Volume.Format_Type.Type_DVDDashVR.value())
    		retVal = "Type_DVDDashVR";
    	else if (value == Volume.Format_Type.Type_DVDPlusVR.value())
    		retVal = "Type_DVDPlusVR";
    	else if (value == Volume.Format_Type.Type_HDDVD.value())
    		retVal = "Type_HDDVD";
    	else if (value == Volume.Format_Type.Type_MP3.value())
    		retVal = "Type_MP3";
    	else if (value == Volume.Format_Type.Type_SVCD.value())
    		retVal = "Type_SVCD";
    	else if (value == Volume.Format_Type.Type_Unknown.value())
    		retVal = "Unknown";
    	
    	return retVal;
    }
    public static String GetDeviceTrayStatusString(int value)
    {
        String retVal = "";
        if (value == Device.TrayStatus_Closed)
            retVal = "Tray Closed";
        else if (value == Device.TrayStatus_Open)
            retVal = "Tray Open";
        else if (value == Device.TrayStatus_Unknown)
            retVal = "Tray Unknown";
        return retVal;
        
    }
    public static String GetDeviceTypeString(int value)
    {
        String retVal = "";
        if ((value & Device.IsOptical) == 0)
            retVal = "Optical Device";
        else if ((value & Device.IsFile)==0)
            retVal = "Optical File";
        else if ((value & Device.IsTape) == 0)
        	retVal = "Tape";
        return retVal;
        
    }
    public static String GetVolumeDateTimeString(Volume.DateTime data)
    {
    	String retVal = "";
    	
    	retVal = ((Short)data.year).toString() + "/" + ((Byte)data.month).toString() + "/" + ((Byte)data.day).toString();
    	retVal += "  " + ((Byte)data.hour).toString() + ":" + ((Byte)data.minute).toString() + ":" + ((Byte)data.second).toString();
    	retVal += ((Byte)data.zone).toString();

    	return retVal;
    }
            
    public static String GetDeviceCopyProtString(int value)
    {
        String retVal = "";
        
        if ((value & Device.ProtectionType_GuardBlock_BadECC) == Device.ProtectionType_GuardBlock_BadECC)
            retVal += "CopyBlock";
        if ((value & Device.ProtectionType_MovieDisc_CSS) == Device.ProtectionType_MovieDisc_CSS)
            retVal += "MovieDisc";
        if ((value & Device.ProtectionType_None) == Device.ProtectionType_None)
            retVal += "None";
        return retVal;
        
    }
    
    public static String GetDriverTypeString(int value)
    {
        String retVal = "None";
        
        if (value == SDKProperties.Driver_Any)
            retVal = "Any driver";
        else if (value == SDKProperties.Driver_Best)
            retVal = "Best driver";
        else if (value == SDKProperties.Driver_Imapi)
            retVal = "Imapi driver";
        else if (value == SDKProperties.Driver_Pxhelp)
            retVal = "PxHelp driver";

        return retVal;
    	
    }
            
}
