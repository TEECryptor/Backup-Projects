/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUIButton.h
* ����ժҪ�� // ��ť�ඨ�弰ʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/


#pragma once

#include "COverlay.h"

#include <vector>

using namespace std;

class VGS_EXPORT CButton : public COverlay
{
public:

	CButton(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName);
	~CButton();

	/**��ð�ť�ľɽӿڵ�JS�ص���������
	@return callBackFunc out : ��ť�¼��Ļص���������
	*/
	const char * GetJsCallbackFunc();
	/**���ð�ť�ľɽӿڵ�JS�ص���������
	@param sFunc int : ��ť�¼��Ļص���������
	*/
	void SetJsCallbackFunc(const char * sFunc);

	/**��ð�ťʹ�õ�ͼ���ļ�
	@param type       in  : 0-��ť����״̬(up)ʹ�õ�ͼƬ��1-����(down)ʱʹ�õ�ͼƬ��2-�����ͣ(over)ʱʹ�õ�ͼƬ
	@param sImageFile out : ͼƬ�ļ�������������·��
	*/
	//void GetImageFile(int type, char* sImageFile);

	/**���ð�ťʹ�õ�ͼ���ļ�
	@param type       in : 0-��ť����״̬(up)ʹ�õ�ͼƬ��1-����(down)ʱʹ�õ�ͼƬ��2-�����ͣ(over)ʱʹ�õ�ͼƬ
	@param sImageFile in : ͼƬ�ļ�������������·��
	*/
	// void SetImageFile(int type, const char* sImageFile);


	// ���ò���  Type : 0-��ť����״̬(up)ʹ�õĲ��ʣ�1-����(down)ʱʹ�õĲ��ʣ�2-�����ͣ(over)ʱʹ�õĲ���
	VOID SetMaterial(INT Type, CMaterial* pMaterial);
	// ��ȡ����
	CMaterial* GetMaterial(INT Type);  // 0-��ť����״̬(up)ʹ�õĲ��ʣ�1-����(down)ʱʹ�õĲ��ʣ�2-�����ͣ(over)ʱʹ�õĲ���

	bool GetMouseUpImageFileName(std::string& str);
	bool SetMouseUpImageFileName(const char* szFile);
	bool GetMouseDownImageFileName(std::string& str);
	bool SetMouseDownImageFileName(const char* szFile);
	bool GetMouseOverImageFileName(std::string& str);
	bool SetMouseOverImageFileName(const char* szFile);

private:
	/* ��������ʹ�û����m_pMtrl */
	//!������ʱ�Ĳ���
	CMaterial*   m_pMouseOverMtrl;
	//!��갴��ʱ�Ĳ���
	CMaterial*   m_pMouseDownMtrl;

	std::string	m_strJsCallbackFunc;
	std::vector<std::string> m_vCallBackFunc;  // �ص����������б�
};