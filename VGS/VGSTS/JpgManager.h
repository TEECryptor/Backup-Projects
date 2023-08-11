//////////////////////////////////////////////////////////////////////
//	JpgManager.h: interface for the CJpgManager class.
//	������	CJpgManager
//	���ߣ�	yaoyoufang
//	���ڣ�	04-8-1
//	��飺	����ʵ���˶�jpg�ļ��ļ��غͱ���
//
//------------------------------------------------------------
#if !defined(AFX_JPGMANAGER_H__CADB9767_047A_49F4_83FF_086623F3DF45__INCLUDED_)
#define AFX_JPGMANAGER_H__CADB9767_047A_49F4_83FF_086623F3DF45__INCLUDED_
//------------------------------------------------------------
#include <windows.h>
#include "ijl.h"
//------------------------------------------------------------
typedef enum _JPGPARAMS
{
	/*ͼƬ����buff��С*/
	_JPGIMAGESIZE		= 0,
	/*ͼƬ��Ȳ���*/
	_JPGIMAGEWIDTH		= 1,
	/*ͼƬ�߶Ȳ���*/
	_JPGIMAGEHEIGHT		= 2,
	/*ͼƬͨ������*/
	_JPGIMAGECHANNELS	= 3
}JPGPARAMS;
//------------------------------------------------------------
class CJpgManager  
{
public:
	CJpgManager();
	virtual ~CJpgManager();
//��������
public:
	//--------------------------------------------------------
	static JPEG_CORE_PROPERTIES s_Image;	 
	//��ʼ��ijl��
	static BOOL InitIjlLibrary();
	//�ͷ�ijl��
	static void FreeIjlLibrary();
	//����
	static BOOL	LoadImageFromBuff(BYTE* pJpgData, int iJpgDataSize, BYTE*& pData, int &iWidth, int &iHeight);
	//--------------------------------------------------------
	/******************����JPG�ļ��ĺ���********************/
	//��jpg�ļ�����ȡͼƬ���Ժ����ݵ���Ա����
	BOOL		jpgOpenFile(const char* pszFileName);
	//����jpgͼƬ����
	BYTE*		jpgGetImageData();
	//����ͼƬ����
	int			jpgGetImageParams(JPGPARAMS eParams);
	//����ͼƬBMP����Ϣ
	void		jpgGetImageBMPInfo(BITMAPINFO &stuBMPInfo);
	/**************����JPG�ļ��ĺ���_End******************/

	/*****************дJPG�ļ��ĺ���*********************/
	//����ͼƬ����
	void		jpgSetImageParams(JPGPARAMS eParams, int iParam);
	//�����ݱ���ΪJPG��ʽ���ļ�
	BOOL		jpgSaveFile(BYTE* pImageData,const char* pszFileName);

	//���ڴ����ȡ���ݣ�Ȼ��ת��Ϊλͼ����
	BOOL		jpgReadBuff(BYTE* pJpgData, int iJpgDataSize);

//˽�з���
private:
	//��ʽת��
	void		jpgRGBA_FPX_to_BGRA(BYTE* data,int width,int height);
	//��ʽת��
	void		jpgBGRA_to_RGBA(BYTE* data,int width,int height);

//˽�г�Ա����
private:
	int			m_iImageSize;		//ͼƬ����Buff��С
	int			m_iImageWidth;		//ͼƬ���ؿ�
	int			m_iImageHeight;		//ͼƬ���ظ�
	int			m_iImageChannels;	//ͼƬͨ����
	BYTE*		m_pImageData;		//ͼƬ����
	BITMAPINFO	m_stuBMPInfo;		//ͼƬBMP��Ϣ����
};

#endif // !defined(AFX_JPGMANAGER_H__CADB9767_047A_49F4_83FF_086623F3DF45__INCLUDED_)
