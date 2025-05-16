#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Client)

enum class VFX;
enum class MULTIVFX;

class CEffectLayer final : public CGameObject
{
private:
	explicit CEffectLayer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MULTIVFX);
	explicit CEffectLayer(const CEffectLayer&);
	virtual ~CEffectLayer() DEFAULT;

public:
	virtual void						Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

	virtual HRESULT						Fetch(any ilArgs) override;
	virtual _bool						Return() override;
	virtual HRESULT						Release() override;

public:
	HRESULT								Append_Effect(_float fTime, VFX);

private:
	_float								m_fTimeAcc					= 0.f;
	_float								m_fRecentProcessEventTime	= 0.f;

	multimap<_float, pair<VFX, _bool>>	m_mmapEffectEvents;
	list<any>							m_lstEffectArgs;
	list<any>::iterator					m_itArgs;

	const MULTIVFX						m_eMultiVFX;

public:
	static shared_ptr<CEffectLayer>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MULTIVFX);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END
