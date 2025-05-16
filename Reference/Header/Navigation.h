#pragma once

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)
class CCell;

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		const _float4x4* pWorldMatrix = { nullptr };
		_int				iCurrentIndex = { -1 };
	}NAVIDESC;

private:
	explicit CNavigation(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CNavigation(const CNavigation&);
	virtual ~CNavigation() DEFAULT;

public:
	virtual HRESULT Initialize_Prototype(any Any);
	HRESULT Initialize(any Any);

public:
	_float3 Get_FirstPos() { return m_vFirstPos; };
	_float3 Get_SecondPos() { return m_vSecondPos; };

#ifdef ACTIVATE_TOOL
public:

	_bool is_Move(_vectorf vPosition);

#ifdef ACTIVATE_TOOL
public:
	virtual HRESULT Render();
#endif

private:
	HRESULT SetUp_Neighbors();

private:
	vector<shared_ptr<class CCell>>			m_Cells;
	typedef vector<shared_ptr<class CCell>>	CELLS;

private:
	const _float4x4* m_pWorldMatrix = { nullptr };

	_int			m_iCurrentIndex = { -1 };

#ifdef ACTIVATE_TOOL
private:
	shared_ptr<class CShader> m_pShader = { nullptr };
#endif

private:
	_float3				m_vFirstPos = { 0.f,0.f,0.f };
	_float3				m_vSecondPos = { 0.f,0.f,0.f };

private:
	static shared_ptr<CNavigation> Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, any Any);
	virtual shared_ptr<CComponent>			Clone(any = g_aNull) override;

};

END