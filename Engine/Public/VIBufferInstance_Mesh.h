#pragma once
#include "Engine_Define.h"
#include "VIBufferInstance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance_Mesh final : public CVIBufferInstance
{
private:
	explicit CVIBufferInstance_Mesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBufferInstance_Mesh(const CVIBufferInstance_Mesh&);
	virtual ~CVIBufferInstance_Mesh() DEFAULT;

public:
	HRESULT										Initialize_Prototype(const wstring& wstrPath, _uint iNumInstance);
	HRESULT										Initialize_Prototype(MODEL, shared_ptr<class CMesh>, _uint iNumInstance);
	HRESULT										Initialize_Prototype(std::ifstream&);
	HRESULT										Initialize(any = g_aNull);

public:
	const _char*								Get_Name() const	{ return m_szName; }

private:
	_char										m_szName[MAX_PATH]	= "";
	_uint										m_iMaterialIndex	= 0;

	_uint										m_iNumBones			= 0;
	vector<_uint>								m_vecBoneIndices;
	vector<_float4x4>							m_vecBoneOffsets;
	unique_ptr<VTXMESH[]>						m_pVertices_NonAnim;
	unique_ptr<VTXMESHANIM[]>					m_pVertices_Anim;

public:
	static shared_ptr<CVIBufferInstance_Mesh>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrPath, _uint iNumInstance);
	static shared_ptr<CVIBufferInstance_Mesh>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MODEL, shared_ptr<class CMesh>, _uint iNumInstance);
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;

	static shared_ptr<CVIBufferInstance_Mesh>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrPath);
	static shared_ptr<CVIBufferInstance_Mesh>	Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	void										Export(std::ofstream&);
	HRESULT										Export(const wstring& wstrPath);
#endif
};

END
