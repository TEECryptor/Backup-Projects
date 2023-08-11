#pragma once
//--------------------------------------------------
#include "Miditrack.h"
#include "MidiFileHelper.h"
#include <vector>
#include <map>
//--------------------------------------------------
typedef vector<CMidiTrack*> PtrMidiTrackArray;
//--------------------------------------------------
class CMidiFile
{
public:
	CMidiFile(void);
	~CMidiFile(void);
	//----------------------------------------------    
	virtual bool	Open(const string &strFileName);
    virtual void	Close();
	//----------------------------------------------   
    int				getTimingDivision();
    void			getSmpteFrameSpec(int &iFps, int &iSubframes);
    FileFormatType	getFileFormat();
    unsigned int	getNumberOfTracks();
	//----------------------------------------------
private:
    bool			parseHeader(const string& midiHeader); 
	bool			parseTrack(ifstream* midiFile, int &iLastTrackNum, int iTrackByteCnt);
	//---------------------------------------------- 
private:  
    FileFormatType			m_eFormat;
	TimingFormat			m_eTimingFormat;
	int						m_iNumberOfTracks;
    int						m_iTimingDivision;
    int						m_iFps;
    int						m_iSubFrames;
	PtrMidiTrackArray		m_paMidiTracks;
	//----------------------------------------------
	string					m_strError;
	//----------------------------------------------
};
//--------------------------------------------------
