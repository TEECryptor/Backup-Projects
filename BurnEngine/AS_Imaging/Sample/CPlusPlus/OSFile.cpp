// OsxPort: testing checkins

#include <iostream>
#include <iosfwd>
#if !defined (__bcm__)
#include <ostream>
#include <fstream>
#endif
#include <vector>
#include <string>
#include "AS_StorageTypes.h"
#include "AS_StorageError.h"
#include "AS_Volume.h"
#include "AS_File.h"
#include "AS_StringHelper.h"
// handles for OSs
#if defined (_WIN32)
#include <windows.h>
typedef HANDLE AppHandle;
#include <sys/stat.h>
#include <io.h>

#define USE_WCHAR_PATHS 1   // WIN32 using WCHAR for now on

#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#ifdef OSXPORT
// on Panther (10.3.9) there is no sys/statvfs
#else
#include <sys/statvfs.h>
#endif
typedef int AppHandle;
#endif


#define OTHER_Execute 0x00000001
#define OTHER_Write   0x00000002
#define OTHER_Read    0x00000004
#define OTHER_ChAttr  0x00000008
#define OTHER_Delete  0x00000010
#define GROUP_Execute 0x00000020
#define GROUP_Write   0x00000040
#define GROUP_Read    0x00000080
#define GROUP_ChAttr  0x00000100
#define GROUP_Delete  0x00000200
#define OWNER_Execute 0x00000400
#define OWNER_Write   0x00000800
#define OWNER_Read    0x00001000
#define OWNER_ChAttr  0x00002000
#define OWNER_Delete  0x00004000

#if defined (USE_WCHAR_PATHS)
#ifdef _WIN32
	typedef WIN32_FIND_DATAW AppFindData;
#endif
	typedef AS_StringW AppString;
	typedef std::wstring AppStdString;
	typedef wchar_t AppChar;
	const AS_String::Type AppStrType = AS_String::Type_Wide;
	#define _APP(x)      L ## x
	#define AppIn  std::wcin
	#define AppAtoi(x) wcstoul(x, 0, 10)
	#define AppSprintf swprintf
	#define AppIfStream std::wifstream 
	#define AppFindFirstFile FindFirstFileW
	#define AppFindNextFile FindNextFileW
	#define AppFileOpen _wfopen
#elif defined (USE_UTF8_PATHS)
	typedef AS_String8 AppString;
	typedef std::string AppStdString;
	typedef unsigned char AppChar;
	const AS_String::Type AppStrType = AS_String::Type_UTF8;
	#define _APP(x)     x
	#define AppIn  std::cin
	#define AppAtoi(x) atoi(x)
	#define AppSprintf sprintf
	#define AppIfStream std::ifstream 
#ifdef _WIN32
	typedef WIN32_FIND_DATAW AppFindData;
#endif
	#define AppFindFirstFile FindFirstFileW
	#define AppFindNextFile FindNextFileW
	#define AppFileOpen fopen
#else  // default to char paths
	typedef AS_StringA AppString;
	typedef std::string AppStdString;
	typedef char AppChar;
	const AS_String::Type AppStrType = AS_String::Type_ASCII;
	#define _APP(x)     x
	#define AppIn  std::cin
	#define AppAtoi(x) atoi(x)
	#define AppSprintf sprintf
	#define AppIfStream std::ifstream 
#ifdef _WIN32
	typedef WIN32_FIND_DATAA AppFindData;
#endif
	#define AppFindFirstFile FindFirstFileA
	#define AppFindNextFile FindNextFileA
	#define AppFileOpen fopen
#endif


static std::vector<AppIfStream*> readStreamlist;
static std::vector<AS_File*> readASFileList;
static std::vector<AppHandle*> readCFileList;

