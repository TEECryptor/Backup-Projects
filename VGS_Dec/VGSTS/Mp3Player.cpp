//----------------------------------------------------
//	Name: Mp3Player.cpp
//	Introduce:The class CMp3Player define file
//	Creator:Youfang Yao
//	Date:11/27/05
//	Version:1.0v
//----------------------------------------------------
#include "stdafx.h"
#include "Mp3Player.h"
#include <mmsystem.h>
//----------------------------------------------------
CMp3Player*		g_pMp3Player = NULL;
//----------------------------------------------------
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg, UINT idEvent,DWORD dwTime)
{
	if(g_pMp3Player) g_pMp3Player->OnTimer(idEvent);
}
//----------------------------------------------------
CMp3Player::CMp3Player()
: m_uCurMciID(0), m_bLoop(true)
{
	g_pMp3Player = this;
}
//----------------------------------------------------
CMp3Player::~CMp3Player()
{
}
//----------------------------------------------------
//To play the music
//----------------------------------------------------
void CMp3Player::PlayMusic(char* pszFile, bool bLoop)
{
	if(pszFile == NULL) return;

	StopMusic();

	MCI_OPEN_PARMS mciopenparms;
	MCI_PLAY_PARMS mciplayparms;
	strcpy(m_strFileName, pszFile);
	mciopenparms.lpstrElementName = m_strFileName;
	//�ļ�����
	mciopenparms.lpstrDeviceType = NULL;
	//�򿪣���MCI�豸����������Ϣ
	UINT uResult = mciSendCommand(0, MCI_OPEN, MCI_DEVTYPE_WAVEFORM_AUDIO, (DWORD)(LPVOID)&mciopenparms);
	if(uResult != 0)
	{
		//MessageBox(NULL, "��mp3�ļ�����", "Error", MB_OK);
		return;
	}

	//�豸ID
	m_uCurMciID = mciopenparms.wDeviceID;
	//
//	mciplayparms.dwCallback = (DWORD)GetSafeHwnd();	
	//����
	DWORD dMusicLen = GetMciInfo(m_uCurMciID, MCI_STATUS_LENGTH);
	DWORD dStartPos = MCI_MAKE_HMS(0, 0, 0);
	DWORD dEndPos = MCI_MAKE_HMS(MCI_HMS_HOUR(dMusicLen), MCI_HMS_MINUTE(dMusicLen), MCI_HMS_SECOND(dMusicLen)); 
	//	
	mciplayparms.dwFrom = dStartPos;
	mciplayparms.dwTo = dEndPos;
	mciSendCommand(m_uCurMciID, MCI_PLAY, MCI_TO | MCI_FROM, (DWORD)(LPVOID)& mciplayparms);
	
	//�Ƿ���ѭ������
	m_bLoop = bLoop;

	//�����ѭ������
	if(m_bLoop) ::SetTimer(NULL, 0, 1000, (TIMERPROC)TimerProc);
}
//------------------------------------------------
//To stop play the music
//------------------------------------------------
void CMp3Player::StopMusic()
{
	KillTimer(NULL, 0);
	mciSendCommand(m_uCurMciID, MCI_CLOSE, 0, NULL);
}
//------------------------------------------------
void CMp3Player::OnTimer(UINT idEvent)
{
	if(m_bLoop)
	{
		if(GetMciInfo(m_uCurMciID, MCI_STATUS_MODE) == MCI_MODE_STOP) 
		{
			StopMusic();
			PlayMusic(m_strFileName, m_bLoop);
		}
	}
}
//------------------------------------------------
//��ȡָ��MCI�豸����Ϣ
//------------------------------------------------
int	CMp3Player::GetMciInfo(UINT uMCIID, int iItem)
{	
	MCI_STATUS_PARMS mcistatusparms;

//	mcistatusparms.dwCallback = (DWORD)GetSafeHwnd();	

	mcistatusparms.dwItem = iItem;
	mcistatusparms.dwReturn = 0;
	//
	mciSendCommand(uMCIID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcistatusparms);

	return mcistatusparms.dwReturn;
}
//------------------------------------------------
