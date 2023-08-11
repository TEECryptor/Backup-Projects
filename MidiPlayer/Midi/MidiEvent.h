#pragma once
//--------------------------------------------------
#include "MidiDefine.h"
//--------------------------------------------------
class CMidiEvent
{
public:
	CMidiEvent(void);

    // No data event
    CMidiEvent(timeT deltaTime,
               MidiByte eventCode);

    // Single data byte case
    CMidiEvent(timeT deltaTime,
               MidiByte eventCode,
               MidiByte data1);

    // Double data byte
    CMidiEvent(timeT deltaTime,
               MidiByte eventCode,
               MidiByte data1,
               MidiByte data2);

    // Meta event
    CMidiEvent(timeT deltaTime,
               MidiByte eventCode,
               MidiByte metaEventCode,
               const std::string &metaMessage);

    // Sysex style constructor
    CMidiEvent(timeT deltaTime,
               MidiByte eventCode,
               const std::string &sysEx);

	~CMidiEvent(void);
	//----------------------------------------------
public:
    void setTime(const timeT &time);
    void setDuration(const timeT& duration);
    timeT addTime(const timeT &time);
    MidiByte getMessageType() const;
    MidiByte getChannelNumber() const;
    timeT getTime() const;
    timeT getDuration() const;
    MidiByte getPitch() const;
    MidiByte getVelocity() const;
    MidiByte getData1() const;
    MidiByte getData2() const;
    MidiByte getEventCode() const;
    bool isMeta() const;
    MidiByte getMetaEventCode() const;
    std::string getMetaMessage() const;
    void setMetaMessage(const std::string &meta);
    friend bool operator<(const CMidiEvent &a, const CMidiEvent &b);
	//----------------------------------------------
private:
   // CMidiEvent& operator=(const CMidiEvent);

    timeT			m_tDeltaTime;
    timeT			m_tDuration;
    MidiByte        m_mbEventCode;
    MidiByte        m_mbData1;         // or Note
    MidiByte        m_mbData2;         // or Velocity

    MidiByte        m_mbMetaEventCode;
    std::string     m_strMetaMessage;
	//----------------------------------------------
};
//--------------------------------------------------
