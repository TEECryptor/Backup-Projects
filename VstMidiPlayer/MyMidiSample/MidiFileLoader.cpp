//--------------------------------------------------
//	File Name: MidiFileLoader.h
//	Introduce: The implement file of class CMidiFileLoader
//	Date: 2010-05-17
//--------------------------------------------------
#include "stdafx.h"
#include "MidiFileLoader.h"
//--------------------------------------------------
CMidiFileLoader::CMidiFileLoader(void)
 : m_dwFormat(1)
 , m_dwTrackCount(0)
 , m_dwTimeDivision(0)
 , m_dwCurrentTempo(0)
 , m_dwDuration(0)
{
}
//--------------------------------------------------
CMidiFileLoader::~CMidiFileLoader(void)
{
	ClearMidiData();
}
//--------------------------------------------------


//--------------------------------------------------
BOOL CMidiFileLoader::LoadMidiFile(LPCTSTR lpszFileName)
{
	OFSTRUCT os;
	memset(&os, 0, sizeof(OFSTRUCT));
	os.cBytes = sizeof(OFSTRUCT);
	HANDLE hMidiFile = CreateFile(	lpszFileName, 
									GENERIC_READ, 
									FILE_SHARE_READ, 
									NULL,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									NULL);
	if (NULL == hMidiFile)
		return FALSE;

	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hMidiFile, NULL);
	if (dwFileSize == 0)
		return FALSE;

	LPBYTE lpMidiFileData = new BYTE[dwFileSize];
	
	DWORD dwReadedBytes = 0;
	if (!ReadFile(hMidiFile, lpMidiFileData, dwFileSize, &dwReadedBytes, NULL) ||
		dwReadedBytes != dwFileSize)
	{
		CloseHandle(hMidiFile);
		delete []lpMidiFileData;
		return FALSE;
	}
	CloseHandle(hMidiFile);

	// Parse all data in midi file
	BOOL bRes = ParseMidiData(lpMidiFileData, dwFileSize);
	delete []lpMidiFileData;
	lpMidiFileData = NULL;

	return bRes;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetMidiFormat()
{
	return m_dwFormat;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetTimeDivision()
{
	return m_dwTimeDivision;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetTempo()
{
	return m_dwCurrentTempo;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetDuration()
{
	return m_dwDuration;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetTrackCount()
{
	return m_dwTrackCount;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetEventCount()
{
	return (DWORD)m_paEvents.size();
}
//--------------------------------------------------
void CMidiFileLoader::GetEvents(LPMIDIEVENT lpEvents, 
								DWORD &dwCount)
{
	DWORD dwCopiedCnt = 0;
	DWORD dwCurrentCnt = 0;
	for (PtrEventArray::iterator it = m_paEvents.begin();
		it != m_paEvents.end() && dwCurrentCnt < dwCount;
		it++, dwCurrentCnt++)
	{
		LPMIDIEVENT lpEvent = *it;
		lpEvents[dwCurrentCnt].dwTime = lpEvent->dwTime;
		lpEvents[dwCurrentCnt].dwEventLength = lpEvent->dwEventLength;
		memcpy(lpEvents[dwCurrentCnt].byShortData, lpEvent->byShortData, 4*sizeof(BYTE));
		if (lpEvents[dwCurrentCnt].dwEventLength > 0 && lpEvent->pLongData)
		{
			lpEvents[dwCurrentCnt].pLongData = new BYTE[lpEvents[dwCurrentCnt].dwEventLength];
			memcpy(lpEvents[dwCurrentCnt].pLongData, lpEvent->pLongData, lpEvents[dwCurrentCnt].dwEventLength*sizeof(BYTE));
		}
	}

	dwCount = dwCurrentCnt;
}
//--------------------------------------------------
DWORD CMidiFileLoader::GetMetaEventCount()
{
	return 0;
}
//--------------------------------------------------
void CMidiFileLoader::GetMetaEvents(LPMIDIEVENT lpEvents, 
									DWORD &dwCount)
{
}
//--------------------------------------------------



//--------------------------------------------------
BOOL CMidiFileLoader::ParseMidiData(LPBYTE lpMidiData, DWORD dwByteSize)
{
	if (NULL == lpMidiData || dwByteSize <= 0)
		return FALSE;

	register LPBYTE p = LPBYTE(lpMidiData);

	ClearMidiData();

	// Check header of MIDI
	if(*(DWORD*)p != MThd) {
		ASSERT(FALSE);
		return FALSE;
	}
	p += sizeof(DWORD);

	// Check header size
	DWORD dwHeaderSize = DWORDSWAP(*(DWORD*)p);
	if( dwHeaderSize != sizeof(MidiFileHeader) ) {
		ASSERT(FALSE);
		return FALSE;
	}
	p += sizeof(DWORD);

	// Get header
	MidiFileHeader stuMidiHeader;
	::CopyMemory(&stuMidiHeader, p, dwHeaderSize);
	m_dwFormat = DWORD(WORDSWAP(stuMidiHeader.wFormat));
	m_dwTrackCount = DWORD(WORDSWAP(stuMidiHeader.wTrackCount));
	m_dwTimeDivision = DWORD(WORDSWAP(stuMidiHeader.wTimeDivision));
	p += dwHeaderSize;

	// Create the array of tracks
	LPMIDITRACK lpTracks = new MidiTrack[m_dwTrackCount];
	for(register DWORD i = 0; i < m_dwTrackCount; ++i) 
	{
		// Check header of track
		if(*(DWORD*)p != MTrk) 
		{
			ASSERT(FALSE);
			return FALSE;
		}
		p += sizeof(DWORD);

		// Track bytes length
		lpTracks[i].dwTrackLength = DWORDSWAP(*(DWORD*)p);
		p += sizeof(DWORD);

		// Track start and current buffer pointers
		lpTracks[i].pTrackStart = lpTracks[i].pTrackCurrent = p;
		p += lpTracks[i].dwTrackLength;

        // Handle bozo MIDI files which contain empty track chunks
		if( !lpTracks[i].dwTrackLength ) {
			//m_Tracks[i].fdwTrack |= ITS_F_ENDOFTRK;
			continue;
		}

		// We always preread the time from each track so the mixer code can
		// determine which track has the next event with a minimum of work
		if( !GetTrackVDWord( &lpTracks[i], &lpTracks[i].dwNextEventTime )) 
		{
			TRACE0("Error in MIDI data\n");
			ASSERT(FALSE);
			return FALSE;
		}
	}

	// Parse tracks data to events
	BOOL bRes = ParseMidiTracks(lpTracks, m_dwTrackCount);
	delete []lpTracks;
	lpTracks = NULL;

	return bRes;
}
//--------------------------------------------------
BOOL CMidiFileLoader::ParseMidiTracks(LPMIDITRACK lpTracks, DWORD dwTrackCount)
{
	if (NULL == lpTracks || 0 == dwTrackCount)
		return FALSE;

	int iEventCnt = 0;
	while(1)
	{
		LPMIDITRACK lpFoundTrack = NULL;
		DWORD dwNextTrackTime = 0xFFFFFFFFL;

		// Find nearest event due
		for (register DWORD dwIndex = 0; dwIndex < dwTrackCount; ++dwIndex) 
		{
			if (FALSE == lpTracks[dwIndex].bIsAtEnd && 
				lpTracks[dwIndex].dwNextEventTime < dwNextTrackTime) 
			{
				dwNextTrackTime = lpTracks[dwIndex].dwNextEventTime;
				lpFoundTrack = &lpTracks[dwIndex];
			}
		}
		
		// None found?  We must be done, so return to the caller with a smile.
		if (NULL == lpFoundTrack) 
		{
			return TRUE; //CONVERTERR_NOERROR;
		}

		// Update the total duration (in mircosecnd)
		m_dwDuration = lpFoundTrack->dwNextEventTime * (m_dwCurrentTempo/m_dwTimeDivision);

		LPMIDIEVENT lpNewEvent = new MidiEvent;

		// Ok, get the event header from that track
		if (!GetTrackEvent(lpFoundTrack, lpNewEvent)) 
		{
			// Warn future calls that this converter is stuck at a corrupt spot
			// and can't continue
			//m_dwStatus |= CONVERTF_STATUS_STUCK;
			delete lpNewEvent;
			return FALSE; //CONVERTERR_CORRUPT;
		}

		iEventCnt++;

		// Don't add end of track event 'til we're done
		if (lpNewEvent->byShortData[0] == MIDI_META && 
			lpNewEvent->byShortData[1] == MIDI_META_EOT) 
		{
			delete lpNewEvent;
			continue;
		}

		// Channel message.
		if (lpNewEvent->byShortData[0] < MIDI_SYSEX) 
		{
		}
		// SysEx event, ignore it.
		else if (	lpNewEvent->byShortData[0] == MIDI_SYSEX || 
					lpNewEvent->byShortData[0] == MIDI_SYSEXEND)
		{
			//delete lpNewEvent;
			//continue;
		}
		// Meta event, the only meta-event we care about is change tempo
		else
		{
			if (lpNewEvent->byShortData[1] == MIDI_META_TEMPO ) 
			{
				m_dwCurrentTempo =	lpNewEvent->pLongData[2] | 
									((DWORD)lpNewEvent->pLongData[1] << 8) |
									((DWORD)lpNewEvent->pLongData[0] << 16 );
			}
			delete lpNewEvent;
			continue;
		}

		// Add to event list
		m_paEvents.push_back(lpNewEvent);

	};

	return TRUE;
}
//--------------------------------------------------
BOOL CMidiFileLoader::GetTrackEvent(LPMIDITRACK lpTrack, LPMIDIEVENT lpEvent)
{
	if (NULL == lpTrack || NULL == lpEvent)
		return FALSE;

	DWORD   idx;
	UINT    dwEventLength;

	// Clear out the temporary event structure to get rid of old data...
	memset(lpEvent, 0, sizeof(MidiEvent));

	// Already at end of track? There's nothing to read.
	if (lpTrack->bIsAtEnd)
		return FALSE;

	// Get the first byte, which determines the type of event.
	BYTE byByte;
	if (!GetTrackByte(lpTrack, &byByte))
		return FALSE;

	// If the high bit is not set, then this is a channel message
	// which uses the status byte from the last channel message
	// we saw. NOTE: We do not clear running status across SysEx or
	// meta events even though the spec says to because there are
	// actually files out there which contain that sequence of data.
	if (!(byByte & 0x80)) 
	{
		// No previous status byte? We're hosed.
		if (!lpTrack->byRunningStatus) 
		{
			//TrackError(lpTrack, gteBadRunStat);
			return FALSE;
		}

		lpEvent->byShortData[0] = lpTrack->byRunningStatus;
		lpEvent->byShortData[1] = byByte;

		byByte = lpEvent->byShortData[0] & 0xF0;
		lpEvent->dwEventLength = 2;

		// Only program change and channel pressure events are 2 bytes long;
		// the rest are 3 and need another byte
		if ((byByte != MIDI_PRGMCHANGE) && (byByte != MIDI_CHANPRESS)) 
		{
			if (!GetTrackByte(lpTrack, &lpEvent->byShortData[2]))
				return FALSE;
			++lpEvent->dwEventLength;
		}
	} 
	else if ((byByte & 0xF0) != MIDI_SYSEX) 
	{
		// Not running status, not in SysEx range - must be
		// normal channel message (0x80-0xEF)
		lpEvent->byShortData[0] = byByte;
		lpTrack->byRunningStatus = byByte;

		// Strip off channel and just keep message type
		byByte &= 0xF0;

		dwEventLength = (byByte == MIDI_PRGMCHANGE || byByte == MIDI_CHANPRESS) ? 1 : 2;
		lpEvent->dwEventLength = dwEventLength + 1;

		if (!GetTrackByte(lpTrack, &lpEvent->byShortData[1]))
			return FALSE;

		if (dwEventLength == 2)
		{
			if (!GetTrackByte(lpTrack, &lpEvent->byShortData[2]))
				return FALSE;
		}
	} 
	else if ((byByte == MIDI_SYSEX) || (byByte == MIDI_SYSEXEND)) 
	{
		// One of the SysEx types. (They are the same as far as we're concerned;
		// there is only a semantic difference in how the data would actually
		// get sent when the file is played. We must take care to put the proper
		// event type back on the output track, however.)
		//
		// Parse the general format of:
		//  BYTE 	bEvent (MIDI_SYSEX or MIDI_SYSEXEND)
		//  VDWORD 	cbParms
		//  BYTE   	abParms[cbParms]
		lpEvent->byShortData[0] = byByte;
		if (!GetTrackVDWord(lpTrack, &lpEvent->dwEventLength)) 
		{
			//TrackError(lpTrack, gteSysExLenTrunc);
			return FALSE;
		}

		// Malloc a temporary memory block to hold the parameter data
		lpEvent->pLongData = new BYTE[lpEvent->dwEventLength];
		if (lpEvent->pLongData == 0) 
		{
			//TrackError(lpTrack, gteNoMem);
			return FALSE;
		}

		// Copy from the input buffer to the parameter data buffer
		for (idx = 0; idx < lpEvent->dwEventLength; idx++)
		{
			if (!GetTrackByte(lpTrack, lpEvent->pLongData + idx))
			{
				//TrackError(lpTrack, gteSysExTrunc);
				return FALSE;
			}
		}
	} 
	else if( byByte == MIDI_META ) 
	{
		// It's a meta event. Parse the general form:
		//  BYTE	bEvent	(MIDI_META)
		//  BYTE	bClass
		//  VDWORD	cbParms
		//  BYTE	abParms[cbParms]
		lpEvent->byShortData[0] = byByte;

		if (!GetTrackByte( lpTrack, &lpEvent->byShortData[1]))
			return FALSE;

		if (!GetTrackVDWord( lpTrack, &lpEvent->dwEventLength)) 
		{	
			//TrackError(lpTrack, gteMetaLenTrunc);
			return FALSE;
		}

		// NOTE: It's perfectly valid to have a meta with no data
		// In this case, dwEventLength == 0 and pLongData == NULL
		if (lpEvent->dwEventLength) 
		{		
			// Malloc a temporary memory block to hold the parameter data
			lpEvent->pLongData = new BYTE[lpEvent->dwEventLength];
			if (lpEvent->pLongData == 0) 
			{
				//TrackError(lpTrack, gteNoMem);
				return FALSE;
			}

			// Copy from the input buffer to the parameter data buffer
			for (idx = 0; idx < lpEvent->dwEventLength; idx++)
			{
				if (!GetTrackByte(lpTrack, lpEvent->pLongData + idx))
				{
					//TrackError(lpTrack, gteMetaTrunc);
					return FALSE;
				}
			}
		}

		if (lpEvent->byShortData[1] == MIDI_META_EOT)
			lpTrack->bIsAtEnd = TRUE;
	} 
	else 
	{
		// Messages in this range are system messages and aren't supposed to
		// be in a normal MIDI file. If they are, we've either misparsed or the
		// authoring software is stupid.
		return FALSE;
	}

	// Event time was already stored as the current track time (in mircosecond)
	lpEvent->dwTime = lpTrack->dwNextEventTime * (m_dwCurrentTempo/m_dwTimeDivision);

	// Now update to the next event time. The code above MUST properly
	// maintain the end of track flag in case the end of track meta is
	// missing.  NOTE: This code is a continuation of the track event
	// time pre-read which is done at the end of track initialization.
	if (FALSE == lpTrack->bIsAtEnd) 
	{
		DWORD	tkDelta;
		if (!GetTrackVDWord(lpTrack, &tkDelta))
			return FALSE;

		lpTrack->dwNextEventTime += tkDelta;
	}

	return TRUE;
}
//--------------------------------------------------
BOOL CMidiFileLoader::GetTrackByte(LPMIDITRACK lpTrack, LPBYTE lpbyByte)
{
	if (DWORD(lpTrack->pTrackCurrent - lpTrack->pTrackStart) == lpTrack->dwTrackLength)
		return FALSE;

	*lpbyByte = *lpTrack->pTrackCurrent++;

	return TRUE;
}
//--------------------------------------------------
BOOL CMidiFileLoader::GetTrackVDWord(LPMIDITRACK lpTrack, LPDWORD lpdw)
{
	if (NULL == lpTrack || NULL == lpdw)
		return FALSE;

	if (lpTrack->dwTrackLength <= 0)
		return FALSE;

	BYTE	byByte;
	DWORD	dw = 0;

	do {
		if (!GetTrackByte(lpTrack, &byByte))
			return FALSE;

		dw = ( dw << 7 ) | ( byByte & 0x7F );
	} while (byByte & 0x80);

	*lpdw = dw;

	return TRUE;
}
//--------------------------------------------------
void CMidiFileLoader::ClearMidiData()
{
	LPMIDIEVENT lpEvent = NULL;
	for (PtrEventArray::iterator it = m_paEvents.begin();
		it != m_paEvents.end();
		it++)
	{
		lpEvent = *it;
		if (NULL != lpEvent)
		{
			delete lpEvent;
			lpEvent = NULL;
		}		
	}
	m_paEvents.clear();
}
//--------------------------------------------------