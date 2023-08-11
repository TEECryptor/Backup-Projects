// CDynamicTex.h: interface for the CDynamicTex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDYNAMICTEX_H__73DFEEE0_2784_43D8_B023_02854EE17CBC__INCLUDED_)
#define AFX_CDYNAMICTEX_H__73DFEEE0_2784_43D8_B023_02854EE17CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CTexture.h"

#include <vector>
#include <string>
#include <algorithm>

class VGS_EXPORT CDynamicTex : public CTexture
{
public:
	CDynamicTex(LPDIRECT3DDEVICE9 pd3dDevice,int nWidth, int nHeight, D3DFORMAT fmt, int framecount);
	~CDynamicTex();

public:
	/** D3D Device����
	*/
	LPDIRECT3DDEVICE9				m_pd3dDevice;
	/** ��̬��ͼ��ǰʹ�õ���ͼ���棬���Դ���
	*/
	LPDIRECT3DTEXTURE9		m_pCurTexture;
	/** ��̬��ͼ����ͼ���У����ڴ���
	*/
	std::vector<LPDIRECT3DTEXTURE9>	m_vecTextures;
	/** ͼ���ļ����б�.
	*/
	std::vector<std::string> m_vecFileName;

	//֡��ͼ��ͼ����������
	std::vector<int>		imagesIdx;          
	//��̬��ͼ����
	std::string				name;	



	/** �Ƿ����ڲ���
	*/
	bool	m_bPlay;
	/** �Ƿ�ѭ������
	*/
	bool	m_bLoop;
	/** ��̬��ͼ���ŵ�֡�ʣ�����������/��ͼ����.��������ȾFPS.��λ����.
	*/
	DWORD	m_nFPS;
	/** �������ڣ���λ��.
	*/
	float	m_fDuration;

	/** �Ѿ���������ͼ����
	*/
	int		m_nCreatedCount;
	
	/** ��̬��ͼ��֡��
	*/
	int		m_frameCnt;		

	/** ��ǰ���ŵ�֡
	*/
	int		m_nCurFrame;
	/** 
	*/
	int		m_nLstFrame;
public:

	/** ���ͼ����ͼ����
	*/
	HRESULT							FillImageToTextureBuffer(size_t imageIndex, DWORD dataSize, BYTE* pData);

	//�ı���ͼ��ĳһ֡
	HRESULT							ChangeFrameTexture(INT frameIndex);
	//������ͼ
	void							Update();
	//����
	void							Play();
	//��ͣ
	void							Pause();
	//ֹͣ
	void							Stop();
	//�ͷ���ͼ����
	void							Invalidate();
	//����
	HRESULT							Restore();
	//�ͷ���Դ
	void							Release();		

	inline LPDIRECT3DTEXTURE9	GetCurTexture() { return m_pCurTexture;};
	
	/** ��ȡ��̬��ͼ�Ĳ������ڣ���λΪ��.
	*/
	inline float		GetDuration() { return m_fDuration;}
	/** ���ö�̬��ͼ�Ĳ������ڣ���λΪ��.
	*/
	inline void	SetDuration(float fDuration) { m_fDuration = fDuration; m_nFPS = (int)(1000 * fDuration/GetImageCount());}

	/** ��������ȡ��ͼ�ļ���
	*/
	std::string GetFileName(size_t nIdx);
	/** ��ȡ��ͼ����
	*/
	inline int	GetImageCount() {return m_vecTextures.size(); }
	/** ���������޸���ͼĳ��ͼ��
	@param nIdx in: ��������������Чֵ.
	@param strFileName in:�µ�ͼ���ļ���.
	*/
	HRESULT	ChangeImage(size_t nIdx, const std::string & strFileName);
	/** ��������һ��ͼ��
	@param strFileName in:ͼ���ļ���.
	*/
	HRESULT	AddImage(const std::string & strFileName);
	/** ������ɾ��ͼĳ��ͼ��
	@param nIdx in: ��������������Чֵ.
	*/
	bool DeleteImage(size_t nIdx);
	/** �������ͼ��.
	*/
	void ClearImages();
protected:
	/** ��ʼ����̬��ͼ����
	*/
	void	Init();


};

#endif // !defined(AFX_CDYNAMICTEX_H__73DFEEE0_2784_43D8_B023_02854EE17CBC__INCLUDED_)
