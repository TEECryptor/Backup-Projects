/////////////////////////////////////////////////////////////////////////
// ����: CD3DDevice.h
// ����: �豸����࣬���ڼ��D3D�豸������
// ����: lixin
// ����: 2006-06-26
/////////////////////////////////////////////////////////////////////////

#ifndef _CD3DDEVICE_H
#define _CD3DDEVICE_H

#include <string>

#include "VGSHeader.h"
#include "CSysFunc.h"

//��ʾ����Ϣ�ṹ��
typedef struct _ADAPTER
{
	D3DADAPTER_IDENTIFIER9	HardDriveInfo;				//��ǰ�Կ�Ӳ��������Ϣ
	D3DDISPLAYMODE			DisplayMode;				//��ǰ�Կ���ʾģʽ
	HMONITOR				Moniter;					//�뵱ǰD3D�豸���ӵ���ʾ�����
	UINT					MultiSampleTypeCnt;			//���ܳ���������
	D3DMULTISAMPLE_TYPE*	pMultiSampleType;			//���ܳ������б�
	DWORD*					pMultiSampleQualityLevels;	//
	
	D3DCAPS9				HALCap;						//HAL�豸������������������
	D3DCAPS9				REFCap;						//REF�豸����������������
	UINT					VideoMemSize;				//�Դ��С
	
    DWORD					MaxTextureBlendStages;		//Maximum number of texture-blending stages supported
    DWORD					MaxSimultaneousTextures;	//Maximum number of textures that can be simultaneously bound to the texture blending stages.
	
	BOOL					IsSupportTexGenSphereMap;	//�Ƿ�֧��������ͼ
	BOOL					IsSupportCUBEMAP;			//��ǰ�豸�Ƿ�֧��cube
	BOOL					IsSupportDynamicTexture;	//��ǰ�豸�Ƿ�֧�ֶ�̬��ͼ
	BOOL                    IsSupportTex64;				//�Ƿ�֧��64λ��ͼ
	BOOL                    IsSupportGPUNormal;			//�Ƿ�֧��GPU���ɷ�����ͼ	
	D3DFORMAT				SupportBestBumpFormat;		//֧�ֵ���õİ�͹��ͼ����
	FLOAT					SupportVS_Version;			//�Ƿ�֧��Vertex Shader, �汾��
	FLOAT					SupportPS_Version;			//�Ƿ�֧��Pixel Shader, �汾��
}_ADAPTER;

//����D3D�豸����Ҫ�Ļ��������ṹ�壬��ϵͳ�ж���Կ�ʱ��ѡ�񲢱��浱ǰϵͳ���������
typedef struct D3D_PARAM
{
	D3DDEVTYPE				DevType;					//��������HAL | REF
	DWORD					VertexProcessType;			//���㴦������
	D3DMULTISAMPLE_TYPE		MultiSampleType;			//���������2X or 4X?
	DWORD					MultiSampleQuality;			//�����������Ŀǰû��ʹ��
	
    DWORD					MaxTextureBlendStages;		//Maximum number of texture-blending stages supported
    DWORD					MaxSimultaneousTextures;	//Maximum number of textures that can be simultaneously bound to the texture blending stages.
	
	BOOL					IsSupportTexGenSphereMap;	//�Ƿ�֧��������ͼ
	BOOL					IsSupportCUBEMAP;			//��ǰ�豸�Ƿ�֧��cube
	BOOL					IsSupportDynamicTexture;	//��ǰ�豸�Ƿ�֧�ֶ�̬��ͼ
	BOOL                    IsSupportTex64;				//�Ƿ�֧��64λ��ͼ
	BOOL                    IsSupportGPUNormal;			//�Ƿ�֧��GPU���ɷ�����ͼ
	D3DFORMAT				SupportBestBumpFormat;		//֧�ֵ���õİ�͹��ͼ����	
	FLOAT					SupportVS_Version;			//�Ƿ�֧��Vertex Shader, �汾��
	FLOAT					SupportPS_Version;			//�Ƿ�֧��Pixel Shader, �汾��
}D3D_PARAM;

//CD3DDevice��----------------------------------------------------------------------
class VGS_EXPORT CD3DDevice  
{
private:
	LPDIRECT3D9					m_pd3d;						//d3d����
	
public:
	UINT						adapterCnt;					//�Կ�����
	_ADAPTER*					pAdapters;					//�Կ��б�
	
	//�õ���ǰϵͳ��������ʾ�豸����
	HRESULT						GetAllAdapterAbility();	
	//�õ���ǰ����ʵ��豸����
	HRESULT						ChooseBestD3DParam(D3D_PARAM* pD3DParam);
	//ȷ���豸��ĳ��״̬�µĴ�������
	HRESULT						ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format );
	//�ͷ��ڴ�������Դ
	VOID						Release();
	//����Ƿ�֧��Bump
	D3DFORMAT					ConfirmBumpUV(D3DCAPS9* pCaps, D3DFORMAT backBufFormat);
	//����Ƿ�֧��Cube
	BOOL						ConfirmCubeMap(D3DCAPS9* pCaps);
	//����Ƿ�֧��DynamicTexture
	BOOL						ConfirmDynamicTexture(D3DCAPS9* pCaps);
	//����Ƿ�֧��Ӳ��������ͼ
    BOOL                        ConfirmGPUNormal(_ADAPTER* pAdapter);
	
	
public:
	CD3DDevice(LPDIRECT3D9 pd3d);
	~CD3DDevice();
	
};

#endif
