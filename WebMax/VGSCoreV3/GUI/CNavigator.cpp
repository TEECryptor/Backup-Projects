/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUINavigator.cpp
* ����ժҪ�� // ����ͼ�ඨ�弰ʵ��
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

#include "CNavigator.h"

#include "CGUIManager.h"
#include <algorithm>
#include "..\Root.h"
#include "..\CCamera.h"
#include "..\CModel.h"

using namespace std;

CNavigator::CNavigator(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName)
		  :COverlay(pd3dDevice, sName)
{
	m_ObjType = VGS_GUIOBJ_TYPE_NAVIGATION;
	m_fWidth  = 0.5f;
	m_fHeight = 0.5f;

	memset(&m_rcNavScene, 0, sizeof(m_rcNavScene));
}

CNavigator::~CNavigator()
{
	VecNavPointerPtr::iterator it = m_vNavPointer.begin(), end = m_vNavPointer.end();
	for (; it != end; ++it)
		SAFE_DELETE(*it);
	m_vNavPointer.clear();
}
void CNavigator::Render()
{
	if (!m_bVisible) 
		return;
	// ���ò��ʣ���Ⱦ����
	if (m_pMtrl)
		m_pMtrl->SetToDevice(m_pd3dDevice, 0);
	m_pd3dDevice->SetStreamSource( 0, m_VBuffer, 0, m_FVFSize );
	//����FVF
	m_pd3dDevice->SetFVF(m_FVF); //DX9ΪSetFVF,DX8���������	
	//����IB
	m_pd3dDevice->SetIndices(m_pIBuffer);

	//�ı������������������������ϴ���ȵ����������ٶ�**************
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVertexCount, 0,	m_nFaceCount);

	VecNavPointerPtr::iterator it = m_vNavPointer.begin(), end = m_vNavPointer.end();
	for (; it != end; ++it)
	{
		if (*it)
			(*it)->Render();
	}
}
CNavPointer* CNavigator::GetPointerByIndex(int idx)
{
	if (idx >= (int)m_vNavPointer.size())
		return 0;
	return m_vNavPointer[idx];
}

std::string CNavigator::GetPointerName(INT index)
{
	CNavPointer* ptr = GetPointerByIndex(index);
	if (ptr)
	{
		return ptr->GetName();
	}
	return "";
}
CNavPointer* CNavigator::GetPointerByHandle(DWORD dwHandle)
{
	CNavPointer* ptr = (CNavPointer*)dwHandle;
	VecNavPointerPtr::iterator it = std::find(m_vNavPointer.begin(), m_vNavPointer.end(), ptr);
	if (it != m_vNavPointer.end())
		return (*it);
	return 0;
}
CNavPointer* CNavigator::GetPointerByName(const std::string& name)
{
	VecNavPointerPtr::iterator it = std::find_if(m_vNavPointer.begin(), m_vNavPointer.end(), GuiNameFunctor(name));
	if (it != m_vNavPointer.end())
		return (*it);
	return 0;
}
// ָ�����Ƶĵ���ָ���Ƿ��Ѿ����ڡ�sVGSName�������VGS����
bool CNavigator::IsExist(const std::string &sVGSName)
{
	VecNavPointerPtr::iterator it = std::find_if(m_vNavPointer.begin(), m_vNavPointer.end(), GuiNameFunctor(sVGSName));
	return (it != m_vNavPointer.end());
}

