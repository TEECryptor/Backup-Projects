//----------------------------------------------------
//	Name: Mp3Player.h
//	Introduce:The class CMp3Player define file
//	Creator:Youfang Yao
//	Date:11/27/05
//	Version:1.0v
//----------------------------------------------------
#ifndef _MP3PLAYER_H_
#define _MP3PLAYER_H_
//----------------------------------------------------
#include <windows.h>
//----------------------------------------------------
class CMp3Player
{
public:
	//------------------------------------------------
	CMp3Player();
	~CMp3Player();
public:
	//------------------------------------------------
	//To play the music
	void PlayMusic(char* pszFile, bool bLoop = true);
	//To stop play the music
	void StopMusic();
	void OnTimer(UINT idEvent);
	//------------------------------------------------
private:	
	//------------------------------------------------
	//��ȡָ��MCI�豸����Ϣ
	int	GetMciInfo(UINT uMCIID, int iItem);
	//-----------------------------------------------
private:	
	char		m_strFileName[MAX_PATH];
	UINT		m_uCurMciID;	//��ǰMCI�豸ID
	bool		m_bLoop;		//�Ƿ�ѭ������
	//-----------------------------------------------
};
//----------------------------------------------------
#endif	//_MP3PLAYER_H_
//----------------------------------------------------