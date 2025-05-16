#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CXMLReader final : public CSingleton<CXMLReader>
{
private:
	explicit CXMLReader() DEFAULT;
	virtual ~CXMLReader() DEFAULT;

public:
	void				Open(wstring _wstrFilePath);
	void				Close();

public:
	const wstring		Read_String(_uint iLength);
	_int				Read_Int();
	_uint				Read_uInt();

	_float				Read_Float();
	_float2				Read_Float2();
	_float3				Read_Float3();
	_float4				Read_Float4();

private:
	HANDLE		m_FileHandle = { 0 };
	DWORD		m_NumberOfByteRead = { 0 };

	friend CSingleton<CXMLReader>;
};

END