extern void OutputProgress(AS_File::Size pos, AS_File::Size totalSize);
extern std::string GetACPName(const AS_File::Path & filenamebuf);
#if !defined (__bcm__)
extern std::wstring GetWideName(const AS_File::Path & filenamebuf);
#endif
bool IsDirectoryPath(const AS_File::Path & fullPath)
{

#if defined (_WIN32)

	AppFindData findData;
	HANDLE fileHandle;

	fileHandle = AppFindFirstFile((AppChar*)fullPath.data, &findData);

	if (fileHandle == INVALID_HANDLE_VALUE) 
	{
		printf("Failed to find file %s\n",(UInt8*)fullPath.data);
		return false;
	}
	FindClose(fileHandle);
	if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
	//	std::cout << "directory name is: " << fullPath << std::endl;

		return true;
	}
	else
	{
	//	std::cout << "file name is: " << fullPath << std::endl;
		
		return false;
	}
#else
	struct stat _stat;
	if (
		stat(GetACPName(fullPath).c_str(), &_stat) == -1
		&& lstat(GetACPName(fullPath).c_str(), &_stat) == -1) // broken symlink?
	{
		std::cout << "Problem getting path info for: " << GetACPName(fullPath)<< std::endl;
		return false;
	}
	else
        return S_ISDIR(_stat.st_mode);




#endif



}

UInt32 GetEntryPermissions(const AS_File::Path & pathName)
{

	UInt32 filePerms = 0;
#if defined (_WIN32)

	struct __stat64 statInfo;
	if (_wstat64(GetWideName(pathName).c_str(), &statInfo) == -1)
		std::cout << "Problem getting path info for: " << GetACPName(pathName) << std::endl;

#elif defined(OSXPORT)
	struct stat statInfo;
	if (
		stat(GetACPName(pathName).c_str(), &statInfo) == -1
		&& lstat(GetACPName(pathName).c_str(), &statInfo) == -1) // broken symlink?
		std::cout << "Problem getting path info for: " << GetACPName(pathName) << std::endl;

#elif defined (__unix__) || defined (__mipsel__)
	struct stat64 statInfo;
	if (
		stat64(GetACPName(pathName).c_str(), &statInfo) == -1
		&& lstat64((char*)pathName.data, &statInfo) == -1) // broken symlink?
		std::cout << "Problem getting path info for: " << GetACPName(pathName) << std::endl;

#elif
	struct stat statInfo;
	if (
		stat(GetACPName(pathName).c_str(), &statInfo) == -1
		&& lstat(GetACPName(pathName).c_str(), &statInfo) == -1) // broken symlink?
		std::cout << "Problem getting path info for: " << GetACPName(pathName) << std::endl;

#endif
#if !defined (_WIN32)
	if (statInfo.st_mode & S_IRUSR)
		filePerms = OWNER_Read;
	if (statInfo.st_mode & S_IXUSR)
		filePerms |= OWNER_Execute;
	if (statInfo.st_mode & S_IRGRP)
		filePerms |= GROUP_Read;
	if (statInfo.st_mode & S_IXGRP)
		filePerms |= GROUP_Execute;
	if (statInfo.st_mode & S_IROTH)
		filePerms |= OTHER_Read;
	if (statInfo.st_mode & S_IXOTH)
		filePerms |= OTHER_Execute;
    if (statInfo.st_mode & S_IWUSR)
        filePerms |= OWNER_Write|OWNER_ChAttr|OWNER_Delete;
    if (statInfo.st_mode & S_IWGRP)
        filePerms |= GROUP_Write|OWNER_ChAttr|OWNER_Delete;
    if (statInfo.st_mode & S_IWOTH)
        filePerms |= OTHER_Write|OTHER_ChAttr|OTHER_Delete;
#else
	// TODO (WIN32):  Add in ways to get group and other from stat info
	if (statInfo.st_mode & S_IREAD)
		filePerms = OWNER_Read;
	if (statInfo.st_mode & S_IEXEC)
		filePerms |= OWNER_Execute;
	if (statInfo.st_mode & S_IWRITE)
		filePerms |= OWNER_Write|OWNER_ChAttr|OWNER_Delete;

#endif

	return filePerms;


}
bool DoPermissionsMatch(const AS_Volume & vol, const AS_File::Path & srcPath, const AS_File::Path & dstPath, UInt32 srcCompareMethod, UInt32 dstCompareMethod)
{



	bool matched = true;

	UInt32 srcPerms, dstPerms = 0;
	UInt32 retsz = 0;

	AS_StorageError err;
	AS_Volume::FS_Type format;
	if (srcCompareMethod == 0)
	{
		// currently we only support UDF for AS_STorage method, so query FS and only allow for UDF FS on media
		err = AS_Volume_GetProperty(vol, AS_Volume::Prop_Format_Type, sizeof(format), &format, &retsz);
		if (format > AS_Volume::FS_Joliet)
		{
			err = AS_Volume_GetEntryProperty(vol, srcPath, AS_File::Prop_UdfPermissions, sizeof(srcPerms), &srcPerms, &retsz); 
			if (err != AS_StorageError_None)
				return false;
		}
		else
			matched = true;    // just set to true for now if not UDF

	}
	else
	{
		srcPerms = 	GetEntryPermissions(srcPath);
	}

	if (dstCompareMethod == 0)
	{
		// check if fs is udf
		err = AS_Volume_GetProperty(vol, AS_Volume::Prop_Format_Type, sizeof(format), &format, &retsz);
		if (format > AS_Volume::FS_Joliet)
		{
			err = AS_Volume_GetEntryProperty(vol, dstPath, AS_File::Prop_UdfPermissions, sizeof(dstPerms), &dstPerms, &retsz); 
			if (err != AS_StorageError_None)
				return false;
		}
		else
			matched = true;    // just set to true for now if not UDF

	}
	else
	{
		dstPerms = GetEntryPermissions(dstPath);

	}

	if (dstPerms == srcPerms)
		matched = true;
	else
	{
		std::cout << "failed to match permissions " << std::endl;
		// TODO: tell me what failed 


		matched = false;

	}

	return matched;

}

