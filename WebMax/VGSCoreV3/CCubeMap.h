////////////////////////////////////////////////////////////////////////////////////////////////
// ����: CEnvMap���Ͷ���--������ͼ
// ����: ��ĳ��ģ��Ӧ�û�����ͼ��ʵ�֣�������̬�;�̬�ģ�ʹ����DX8�е�Cube Map����
//			������ͼ���Ͷ��壬����ʵ����������ͼ�Ĵ���������������ͼ��λ��
//         �йأ�������������ͼ���Է�������CModel���У�����ģ�͵�λ�øı���ı䡣
//		   �������CMaterial���У���Ƚ��Ѵ�����Ϊ�����ж�������õ����CubeMap
//         ��ÿ�������λ���ֲ���ͬ��
//         CCubeMap��ʵ��������CubeMap����̬CubeMap,��̬CubeMap����̬CubeMapͨ��ָ��
//		   6��ͼƬ��ʵ�֣�����̬CubeMap�Ǹ�����Χ����ʵʱ��Ⱦ���ɵ�6��ͼ����ʵ�ֵģ�Ϊ��
//		   ��ʡʵʱ��Ⱦ�����Ŀ�����CCubeMap�����ָ����Ҫ��Ⱦ����������ͼ��ģ�ͣ�
//         �������Դ���ʡ��������
// ����: Lixin
// ����: 2007-1-12
// �汾˵��: 
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CCUBEMAP_H
#define _CCUBEMAP_H

#include "VGSHeader.h"
#include "CImage.h"
#include "CTexture.h"

#include <vector>
using std::vector;

class VGS_EXPORT CCubeMap : public CTexture
{
public:
	LPDIRECT3DDEVICE9				m_pd3dDevice;									//d3d�豸
	LPDIRECT3DCUBETEXTURE9			m_pCubeTex;										//������ͼ

	BOOL							m_Mode;											//��ͼģʽ	
	D3DFORMAT						m_Format;                                       //cube map�ĸ�ʽ
	INT								m_MipCount;										//MipMapCount
	INT								m_Size;                                         //cube map�Ĵ�С

	//��̬CubeMap����
	INT								imagesIdx[6];									//6��ͼ��ı�ţ�����(+-)x, (+-)y, (+-)z ��˳��


	//��̬CubeMap����
	LPDIRECT3DSURFACE9				m_pZBuffer;										//������ȾcubeMap��ZBuffer
	vector<DWORD>					m_ReflectionModels;								//��̬cubeMap��Ҫ��Ⱦ��ģ��״̬�б�

	BOOL							isRendering;									//�Ƿ�������Ⱦ
	DWORD							m_pCenterModel;									//CubeMap���������
	BOOL                            bCreated;										//�Ƿ��Ѿ�����
	BOOL							createdTexCount;								//�Ѿ���������ͼ����

	/** ��̬������ͼʱ������6��ͼ����ļ���,����(+-)x, (+-)y, (+-)z ��˳��.--added by linxq --2009-6-3
	*/
	std::vector<std::string>	m_vecFileName;

public:
	//��ʼ����̬CubeMap
	HRESULT		InitStaticCubeMap();	
	//���ͼ����ͼ����
	HRESULT		FillImageToCubeTexBuffer(INT faceIndex, DWORD dataSize, BYTE* pData);
	/** ֱ��ʹ��6��ͼ�����þ�̬������ͼ.--added by linxq --2009-6-3
	@param vecFileName in:ͼ���ļ�����.
	@return : �Ƿ�ɹ�.
	*/
	HRESULT		FillImagesToTexture(const std::vector<std::string>& vecFileName);
	/** ��������ȡ��̬������ͼ��ͼ���ļ�.--added by linxq --2009-6-3
	@param nIdx in: ��Ч��������.
	@return : �ļ�����.
	*/
	const char*	GetImageByIndex(size_t nIdx);

	//��ʼ����̬CubeMap
	HRESULT		InitDynamicCubeMap();

	//����ͼ�񵽾�̬CubeMap
	BOOL AddStaticEnvTexture(DWORD frontImage, DWORD backImage,
							 DWORD leftImage, DWORD rightImage,
							 DWORD topImage, DWORD bottomImage);

	//����ģ�͵���̬CubeMap����Ⱦģ��״̬�б�
	VOID AddEnvModel(DWORD pModel);	
	VOID removeEnvModel(DWORD modelHandle);

	/** ��ȡ��̬������ͼ��ģ������.--added by linxq --2009-6-3
	@return : ӳ���ģ������.
	*/
	inline int GetEnvModelCount() { return m_ReflectionModels.size();}
	/** ��������ȡ��̬������ͼ��ģ�;��.--added by linxq --2009-6-3
	@param nIdx in: ��Ч��������.
	@return : ģ�;��.
	*/
	DWORD GetEnvModelHandle(size_t nIdx);
	/** ɾ��ȫ������ģ��.--added by linxq --2009-6-3
	*/
	void RemoveAllEnvModels();
	
	//�ͷ���Դ
	VOID Release();	

	//virtual std::string             getTextrueFileName(int index = 0);
	
public:
	// mode : 0 -- ��̬ 1 -- ��̬
	CCubeMap(LPDIRECT3DDEVICE9 pDevice, INT mode, D3DFORMAT format, INT width, INT mipCount);
	~CCubeMap();
};

#endif
