//--------------------------------------------------
#include "Midifile.h"
//--------------------------------------------------
CMidiFile::CMidiFile(void)
 : m_eFormat(MIDI_SIMULTANEOUS_TRACK_FILE)
 , m_eTimingFormat(MIDI_TIMING_PPQ_TIMEBASE)
 , m_iNumberOfTracks(0)
 , m_iTimingDivision(0)
 , m_iFps(0)
 , m_iSubFrames(0)
 , m_strError("No Error!")
{
}
//--------------------------------------------------
CMidiFile::~CMidiFile(void)
{
}
//--------------------------------------------------


//--------------------------------------------------
bool CMidiFile::Open(const string &strFileName)
{
    ifstream* midiFile = new ifstream(strFileName.c_str(), ios::in | ios::binary);
	if (!midiFile || !midiFile->is_open())
	{
        m_eFormat = MIDI_FILE_NOT_LOADED;
        m_strError = "File not found or not readable.";
		return false;
	}

	// Get file size
	size_t fileSize = 0;
    midiFile->seekg(0, std::ios::end);
    fileSize = midiFile->tellg();
    midiFile->seekg(0, std::ios::beg);

    // Parse the MIDI header first.  
	// The first 14 bytes of the file.
    if (!parseHeader(getMidiBytes(midiFile, 14))) 
	{
        m_eFormat = MIDI_FILE_NOT_LOADED;
        m_strError = "Not a MIDI file.";
        return false;
    }

	int i = 0;

	// Parse all tracks data
    for (int j = 0; j < m_iNumberOfTracks; ++j) 
	{
		int iTrackByteCnt = skipToNextTrack(midiFile);
		if (iTrackByteCnt <= 0)
		{
            m_eFormat = MIDI_FILE_NOT_LOADED;
            m_strError = "File corrupted or in non-standard format?";
            return false;
        }

        // Run through the events taking them into our internal
        // representation.
        if (!parseTrack(midiFile, i, iTrackByteCnt)) 
		{
            m_eFormat = MIDI_FILE_NOT_LOADED;
            m_strError = "File corrupted or in non-standard format?";
            return (false);
        }

        ++i; // j is the source track number, i the destination
    }

    m_iNumberOfTracks = i;

    midiFile->close();

	return true;
}
//--------------------------------------------------
void CMidiFile::Close()
{
}
//--------------------------------------------------
int CMidiFile::getTimingDivision() 
{ 
	return m_iTimingDivision; 
}
//--------------------------------------------------
void CMidiFile::getSmpteFrameSpec(int &iFps, int &iSubframes) 
{ 
	iFps = m_iFps; 
	iSubframes = m_iSubFrames; 
}
//--------------------------------------------------
FileFormatType CMidiFile::getFileFormat() 
{ 
	return m_eFormat; 
}
//--------------------------------------------------
unsigned int CMidiFile::getNumberOfTracks() 
{ 
	return (int)m_paMidiTracks.size(); 
}
//--------------------------------------------------