AS_StorageError FileRead(const AppHandle & hdl,
	void * buffer,
	size_t bytes)
{
	size_t totalBytes = bytes;
	while (bytes > 0)
	{
		UInt32 bytesRead=0;
#if defined (_WIN32)
		DWORD dwBytes = bytes > 0xffffffff ? 0xffffffff : static_cast<DWORD>(bytes);
		DWORD dwBytesRead;
		if (!ReadFile(hdl, buffer, dwBytes, &dwBytesRead, 0))
			return AS_StorageError_Fatal;
		else if (dwBytes != dwBytesRead)
		{
			DWORD readErr = GetLastError();
			return AS_StorageError_Fatal;
		}
		bytesRead = static_cast<UInt32>(dwBytesRead);
#else
		if (read(hdl, buffer, bytes) == -1)
			return AS_StorageError_Fatal;

		bytesRead = static_cast<UInt32>(bytes);

#endif

		buffer = static_cast<UInt8 *>(buffer) + bytesRead;
		bytes -= bytesRead;

		OutputProgress((totalBytes - bytes), totalBytes);
	}
	return AS_StorageError_None;
}

AS_StorageError FileWrite(const AppHandle & hdl,
	const void * buffer,
	size_t bytes)
{
	size_t totalBytes = bytes;
	while (bytes > 0)
	{
		UInt32 bytesWritten=0;
#if defined (_WIN32)
		DWORD dwBytes = bytes > 0xffffffff ? 0xffffffff : static_cast<DWORD>(bytes);
		DWORD dwBytesWritten;
		if (!WriteFile(hdl, buffer, dwBytes, &dwBytesWritten, 0))
			return AS_StorageError_Fatal;
		else if (dwBytes != dwBytesWritten)
			return AS_StorageError_Fatal;
		bytesWritten = static_cast<UInt32>(dwBytesWritten);
#else
		if (write(hdl, buffer, bytes) == -1)
			return AS_StorageError_Fatal;

		bytesWritten = static_cast<UInt32>(bytes);

#endif

		buffer = static_cast<const UInt8 *>(buffer) + bytesWritten;
		bytes -= bytesWritten;

	}
	return AS_StorageError_None;
}

