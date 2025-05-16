#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFileSystem final
{
public:
	static std::wstring Get_RelativePath(const std::wstring& strPath);
	static std::wstring Get_FileName(const std::wstring& strPath);

	static std::wstring Crate_RelativePath(const std::wstring& strExportDir, const std::wstring& strFileName);
};

END