//--------------------------------------------------
bool CMidiFile::parseHeader(const string& midiHeader)
{
    if (midiHeader.size() < 14)
        return false;

	// check MIDI header
    if (midiHeader.compare(0, 4, MIDI_FILE_HEADER) != 0) 
        return false;

	// check header size
    if (midiBytesToLong(midiHeader.substr(4, 4)) != 6L) 
        return false;

	// get MIDI file information
	m_eFormat = (FileFormatType)midiBytesToInt(midiHeader.substr(8, 2));
    m_iNumberOfTracks = midiBytesToInt(midiHeader.substr(10, 2));
	m_iTimingDivision = midiBytesToInt(midiHeader.substr(12, 2));
	m_eTimingFormat = MIDI_TIMING_PPQ_TIMEBASE;

	// don't support 
    if (m_eFormat == MIDI_SEQUENTIAL_TRACK_FILE) 
        return false;

	// file uses SMPTE timing
    if (m_iTimingDivision > 32767) 
	{
        m_eTimingFormat = MIDI_TIMING_SMPTE;
        m_iFps = 256 - (m_iTimingDivision >> 8);
        m_iSubFrames = (m_iTimingDivision & 0xff);
    }

    return true;
}
//-------------------------------------------------- 
bool CMidiFile::parseTrack(ifstream* midiFile, int &iLastTrackNum, int iTrackByteCnt)
{
	if (!midiFile || !midiFile->is_open())
		return false;

    MidiByte midiByte, metaEventCode, data1, data2;
    MidiByte eventCode = 0x80;
    std::string metaMessage;
    unsigned int messageLength;
    unsigned long deltaTime;
 //   unsigned long accumulatedTime = 0;

    // This would be a vector<TrackId> but TrackId is unsigned
    // and we need -1 to indicate "not yet used"
    std::vector<int> channelTrackMap(16, -1);

    // This is used to store the last absolute time found on each track,
    // allowing us to modify delta-times correctly when separating events
    // out from one to multiple tracks
    //
 //   std::map<int, unsigned long> trackTimeMap;

    // Meta-events don't have a channel, so we place them in a fixed
    // track number instead
    int iMetaTrack = iLastTrackNum;
	
    // Remember the last non-meta status byte (-1 if we haven't seen one)
    int runningStatus = -1;

    bool firstTrack = true;

    while (!midiFile->eof() && (iTrackByteCnt > 0)) 
	{
        deltaTime = getNumberFromMidiBytes(midiFile);

        // Get a single byte
        midiByte = getMidiByte(midiFile);
		
		// Have new event code
        if (midiByte & MIDI_STATUS_BYTE_MASK) 
		{      
            eventCode = midiByte;
            data1 = getMidiByte(midiFile);    
		}
		else
		{  
			if (runningStatus < 0)
			{
                //throw (Exception(qstrtostr(QObject::tr("Running status used for first event in track"))));
			}
            eventCode = (MidiByte)runningStatus;
            data1 = midiByte;
		}
		
        if (eventCode == MIDI_FILE_META_EVENT) // meta events
        {			
            metaEventCode = data1;
            messageLength = getNumberFromMidiBytes(midiFile);
            metaMessage = getMidiBytes(midiFile, messageLength);

            if (MIDI_TIME_SIGNATURE == metaEventCode ||
				MIDI_SET_TEMPO == metaEventCode)
            {
                //m_containsTimeChanges = true;
            }
/*
            // accumulatedTime is abs time of last event on any track;
            // trackTimeMap[trackNum] is that of last event on this track
            long gap = accumulatedTime - trackTimeMap[iMetaTrack];
            accumulatedTime += deltaTime;
            deltaTime += gap;
            trackTimeMap[iMetaTrack] = accumulatedTime;
*/
            CMidiEvent *e = new CMidiEvent(	deltaTime,
											MIDI_FILE_META_EVENT,
											metaEventCode,
											metaMessage);

            //m_midiComposition[iMetaTrack].push_back(e);
		}
		else
		{
            runningStatus = eventCode;

            CMidiEvent *midiEvent;
            int channel = (eventCode & MIDI_CHANNEL_NUM_MASK);
            if (channelTrackMap[channel] == -1) 
			{
                if (!firstTrack) 
                    ++iLastTrackNum;
				else
                    firstTrack = false;
				channelTrackMap[channel] = iLastTrackNum;
//				m_trackChannelMap[iLastTrackNum] = channel;
			}

            int trackNum = channelTrackMap[channel];

			static int prevTrackNum = -1, prevChannel = -1;
			if (prevTrackNum != (int) trackNum ||
				prevChannel != (int) channel) 
			{
				prevTrackNum = trackNum;
				prevChannel = channel;
			}
/*			
            long gap = accumulatedTime - trackTimeMap[trackNum];
            accumulatedTime += deltaTime;
            deltaTime += gap;
            trackTimeMap[trackNum] = accumulatedTime;
*/
            switch (eventCode & MIDI_MESSAGE_TYPE_MASK) 
			{
            case MIDI_NOTE_ON:
            case MIDI_NOTE_OFF:
            case MIDI_POLY_AFTERTOUCH:
            case MIDI_CTRL_CHANGE:
                data2 = getMidiByte(midiFile);
                midiEvent = new CMidiEvent(deltaTime, eventCode, data1, data2);
               // m_midiComposition[trackNum].push_back(midiEvent);
                break;
            case MIDI_PITCH_BEND:
                data2 = getMidiByte(midiFile);
                midiEvent = new CMidiEvent(deltaTime, eventCode, data1, data2);
                //m_midiComposition[trackNum].push_back(midiEvent);
                break;
            case MIDI_PROG_CHANGE:
            case MIDI_CHNL_AFTERTOUCH:
				midiEvent = new CMidiEvent(deltaTime, eventCode, data1);
               // m_midiComposition[trackNum].push_back(midiEvent);
                break;
            case MIDI_SYSTEM_EXCLUSIVE:
                messageLength = getNumberFromMidiBytes(midiFile, data1);
                metaMessage = getMidiBytes(midiFile, messageLength);

                if (MidiByte(metaMessage[metaMessage.length() - 1]) !=
                        MIDI_END_OF_EXCLUSIVE) 
				{
                    continue;
                }

                metaMessage = metaMessage.substr(0, metaMessage.length() - 1);
                midiEvent = new CMidiEvent(deltaTime,
                                          MIDI_SYSTEM_EXCLUSIVE,
                                          metaMessage);
                //m_midiComposition[trackNum].push_back(midiEvent);
                break;
            case MIDI_END_OF_EXCLUSIVE:	//Found a stray MIDI_END_OF_EXCLUSIVE
                break;
            default:	//Unsupported MIDI Event Code
                break;
            }
		}
	}
	
	return true;
}
//-------------------------------------------------- 