AS_StorageError CreateDirectoryOnHD(const AS_File::Path & dstpath) 
{
	AS_StorageError err = AS_StorageError_None;

#if defined _WIN32
	WIN32_FILE_ATTRIBUTE_DATA data;
	if (GetFileAttributesExW((wchar_t*)(dstpath.data), GetFileExInfoStandard, &data) != 0)
		return AS_StorageError_FileExists;
		
	if (!CreateDirectoryW((wchar_t*)(dstpath.data), NULL))
	{
		printf("Failed to create dir: %s\n",GetACPName(dstpath).c_str());
		return AS_StorageError_Fatal;
	}
	else
	{
		printf("Succeeded to create dir: %s\n",GetACPName(dstpath).c_str());
	}
#else
	const mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH;
	if (mkdir(GetACPName(dstpath).c_str(), mode) == -1)
		return AS_StorageError_Fatal;
	else
	{
		printf("Succeeded to create dir: %s\n",GetACPName(dstpath).c_str());
	}
#endif

	return err;
}



AS_StorageError OpenReadOnlyFileOnHD(const AS_File::Path & dstpath, AppHandle & hdl)
{
#if defined _WIN32
	DWORD access = GENERIC_READ;
	DWORD share = FILE_SHARE_READ;

	// open read only
	hdl =
		CreateFileW(
			GetWideName(dstpath).c_str(),
			access,
			share,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if (!hdl)
	{
		printf("Failed to open file: %s\n",GetACPName(dstpath).c_str());
		return AS_StorageError_FileNotFound;
	}
#ifdef _DEBUG
	else
		printf("Succeeded to open file: %s\n",GetACPName(dstpath).c_str());
#endif
#else
	const mode_t omode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // no execute permissions
	const int oflag = O_RDONLY;
	hdl = open(GetACPName(dstpath).c_str(), oflag, omode);
	if (hdl == -1)
		return AS_StorageError_FileNotFound;
#ifdef _DEBUG
	else
		printf("Succeeded to open file: %s\n",GetACPName(dstpath).c_str());
#endif
#endif
	return AS_StorageError_None;


}


AS_StorageError CreateFileOnHD(const AS_File::Path & dstpath, AppHandle & hdl)
{
#if defined _WIN32
	DWORD access = GENERIC_READ |GENERIC_WRITE;
	DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE;
	if (
		share == 0 // share not specified?
		&& access == GENERIC_READ) // read-only access?
		share = FILE_SHARE_READ; // share read access for compatibility

	hdl =
		CreateFileW(
			GetWideName(dstpath).c_str(),
			access,
			share,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

	if (!hdl)
	{
		printf("Failed to create file: %s\n",GetACPName(dstpath).c_str());
		return AS_StorageError_Fatal;
	}
#ifdef _DEBUG
	else
		printf("Succeeded to create file: %s\n",GetACPName(dstpath).c_str());
#endif
#else
	const mode_t omode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // no execute permissions
	const int oflag = O_CREAT | O_RDWR;
	hdl = open(GetACPName(dstpath).c_str(), oflag, omode);
	if (hdl == -1)
		return AS_StorageError_Fatal;
#ifdef _DEBUG
	else
		printf("Succeeded to create file: %s\n",GetACPName(dstpath).c_str());
#endif
#endif
	return AS_StorageError_None;


}

AS_StorageError CloseFileOnHD(AppHandle & hdl)
{
#if defined _WIN32
	if (!CloseHandle(hdl))
		return AS_StorageError_Fatal;
#else
	if (close(hdl) == -1)
		return AS_StorageError_Fatal;
#endif
	hdl = 0;
	return AS_StorageError_None;


}

UInt64 GetFileSize(const AS_File::Path & srcPath)
{

	UInt64 sizeofFile = 0;
	FILE* fptr = AppFileOpen((AppChar*)(srcPath.data), _APP("rb"));

	if (fptr)
	{
#if defined (_WIN32)
		int filenum = 0;
		filenum = _fileno(fptr);
		_lseeki64(filenum, 0, SEEK_END);
		
		fpos_t pos;
		fgetpos(fptr, &pos);
		sizeofFile = pos;
		pos = 0;
		fsetpos(fptr, &pos);
#elif defined (OSXPORT)
		fseeko(fptr, 0, SEEK_END);
		sizeofFile = (size_t)ftello(fptr);
		fseeko(fptr, 0, SEEK_SET);
#else
		fseeko64(fptr, 0, SEEK_END);
		sizeofFile = (size_t)ftello64(fptr);
		fseeko64(fptr, 0, SEEK_SET);
#endif

		fclose(fptr);

	}

	return sizeofFile;


}

bool FileExists(const AS_File::Path & filePath)
{

   	AppHandle hdl;
	AS_StorageError err = OpenReadOnlyFileOnHD(filePath, hdl);
	if (hdl != AppHandle(-1) && err != AS_StorageError_FileNotFound)
	{
		CloseFileOnHD(hdl);
		return true;
	}

	return false;


}

AS_StorageError OpenReadOnly(const AS_Volume & vol, const AS_File::Path & path, UInt32 method, UInt64 * fileSize, UInt32* openIndex)
{

	AS_StorageError err;
	switch (method)
	{

	case 0:
		// AS_Storage
		{

			AS_File* newASFile = new AS_File();
			err = AS_Volume_OpenFile(vol, path, AS_File::Read, *newASFile);
			if (err != AS_StorageError_None)
				return err;

			AS_File::Size sizeOfFile = 0;
			UInt32 outDataSize=0;
			err = AS_File_GetProperty(*newASFile, AS_File::Prop_Size, sizeof(sizeOfFile), &sizeOfFile, &outDataSize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;

			*fileSize = sizeOfFile;

			*openIndex = (UInt32)readASFileList.size();

			readASFileList.push_back(newASFile);			

		}
		break;
	case 1:
		{

#if !defined (__bcm__)
			AppIfStream* readStream = new AppIfStream();

			//readStream->open((AppChar*)(path.data), std::ios::in | std::ios::binary);
			readStream->open((const char*)(path.data), std::ios::in | std::ios::binary);
			*fileSize = GetFileSize(path);
			*openIndex = (UInt32)readStreamlist.size();

			readStreamlist.push_back(readStream);
#endif
		}
		break;
	case 2:
		{
			AppHandle * hdl = new AppHandle;

			err = OpenReadOnlyFileOnHD(path, *hdl);
			if (err != AS_StorageError_None)
				return err;

			*fileSize = GetFileSize(path);
			*openIndex = (UInt32)readCFileList.size();

			readCFileList.push_back(hdl);

		} 
		break;
	};

	return AS_StorageError_None;

}

AS_StorageError ReadFileByMethod(UInt32 index, UInt32 method, void* buffer, std::streamsize bytesToRead, UInt64 fileSz)
{

	AS_StorageError err = AS_StorageError_None;
	switch (method)
	{
	case 0:
		{
			std::vector<AS_File*>::const_iterator itr = readASFileList.begin() + index;
			// use AS_File functions
			AS_File::Count bytesRead = 0;
			AS_File::Count bufferSize = 32768;
			// check where we're at now
			UInt64 curPos, totalBytesRead = 0;
			UInt32 retSz = 0;

			while (totalBytesRead < bytesToRead)
			{

				AS_File_GetProperty(*(*itr), AS_File::Prop_CurrentPos, sizeof(curPos), &curPos, &retSz);
				if (bufferSize > (fileSz - curPos))
					bufferSize = (AS_File::Count)(fileSz - curPos);

				err = AS_File_Read(*(*itr), bufferSize, buffer, &bytesRead);
				if (bufferSize != bytesRead)
					return AS_StorageError_ReadError;

				totalBytesRead += bufferSize;
				OutputProgress(totalBytesRead, bytesToRead);

			}

			return err;

		}
		break;
	case 1:
		// std c++ ifstream to read
		{
#if !defined (__bcm__)
			std::vector<AppIfStream*>::const_iterator readStream = readStreamlist.begin() + index;
			if (!(*readStream)->is_open())
				return AS_StorageError_FileNotFound;

			std::streamsize bytes = 0;
			std::streamsize bufferSize = 32768;

			std::streamsize curPos = (*readStream)->tellg();
			if (bufferSize > (std::streamsize)fileSz - curPos)
				bufferSize = (std::streamsize)fileSz - curPos;
			
			while (bytes != bytesToRead)
			{
				
				(*readStream)->read((AppChar*)buffer, bufferSize);

				bytes+= bufferSize;
				OutputProgress(bytes, bytesToRead);
			}
#endif
		} 
		break;
	case 2:
		{
			std::vector<AppHandle*>::const_iterator AppHdl = readCFileList.begin() + index;
			AppHandle* hdl = *AppHdl;
			AS_StorageError err = FileRead(*hdl, buffer, bytesToRead);
		}
		break;
	};

	return AS_StorageError_None;

}



AS_StorageError CloseFile(UInt32 index, UInt32 method)
{

	switch (method)
	{

	case 0:

		{
			std::vector<AS_File*>::const_iterator readASFileHandle = readASFileList.begin() + index;
			AS_File_Close((*(*readASFileHandle)));
			readASFileList.erase(readASFileList.begin() + index);
		}
		break;
	case 1:
		{
#if !defined (__bcm__)

			std::vector<AppIfStream*>::const_iterator readStream = readStreamlist.begin() + index;
			if (!(*readStream)->is_open())
				return AS_StorageError_FileNotFound;
			(*readStream)->close();

			readStreamlist.erase(readStreamlist.begin() + index);
#endif
		}

		break;
	case 2:
		{
			std::vector<AppHandle*>::const_iterator AppHdl = readCFileList.begin() + index;
			AppHandle * hdl = *AppHdl;
			CloseFileOnHD(*hdl);
			readCFileList.erase(readCFileList.begin() + index);
		}
		break;
	};

	return AS_StorageError_None;
}


AS_StorageError CopyFileToHD(const AS_Volume & vol, const AS_File::Path & srcpath, const AS_File::Path & dstpath)
{
	AS_StorageError err = AS_StorageError_None;

	AS_File srcFile;
	AppHandle hdl;
	CreateFileOnHD(dstpath, hdl);
	
	AS_File::Size sizeOfFile;
	AS_File::Size curPos = 0;
	AS_File::Count numRead;

	UInt32 outDataSize=0;


	err = AS_Volume_OpenFile(vol, srcpath, AS_File::Read, srcFile);
	if (err != AS_StorageError_None)
		return err;

	err = AS_File_GetProperty(srcFile, AS_File::Prop_Size, sizeof(sizeOfFile), &sizeOfFile, &outDataSize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	while (curPos < sizeOfFile)
	{
		AS_File::Size readSize = 32768;
		if ((sizeOfFile - curPos) < readSize)
			readSize = sizeOfFile - curPos;

	    char * readBuffer = new char[(UInt32)readSize];

		err = AS_File_Read(srcFile, (AS_File::Count)readSize, readBuffer, &numRead);
		if (err != AS_StorageError_None)
		{
			printf("Failed to read from this file\n");
			curPos = sizeOfFile;
			break;
		}

		err = FileWrite(hdl, readBuffer, numRead);

		delete[] readBuffer;

		curPos += readSize;

		OutputProgress(curPos, sizeOfFile);
	
	}

	CloseFileOnHD(hdl);
	return AS_StorageError_None;

}

AS_StorageError VerifyFile(const AS_Volume & vol, const AS_File::Path & srcPath, const AS_File::Path & dstPath, UInt32 srcCompareMethod, UInt32 dstCompareMethod, bool verifyContents, bool verifyPermissions)
{
	AS_StorageError err = AS_StorageError_None;

	UInt64 srcFileSize, dstFileSize = 0;

	UInt32 openSrcIndex, openDstIndex = 0;
	//open src in preferred mode
	err = OpenReadOnly(vol, srcPath, srcCompareMethod, &srcFileSize, &openSrcIndex);  // set to use method 2 - std::ifstream to read from hdd
	if (err != AS_StorageError_None)
		return err;


	// open dest in preferred mode

	err = OpenReadOnly(vol, dstPath, dstCompareMethod, &dstFileSize, &openDstIndex);
	if (err != AS_StorageError_None)
		return err;

	AS_File::Size curPos = 0;

	if (srcFileSize != dstFileSize)
		std::cout << "Matching file sizes failed: Src Size = " << srcFileSize << " Dest Size is = " << dstFileSize << std::endl;

	bool matchedPermissions = true;
	if (verifyPermissions)
		matchedPermissions = DoPermissionsMatch(vol, srcPath, dstPath, srcCompareMethod, dstCompareMethod);
	
	if (verifyContents)
	{
		while (curPos < srcFileSize &&  err != AS_StorageError_DataMiscompare)
		{
			AS_File::Size readSize = 32768;
			if ((srcFileSize - curPos) < readSize)
				readSize = srcFileSize - curPos;

			char * readBuffer = new char[(UInt32)readSize];

			char* readBufferHDD = new char[(UInt32)readSize];

			err = ReadFileByMethod(openDstIndex, dstCompareMethod, readBuffer, (std::streamsize)readSize, dstFileSize);
			if (err != AS_StorageError_None)
			{
				printf("Failed to read from this file\n");
				curPos = srcFileSize;
				break;
			}
			err = ReadFileByMethod(openSrcIndex, srcCompareMethod, readBufferHDD, (std::streamsize)readSize, srcFileSize);

			// compare buffer in memory
			if (memcmp(readBuffer, readBufferHDD, (size_t)readSize) != 0)
			{
			
				delete[] readBuffer;
				delete[] readBufferHDD;
				CloseFile(openSrcIndex, srcCompareMethod);  
				CloseFile(openDstIndex, dstCompareMethod);  
				// return compare error from here
				return AS_StorageError_DataMiscompare;

			}

			delete[] readBuffer;
			delete[] readBufferHDD;

			curPos += readSize;

			OutputProgress(curPos, srcFileSize);
		
		}
	}
	
	CloseFile(openSrcIndex, srcCompareMethod);  
	CloseFile(openDstIndex, dstCompareMethod);  

	return err;

}
AS_StorageError VerifyDirectory(const AS_Volume & vol, const AS_File::Path & srcPath, const AS_File::Path & dstPath, UInt32 srcCompareMethod=0, UInt32 dstCompareMethod=0, bool verifyPermissions = false)
{
	AS_StorageError err = AS_StorageError_None;

	bool matchedDirs = true;
	bool matchedPermissions = true;

	if (verifyPermissions)
		matchedPermissions = DoPermissionsMatch(vol, srcPath, dstPath, srcCompareMethod, dstCompareMethod);
	
	if (dstCompareMethod == 0)
	{

		AS_File::Kind kind;
		AS_File::Size size = 0;
		err = AS_Volume_GetEntryInfo(vol, dstPath, kind, size);
		if (kind != AS_File::File_IsDir)
			matchedDirs = false;

		// get volume prop for permissions
		verifyPermissions;
	}
	else
	{
		if (!IsDirectoryPath(dstPath))
			matchedDirs = false;

		// get win32 api permissions on dir
		verifyPermissions;
	}

	if (srcCompareMethod == 0)
	{
		AS_File::Kind kind;
		AS_File::Size size = 0;
		err = AS_Volume_GetEntryInfo(vol, srcPath, kind, size);
		if (kind != AS_File::File_IsDir)
			matchedDirs = false;

		
		// get volume prop for permissions
		verifyPermissions;
	}
	else
	{
		if (!IsDirectoryPath(srcPath))
			matchedDirs = false;
		// get win32 api permissions on dir
		verifyPermissions;
	}

	if (!(matchedDirs && matchedPermissions))
		return AS_StorageError_DataMiscompare;

	return err;


}

AS_StorageError CreateEntryOnHD(const AS_Volume & vol, const AS_File::Kind & kind, const AS_File::Path & srcpath, const AS_File::Path & dstpath)
{
	AS_StorageError err = AS_StorageError_None;
	if (kind == AS_File::File_IsDir)
		err = CreateDirectoryOnHD(dstpath);
	else
		err = CopyFileToHD(vol, srcpath, dstpath);
	
	return err;
}

UInt32 GetFileList(std::vector<AS_String>& list, const AS_File::Path & dirPath, const AS_File::Path & extension)
{
	UInt32 Count = 0;

// expect wide chars for win32
#ifdef WIN32
	AppFindData findData;
	HANDLE fileHandle;
	int flag = 1;

	AppStdString search = _APP("*.");
	AppStdString newDirPath;
	AppStdString noPath = _APP("");
	AppStdString ext((AppChar*)extension.data);
	if (memcmp((AppChar*)dirPath.data,noPath.c_str(), sizeof(AppChar)) == 0) 
		newDirPath = AppStdString(_APP("."));
	else
		newDirPath = AppStdString((AppChar*)(dirPath.data));
	
	newDirPath += _APP("\\");

	AppStdString newSearch;
	// check for overflow
	size_t expectedSize = newDirPath.size()+ext.size()+search.size();
	if (newSearch.size() > expectedSize)
		return 0;

	newSearch = newDirPath + search + ext;
	newSearch[expectedSize] = '\0';

	fileHandle = AppFindFirstFile(newSearch.c_str(), &findData);

	if (fileHandle == INVALID_HANDLE_VALUE) return Count;

	AS_String newFileString;

	while (flag)
	{
		if ((memcmp(findData.cFileName,".", 1*sizeof(AppChar)) != 0) &&
			(memcmp(findData.cFileName,"..", 2*sizeof(AppChar)) != 0))/* &&
         (findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)*/
		{
			const size_t maxChars = 512;
			AppStdString newFullPath = newDirPath + AppStdString(findData.cFileName);
			AppChar * newStrPath = new AppChar[maxChars];
			memcpy(newStrPath, newFullPath.c_str(), newFullPath.size()*sizeof(newFullPath[0])+1);
			newStrPath[newFullPath.size()] = '\0';			
			newFileString.data = newStrPath;
			newFileString.size = maxChars;
			newFileString.type = AppStrType;
			list.push_back(newFileString);
			Count++;
		}
		flag = AppFindNextFile(fileHandle, &findData);
	}
	FindClose(fileHandle);
// expect char* for non-win32
#else

	std::string search(".");
	std::string newDirPath((char*)dirPath.data);
	search += std::string((char*)extension.data);
	DIR* dir = opendir(newDirPath.c_str());
	struct dirent* entry;
	std::string temp;
	newDirPath += std::string("/");

	if (dir == NULL)
		return 0;

	while ((entry = readdir(dir)) != NULL)
	{
		if ((strcmp(entry->d_name,".") != 0) &&
			(strcmp(entry->d_name,"..") != 0))
		{

			temp = newDirPath + std::string(entry->d_name);
			struct stat x;
			stat(entry->d_name, &x); 
			if (!S_ISDIR(x.st_mode))
			//if (temp.substr(0, search.size()) == search)
			{
            const size_t maxChars = 512;
				AS_String newFilePath;
				char* newStrPath = new char[maxChars];
				memcpy(newStrPath, temp.c_str(), temp.size());
				newStrPath[temp.size()] = '\0';
				newFilePath.type = AS_String::Type_ASCII;
				newFilePath.data = newStrPath;
				newFilePath.size = temp.size();

				list.push_back(newFilePath);
				Count++;
			}
		}
	}
	closedir(dir);
#endif

	return Count;

}

