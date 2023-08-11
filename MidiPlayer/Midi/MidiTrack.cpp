//--------------------------------------------------
#include "Miditrack.h"
//--------------------------------------------------
CMidiTrack::CMidiTrack(int iID)
 : m_iTrackID(iID)
{
}
//--------------------------------------------------
CMidiTrack::~CMidiTrack(void)
{
	clearEvents();
}
//--------------------------------------------------


//--------------------------------------------------
void CMidiTrack::addEvent(CMidiEvent *pEvnet)
{
	m_paMidiEvents.push_back(pEvnet);
}
//--------------------------------------------------
CMidiEvent* CMidiTrack::getEvent(int iIndex)
{
	int iCurIndex = 0;
	CMidiEvent* pEvent = NULL;
	for (PtrMidiEventArray::iterator it = m_paMidiEvents.begin();
		 it != m_paMidiEvents.end();
		 it++, iCurIndex++)
	{
		if (iCurIndex == iIndex)
		{
			pEvent = (*it);
			break;
		}
	}
	return pEvent;
}
//--------------------------------------------------
int CMidiTrack::getEventCnt()
{
	return (int)m_paMidiEvents.size();
}
//--------------------------------------------------
void CMidiTrack::clearEvents()
{
	CMidiEvent* pEvent = NULL;
	for (PtrMidiEventArray::iterator it = m_paMidiEvents.begin();
		 it != m_paMidiEvents.end();
		 it++)
	{
		pEvent = *it;
		if (NULL != pEvent)
		{
			delete pEvent;
			pEvent = NULL;
		}
	}
	m_paMidiEvents.clear();
}
//--------------------------------------------------
