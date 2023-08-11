//--------------------------------------------------
#include "Midievent.h"
//--------------------------------------------------
CMidiEvent::CMidiEvent(void)
 : m_tDeltaTime(0)
 , m_tDuration(0)
 , m_mbEventCode(0)
 , m_mbData1(0)
 , m_mbData2(0)
 , m_mbMetaEventCode(0)
 , m_strMetaMessage("")
{
}
//--------------------------------------------------
CMidiEvent::CMidiEvent(timeT deltaTime,
					   MidiByte eventCode)
 : m_tDeltaTime(deltaTime)
 , m_tDuration(0)
 , m_mbEventCode(eventCode)
 , m_mbData1(0)
 , m_mbData2(0)
 , m_mbMetaEventCode(0)
 , m_strMetaMessage("")
{
}
//--------------------------------------------------
CMidiEvent::CMidiEvent(	timeT deltaTime,
						MidiByte eventCode,
						MidiByte data1)
 : m_tDeltaTime(deltaTime)
 , m_tDuration(0)
 , m_mbEventCode(eventCode)
 , m_mbData1(data1)
 , m_mbData2(0)
 , m_mbMetaEventCode(0)
 , m_strMetaMessage("")
{
}
//--------------------------------------------------
CMidiEvent::CMidiEvent(	timeT deltaTime,
						MidiByte eventCode,
						MidiByte data1,
						MidiByte data2)
 : m_tDeltaTime(deltaTime)
 , m_tDuration(0)
 , m_mbEventCode(eventCode)
 , m_mbData1(data1)
 , m_mbData2(data2)
 , m_mbMetaEventCode(0)
 , m_strMetaMessage("")
{
}
//--------------------------------------------------
CMidiEvent::CMidiEvent(	timeT deltaTime,
						MidiByte eventCode,
						MidiByte metaEventCode,
						const std::string &metaMessage)
 : m_tDeltaTime(deltaTime)
 , m_tDuration(0)
 , m_mbEventCode(eventCode)
 , m_mbData1(0)
 , m_mbData2(0)
 , m_mbMetaEventCode(metaEventCode)
 , m_strMetaMessage(metaMessage)
{
}
//--------------------------------------------------
CMidiEvent::CMidiEvent(	timeT deltaTime,
						MidiByte eventCode,
						const std::string &sysEx)
 : m_tDeltaTime(deltaTime)
 , m_tDuration(eventCode)
 , m_mbEventCode(eventCode)
 , m_mbData1(0)
 , m_mbData2(0)
 , m_mbMetaEventCode()
 , m_strMetaMessage(sysEx)
{
}
//--------------------------------------------------
CMidiEvent::~CMidiEvent(void)
{
}
//--------------------------------------------------


//--------------------------------------------------
void CMidiEvent::setTime(const timeT &time) 
{ 
	m_tDeltaTime = time; 
}
//--------------------------------------------------
void CMidiEvent::setDuration(const timeT& duration) 
{
	m_tDuration = duration;
}
//--------------------------------------------------
timeT CMidiEvent::addTime(const timeT &time)
{
    m_tDeltaTime += time;
    return m_tDeltaTime;
}
//--------------------------------------------------
MidiByte CMidiEvent::getMessageType() const
{ 
	return ( m_mbEventCode & MIDI_MESSAGE_TYPE_MASK ); 
}
//--------------------------------------------------
MidiByte CMidiEvent::getChannelNumber() const
{ 
	return ( m_mbEventCode & MIDI_CHANNEL_NUM_MASK ); 
}
//--------------------------------------------------
timeT CMidiEvent::getTime() const 
{ 
	return m_tDeltaTime; 
}
//--------------------------------------------------
timeT CMidiEvent::getDuration() const 
{ 
	return m_tDuration; 
}
//--------------------------------------------------
MidiByte CMidiEvent::getPitch() const 
{ 
	return m_mbData1; 
}
//--------------------------------------------------
MidiByte CMidiEvent::getVelocity() const 
{ 
	return m_mbData2; 
}
//--------------------------------------------------
MidiByte CMidiEvent::getData1() const 
{ 
	return m_mbData1; 
}
//--------------------------------------------------
MidiByte CMidiEvent::getData2() const 
{ 
	return m_mbData2; 
}
//--------------------------------------------------
MidiByte CMidiEvent::getEventCode() const 
{ 
	return m_mbEventCode; 
}
//--------------------------------------------------
bool CMidiEvent::isMeta() const 
{ 
	return(m_mbEventCode == MIDI_FILE_META_EVENT); 
}
//--------------------------------------------------
MidiByte CMidiEvent::getMetaEventCode() const 
{ 
	return m_mbMetaEventCode; 
}
//--------------------------------------------------
std::string CMidiEvent::getMetaMessage() const 
{ 
	return m_strMetaMessage; 
}
//--------------------------------------------------
void CMidiEvent::setMetaMessage(const std::string &meta) 
{ 
	m_strMetaMessage = meta; 
}
//--------------------------------------------------


//--------------------------------------------------
bool operator<(const CMidiEvent &a, const CMidiEvent &b)
{
    return a.getTime() < b.getTime();
}
//--------------------------------------------------