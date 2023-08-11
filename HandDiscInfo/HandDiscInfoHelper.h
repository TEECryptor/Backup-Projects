//------------------------------------------------------
//	Name:HandDiscInfoHelper.h
//------------------------------------------------------
typedef struct _DRIVERSTATUS 
{ 
        BYTE     bDriverError;		//Error code from driver,  or 0 if no error. 
        BYTE     bIDEStatus;        //Contents of IDE Error register. 
        //Only valid when bDriverError is SMART_IDE_ERROR. 
        BYTE     bReserved[2];		//Reserved for future expansion. 
        DWORD     dwReserved[2];    //Reserved for future expansion. 
}DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS; 

typedef struct _SENDCMDOUTPARAMS 
{ 
        DWORD           cBufferSize;       //     Size   of   bBuffer   in   bytes 
        DRIVERSTATUS	DriverStatus;     //     Driver   status   structure. 
        BYTE            bBuffer[1];         //     Buffer   of   arbitrary   length   in   which   to   store   the   data   read   from   the                                                                                                               //   drive. 
}SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS; 

typedef struct _SRB_IO_CONTROL 
{ 
        ULONG   HeaderLength; 
        UCHAR   Signature[8]; 
        ULONG   Timeout; 
        ULONG   ControlCode; 
        ULONG   ReturnCode; 
        ULONG   Length; 
}SRB_IO_CONTROL, *PSRB_IO_CONTROL; 

typedef struct _IDEREGS 
{ 
        BYTE   bFeaturesReg;               //   Used   for   specifying   SMART   "commands ". 
        BYTE   bSectorCountReg;         //   IDE   sector   count   register 
        BYTE   bSectorNumberReg;       //   IDE   sector   number   register 
        BYTE   bCylLowReg;                   //   IDE   low   order   cylinder   value 
        BYTE   bCylHighReg;                 //   IDE   high   order   cylinder   value 
        BYTE   bDriveHeadReg;             //   IDE   drive/head   register 
        BYTE   bCommandReg;                 //   Actual   IDE   command. 
        BYTE   bReserved;                     //   reserved   for   future   use.     Must   be   zero. 
}IDEREGS, *PIDEREGS, *LPIDEREGS; 

typedef struct _SENDCMDINPARAMS 
{ 
        DWORD           cBufferSize;       //     Buffer   size   in   bytes 
        IDEREGS       irDriveRegs;       //     Structure   with   drive   register   values. 
        BYTE   bDriveNumber;               //     Physical   drive   number   to   send   
        //     command   to   (0,1,2,3). 
        BYTE   bReserved[3];               //     Reserved   for   future   expansion. 
        DWORD           dwReserved[4];   //     For   future   use. 
        BYTE             bBuffer[1];         //     Input   buffer. 
}SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS; 

typedef struct _GETVERSIONOUTPARAMS 
{ 
        BYTE   bVersion;             //   Binary   driver   version. 
        BYTE   bRevision;           //   Binary   driver   revision. 
        BYTE   bReserved;           //   Not   used. 
        BYTE   bIDEDeviceMap;   //   Bit   map   of   IDE   devices. 
        DWORD   fCapabilities;   //   Bit   mask   of   driver   capabilities. 
        DWORD   dwReserved[4];   //   For   future   use. 
}GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS; 

////////////////////////////////////////////////////////////////////// 

//结构定义   
typedef struct _UNICODE_STRING   
{   
        USHORT     Length;//长度   
        USHORT     MaximumLength;//最大长度   
        PWSTR		Buffer;//缓存指针   
}UNICODE_STRING, *PUNICODE_STRING;   

typedef struct _OBJECT_ATTRIBUTES   
{   
        ULONG   Length;//长度   18h   
        HANDLE   RootDirectory;//     00000000   
        PUNICODE_STRING   ObjectName;//指向对象名的指针   
        ULONG   Attributes;//对象属性00000040h   
        PVOID   SecurityDescriptor;                 //   Points   to   type   SECURITY_DESCRIPTOR，0   
        PVOID   SecurityQualityOfService;     //   Points   to   type   SECURITY_QUALITY_OF_SERVICE，0   
}OBJECT_ATTRIBUTES;  
 
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;   