// ���/ɾ��������
CNavPointer* CNavigator::AddNavPointer(const std::string &sName, const std::string &sFileName, INT pointerType, DWORD pointedHandle)
{
	if (IsExist(sName))
		return 0;
	CNavPointer* ptr = new CNavPointer(m_pd3dDevice, sName.c_str());
	if (!ptr)
		return 0;
	ptr->SetNavType(pointerType);
	ptr->SetPointerHandle(pointedHandle);

	//material...
	if (!sFileName.empty())
	{
		std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(sName.c_str());
		CMaterial* pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
		if (pMtrl)
		{
			pMtrl->createTextureLayer(sFileName, strMtrl + "_txt0", 0);
			ptr->SetMaterial(pMtrl);
		}
	}
	else
	{
		ptr->SetMaterial(NULL);
	}
	ptr->SetPosition(m_rc.left, m_rc.top);
	m_vNavPointer.push_back(ptr);
	return ptr;
}
// ���/ɾ��������
void CNavigator::AddNavPointer(CNavPointer* ptr)
{
	if (find(m_vNavPointer.begin(), m_vNavPointer.end(), ptr) != m_vNavPointer.end())
	{
		m_vNavPointer.push_back(ptr);
	}
}
//ʹ����ͬ�Ĳ���
CNavPointer* CNavigator::CloneNavPointer(const std::string &sName, const std::string &sReferencePointer)
{
	VecNavPointerPtr::iterator it = std::find_if(m_vNavPointer.begin(), m_vNavPointer.end(), GuiNameFunctor(sReferencePointer));
	if (it == m_vNavPointer.end())//δ�ҵ����е�
		return 0;
	CNavPointer* ptr = new CNavPointer(m_pd3dDevice, sName.c_str());
	if (!ptr)
		return 0;
	ptr->SetNavType((*it)->GetNavType());
	ptr->SetPointerHandle((*it)->GetPointerHandle());
	ptr->SetMaterial((*it)->GetMaterial());
	ptr->SetPosition(m_rc.left, m_rc.top);
	m_vNavPointer.push_back(ptr);
	return ptr;
}

void CNavigator::deleteNavPointer(const std::string &sName)
{
	VecNavPointerPtr::iterator it = std::find_if(m_vNavPointer.begin(), m_vNavPointer.end(), GuiNameFunctor(sName));
	if (it != m_vNavPointer.end())
	{
		//��Ҫ��Mtrl Manager��ɾ������....
		(*it)->GetMaterial()->Release();
		m_vNavPointer.erase(it);
		SAFE_DELETE((*it));
	}
}

// ���µ�����Χ
VOID CNavigator::SetNavRange(const FRect& rc)
{
	m_rcNavScene = rc;
	//update....
}

// ���µ�����ͼ
VOID CNavigator::SetNavMap(const std::string &sNavImageFile)
{
	SetOverlayImageFile(sNavImageFile, std::string(""));
}

// �õ���������ͼʹ�õ�ͼ������
std::string CNavigator::GetNavMap()
{
	std::string str1, str2;
	GetOverlayImageFile(str1, str2);	
	return str1;
}


