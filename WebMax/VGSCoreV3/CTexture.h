/////////////////////////////////////////////////////////////////////////
// ����: CTexture���Ͷ���--�����е���ͼ����
// ����: CTexture���Ͷ�������ͼ�ĸ�������
// ����: lixin	
// ����: 2005-12-20
////////////////////////////////////////////////////////////////////////

#ifndef _CTEXTURE_H
#define _CTEXTURE_H

#include "VGSHeader.h"
#include "ResourceManager.h"
#include "CSysFunc.h"

//##ModelId=47EBC72E03C8
class VGS_EXPORT CTexture : public CResource
{	
public:
	//##ModelId=47EBC72E03C9
	INT						index;

	//##ModelId=47EBC72E03CB
	CHAR					path[224];				//��ͼ·��
	//##ModelId=47EBC72E03CC
	// CHAR					pathAndName[256];		// ��ͼ���ƺ�·��
	std::string             m_sFilePathandName;     // ��ͼ�ļ�����
	//##ModelId=47EBC72E03CD
	int					width;					//��ͼ���
	//##ModelId=47EBC72E03CE
	int					height;					//��ͼ�߶�
	//##ModelId=47EBC72E03CF
	BOOL					bCreated;				//��ͼ�Ƿ��Ѿ�������������Ⱦ����
	UINT					MipCount;				//��ͼ��MipMap����
	//##ModelId=47EBC72E03D1
	D3DFORMAT				m_format;						//ÿ��������ռ���ֽ���
	//##ModelId=47EBC72E03D6
	_D3DXIMAGE_INFO			imageInfo;				//ͼ���ļ���ʽ
	//##ModelId=47EBC72E03DA
	UINT					ZipQuality;				//����ѹ��ΪN3Dʱ��ѹ������0-100
	BOOL					withAlpha;				//����Alphaͨ��
	BOOL                    isOpacity;              //�Ƿ���͸����ͼ
	BOOL                    isDynamic;              //�Ƿ��Ƕ�̬��ͼ
	//UINT					dataSize;				//ͼ���ļ���С
	//BYTE*					data;					//ͼ������
    UINT                    imageIdx;               //ͼƬ��������

	//##ModelId=47EBC72E03DD
	INT						brightness;				//����
	//##ModelId=47EBC72E03DE
	INT						contrast;				//�Աȶ�

	//##ModelId=47EBC72E03DF
	BOOL					bAlphaTest;				//�Ƿ�Alpha����
	//##ModelId=47EBC72E03E0
	BOOL					bAlphaBlend;			//�Ƿ�Alpha��ɫ

	//##ModelId=47EBC72E03E2
	LPDIRECT3DTEXTURE9		m_textureBuf;				//D3D9��ͼ����
	//##ModelId=47EBC72E03E6
	BYTE*					buffer;					//��������

	BOOL					bIsOverlayTexture;		//�Ƿ�����Overlay

	BOOL					bUseByScene;			//�Ƿ񱻳�������
protected:
	VGS_TextureType			m_textureType;

public:
	//������ͼ��alphaֵ
	VOID					SetOpacity(INT opacity);	
	//�ͷ���ͼ��Դ
	virtual VOID			Release();
	//ֻ�ͷ�D3D��ͼ����
	virtual VOID			ReleaseBufferOnly();

	virtual std::string             getTextrueFileName(int index = 0);

	/** �ı�������ͼ��ͼ���ļ�.
	@param strFileName in: �µ�ͼ���ļ�����.
	*/
	HRESULT ChangeImage(const std::string & strFileName);
	/** �ı�͸����ͼ��ͼ���ļ�.--added by linxq --2009-6-3
	@param strFileName in: �µ�ͼ���ļ�����.
	*/
	HRESULT ChangeTransparentImage(const std::string & strFileName);

	VGS_TextureType GetType() const { return m_textureType; }

public:
	CTexture();
	virtual ~CTexture();

};

#endif