//函数指针变量类型 
typedef DWORD (__stdcall *ZWOS)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);   
typedef DWORD (__stdcall *ZWMV)(HANDLE, HANDLE,PVOID, ULONG, ULONG, PLARGE_INTEGER, PSIZE_T, DWORD, ULONG, ULONG);   
typedef DWORD (__stdcall *ZWUMV)(HANDLE, PVOID);   



#define SENDIDLENGTH  256


BOOL WinNTHDSerialNumAsScsiRead(BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen) 
{ 
	BOOL bInfoLoaded = FALSE; 

	for(int iController = 0; iController < 2; ++iController) 
	{ 
        HANDLE hScsiDriveIOCTL = 0; 
        char szDriveName[256]; 
        
        //     Try   to   get   a   handle   to   PhysicalDrive   IOCTL,   report   failure 
        //     and   exit   if   can 't. 
        sprintf(szDriveName, "\\\\.\\Scsi%d: ", iController); 

        //     Windows   NT,   Windows   2000,   any   rights   should   do 
        hScsiDriveIOCTL = CreateFile(	szDriveName, 
										GENERIC_READ | GENERIC_WRITE,   
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										NULL, 
										OPEN_EXISTING, 0, NULL); 

        //   if   (hScsiDriveIOCTL   ==   INVALID_HANDLE_VALUE) 
        //         printf   ( "Unable   to   open   SCSI   controller   %d,   error   code:   0x%lX\n ", 
        //                         controller,   GetLastError   ()); 
        
        if(hScsiDriveIOCTL != INVALID_HANDLE_VALUE) 
        { 
			int iDrive = 0; 
			for(iDrive = 0; iDrive < 2; ++iDrive) 
			{ 
				char szBuffer[sizeof(SRB_IO_CONTROL) + SENDIDLENGTH] = {0}; 

				SRB_IO_CONTROL* p = (SRB_IO_CONTROL*)szBuffer; 
				SENDCMDINPARAMS* pin = (SENDCMDINPARAMS*)(szBuffer + sizeof(SRB_IO_CONTROL)); 
				DWORD dwResult; 

				p->HeaderLength = sizeof(SRB_IO_CONTROL); 
				p->Timeout = 10000; 
				p->Length = SENDIDLENGTH; 
				p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY; 
				strncpy((char*)p->Signature, "SCSIDISK ", 8); 

				pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY; 
				pin->bDriveNumber = iDrive; 

				if(DeviceIoControl(hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT,   
									szBuffer, 
									sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1, 
									szBuffer, 
									sizeof(SRB_IO_CONTROL) + SENDIDLENGTH, 
									&dwResult, NULL)) 
				{ 
					SENDCMDOUTPARAMS* pOut = (SENDCMDOUTPARAMS*)(szBuffer + sizeof(SRB_IO_CONTROL)); 
					IDSECTOR* pId = (IDSECTOR*)(pOut-> bBuffer); 
					if(pId->sModelNumber[0]) 
					{ 
						if(*puSerialLen + 20U <= uMaxSerialLen) 
						{ 
							//序列号 
							CopyMemory(dwSerial + *puSerialLen, ((USHORT*)pId) + 10, 20); 

							//   Cut   off   the   trailing   blanks 
							for(UINT i = 20; i != 0U && ' ' == dwSerial[*puSerialLen + i - 1]; --i) 
							{} 
							*puSerialLen += i; 

							//型号 
							CopyMemory(dwSerial + *puSerialLen, ((USHORT*)pId) + 27, 40); 
							// Cut   off   the   trailing   blanks 
							for(i = 40; i != 0U && ' ' == dwSerial[*puSerialLen + i - 1]; -- i) 
							{} 
							* puSerialLen += i; 

							bInfoLoaded = TRUE; 
						} 
						else 
						{ 
							::CloseHandle( hScsiDriveIOCTL); 
							return bInfoLoaded; 
						} 
					} 
				} 
			} 
			::CloseHandle( hScsiDriveIOCTL ); 
        } 
	} 

	return   bInfoLoaded; 
}