// ���µ�����λ��
VOID CNavigator::UpdatePointers()
{
	//x��y����ľ���
	FLOAT xLen = m_rcNavScene.Right - m_rcNavScene.Left;
	FLOAT yLen = m_rcNavScene.Top - m_rcNavScene.Bottom;  //SceneTop��Z������������SceneTop > SceneBottom

	VecNavPointerPtr::iterator it = m_vNavPointer.begin(), end = m_vNavPointer.end();
	for (; it != end; ++it)
	{
		FLOAT xPercent = 0.0f;
		FLOAT yPercent = 0.0f;

		CCamera* pCamera = NULL;
		CModel* pModel = NULL;

		switch ((*it)->GetNavType())
		{
		case 0://���
			pCamera = (CCamera*)(*it)->GetPointerHandle();
			if (pCamera)
			{
				//x��y����m_pCamera��λ��
				D3DXVECTOR3 pos = pCamera->GetPosition(VTS_WORLD); 
				xPercent = (pos.x - m_rcNavScene.Left)/xLen;
				yPercent = 1.0f - (pos.z - m_rcNavScene.Bottom)/yLen; //ƽ��ͼ��Rect Top�����棬����Ҫ��תһ��
			}
			break;			
		case 1://ģ��
			pModel = (CModel*)(*it)->GetPointerHandle();
			if (pModel)
			{
				//x��y����m_pModel��λ��
				// 				xPercent = (pModel->transform.position.x - m_rcNavScene.Left)/xLen;
				// 				yPercent = 1.0f - (pModel->transform.position.z - m_rcNavScene.Bottom)/yLen; //ƽ��ͼ��Rect Top�����棬����Ҫ��תһ��
			}
		default:
			continue;
		}
		//����PointerӦ����Map�ϵ����λ��
		RECT mapOverlayRect = m_rc;

		RECT pointerRect;
		(*it)->GetAbsoluteRect(pointerRect);
		INT pointerWidth = pointerRect.right - pointerRect.left;
		INT pointerHeight = pointerRect.bottom - pointerRect.top;

		INT	pointerX = (INT)(xPercent * (m_rc.right - m_rc.left));
		INT pointerY = (INT)(yPercent * (m_rc.bottom - m_rc.top));

		// 		pointerX += m_rc.left;
		// 		pointerY += m_rc.top;

		pointerX -= pointerWidth/2;
		pointerY -= pointerHeight/2;

		//�õ�Pointer�µ�Rect
		RECT newPointerRect;
		newPointerRect.left = mapOverlayRect.left + pointerX;
		newPointerRect.right = newPointerRect.left + pointerWidth;
		newPointerRect.top = mapOverlayRect.top + pointerY;
		newPointerRect.bottom = newPointerRect.top + pointerHeight;

		//���������߽�
		if (newPointerRect.left < mapOverlayRect.left)
		{
			newPointerRect.left = mapOverlayRect.left;
			newPointerRect.right = newPointerRect.left + pointerWidth;
		}
		else
		{
			//��������ұ߽�
			if (newPointerRect.right > mapOverlayRect.right)
			{
				newPointerRect.right = mapOverlayRect.right;
				newPointerRect.left = newPointerRect.right - pointerWidth;
			}
		}

		//��������ϱ߽�
		if (newPointerRect.top < mapOverlayRect.top)
		{
			newPointerRect.top = mapOverlayRect.top;
			newPointerRect.bottom = newPointerRect.top + pointerHeight;
		}
		else
		{
			if (newPointerRect.bottom > mapOverlayRect.bottom)
			{
				newPointerRect.bottom = mapOverlayRect.bottom;
				newPointerRect.top = newPointerRect.bottom - pointerHeight;
			}
		}
		(*it)->SetAbsoluteRect(newPointerRect);	
	}
}
void CNavigator::UpdateMap(RECT& rc)
{

}
HRESULT CNavigator::UpdateRect(const RECT& rc)
{
	if (!m_VBuffer || !m_pIBuffer)
		return E_FAIL;
	//�µ�λ��
	m_Point[0].x = m_pVertex[0].position.x = (FLOAT)rc.left;
	m_Point[0].y = m_pVertex[0].position.y = (FLOAT)rc.top;

	m_Point[1].x = m_pVertex[1].position.x = (FLOAT)rc.right;
	m_Point[1].y = m_pVertex[1].position.y = (FLOAT)rc.top;

	m_Point[2].x = m_pVertex[2].position.x = (FLOAT)rc.right;
	m_Point[2].y = m_pVertex[2].position.y = (FLOAT)rc.bottom;

	m_Point[3].x = m_pVertex[3].position.x = (FLOAT)rc.left;
	m_Point[3].y = m_pVertex[3].position.y = (FLOAT)rc.bottom;

	//����֮ǰ�ı任
	// 	SetRotation(m_fAngle);//???����δ��������

	//�����ڴ�
	if( FAILED(m_VBuffer->Lock(0, m_nVertexCount * m_FVFSize, (VOID**)&(m_pVertexBuf), D3DLOCK_DISCARD) ))
		return E_FAIL;

	//����������ݵ����㻺��
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[0].position = m_pVertex[0].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[1].position = m_pVertex[1].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[2].position = m_pVertex[2].position;
	((VERTEXDECL_POSRHWCOLORCOORD1*)m_pVertexBuf)[3].position = m_pVertex[3].position;

	//Unlock���㻺��
	m_VBuffer->Unlock();

	//���µ�����
	int left = 0, top = 0;
	VecNavPointerPtr::iterator it = m_vNavPointer.begin(), end = m_vNavPointer.end();
	for (; it != end; ++it)
	{
		if (*it)
		{
			(*it)->GetRelativePosition(left, top);
			(*it)->SetPosition(rc.left + left, rc.top + top);
		}
	}

	return S_OK;
}
