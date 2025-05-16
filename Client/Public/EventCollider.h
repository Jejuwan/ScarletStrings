#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CModelRenderer;
END

BEGIN(Client)

class CEventCollider final : public CGameObject
{
private:
	explicit CEventCollider(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CEventCollider(const CEventCollider&);
	virtual ~CEventCollider() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	EVENT_TYPE							Get_EventType() { return m_eEventType; }
	void								Set_EventType(EVENT_TYPE eEventType) { m_eEventType = eEventType; }

	string&								Get_EventTag() { return m_strEventTag; }
	void								Set_EventTag(const string strEventTag) { m_strEventTag = strEventTag; }

	void								Subscribe_Enter_Callback(function<void(EVENT_TYPE, string)> fpOnEnter) { m_fpOnEnter = fpOnEnter; }

private:
	virtual HRESULT						Ready_Components() override;

private:
	void								Import(const wstring& wstrFilePath);

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;

	shared_ptr<CModelRenderer>			m_pModelRenderer;

private:
	EVENT_TYPE							m_eEventType = EVENT_TYPE::NONE;
	string								m_strEventTag;

private:
	function<void(EVENT_TYPE, string)>	m_fpOnEnter;

private:
	INSTMODELDESC						m_tInstDebugCubeDesc;

private:
	shared_ptr<class CPhysXCollider>	m_pEventCollider;
	ACTORDESC*							m_pActorDesc = nullptr;

public:
	static shared_ptr<CEventCollider>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END