#pragma once
//--------------------------------------------------
#include "Midievent.h"
#include <vector>
//--------------------------------------------------
typedef std::vector<CMidiEvent*> PtrMidiEventArray;
//--------------------------------------------------
class CMidiTrack
{
public:
	CMidiTrack(int iID);
	~CMidiTrack(void);
	//----------------------------------------------
public:
	void		addEvent(CMidiEvent *pEvnet);
	CMidiEvent*	getEvent(int iIndex);
	int			getEventCnt();
	void		clearEvents();
	//----------------------------------------------
private:
	int					m_iTrackID;
	PtrMidiEventArray	m_paMidiEvents;
	//----------------------------------------------
};
//--------------------------------------------------
