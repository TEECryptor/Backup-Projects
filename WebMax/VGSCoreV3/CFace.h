/////////////////////////////////////////////////////////////////////////
// ����: Face���Ͷ���--��������
// ����: Face���Ͷ�����������ĸ�������
// ����: lixin	
// ����: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CFACE_H
#define _CFACE_H

#include "VGSHeader.h"
#include "CVertex.h"

//##ModelId=47EBC74E01A8
class VGS_EXPORT CFace
{

public:
	//##ModelId=47EBC74E01A9
	UINT				index;				//��ǰ��������ģ���������еı��
	//##ModelId=47EBC74E01AB
	CVertex*			vertices0;			//��������ָ��
	//##ModelId=47EBC74E01B6
	CVertex*			vertices1;			//��������ָ��
	//##ModelId=47EBC74E01BB
	CVertex*			vertices2;			//��������ָ��
	//##ModelId=47EBC74E01C0
	D3DXVECTOR3			normal;				//����
	//##ModelId=47EBC74E01C4
	BOOL				visible;			//�Ƿ�ɼ�
	//���ڼ���ײ�������
	D3DXVECTOR3			worldNormal;		//�������������ϵ�еķ���

public:
	//##ModelId=47EBC74E01C5
	CFace();
	//##ModelId=47EBC74E01C6
	~CFace();

};

#endif
