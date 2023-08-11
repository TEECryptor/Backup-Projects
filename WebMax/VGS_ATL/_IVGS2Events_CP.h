#pragma once

template <class T>
class CProxy_IVGS2Events : public IConnectionPointImpl<T, &__uuidof(_IVGS2Events), CComDynamicUnkArray>
{
	//警告此类将由向导重新生成。
public:
	HRESULT Fire_DoScriptFunc(VARIANT * pVar, LONG count)
	{
		CComVariant varResult;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[count];
		int nConnections = m_vec.GetSize();

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				for (LONG i = 0; i < count; ++i)
				{
					pvars[i] = pVar[i];
				}
				DISPPARAMS disp = { pvars, NULL, count, 0 };
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
		return varResult.scode;

	}

};
