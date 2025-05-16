#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Sphere;
class CInteractiveObject;
END

BEGIN(Client)

class CPsychokinesis final : public CGameObject
{
private:
	explicit CPsychokinesis(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CPsychokinesis(const CPsychokinesis&);
	virtual ~CPsychokinesis() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

public:
	void								Capture(_uint iAnimIndex);

private:
	virtual HRESULT						Ready_Components() override;

private:
	HRESULT								Initialize_Keyframe(const wstring& _wstrModelPath);
	_bool								Interpolate_Keyframe(const KEYFRAME& tKeyframe, const shared_ptr<CTransform> pTransform,
										_float fInterpolatedTime, _float fDistanceThreshold = 0.f);

private:
	shared_ptr<CTransform>				m_pPlayerTransform;

private:
	_bool								m_bCapture = { false };
	_float								m_fChargingTime = { 0.f };
	_float								m_fSpecialThrowTime = { 0.f };

	_bool								m_bThrowing[2] = {false};
	_bool								m_bSpecialThrow = { false };
	_bool								m_bStrong = { false };
	_bool								m_bThrowHit = { false };
	_bool								m_bDuplicate = { false };

private:
	vector<pair<ANIMATIONDESC, vector<KEYFRAME>>>			m_vecKeyframeAnimation;

	_uint										m_iFixedIndex = { 7 };
	_uint										m_iCurrentAnimationIndex = { m_iFixedIndex };

	_float										m_fTrackPosition[2] = {0.f};
	_uint										m_iCurrentKeyframeIndex[2] = {0};

	_float										m_fStrongTime = { 1.f };
	_float										m_fThrowHitTime = { 3.f };

	_int										m_iCombo = 0;
	_float m_EffectTimer = { 1.f };

private:
	shared_ptr<CInteractiveObject>				m_pCapturedObject;
	shared_ptr<CInteractiveObject>				m_pRecentThrowObject = { nullptr };
	shared_ptr<class CMap_Dynamic>				m_pDuplicatedObject;
	shared_ptr<class CMap_Special_Dynamic>	m_pSpecialThrowObject;
	class CPlayer*								m_pPlayer = { nullptr };
	_bool										m_bExecuted[100] = { false };

public:
	void Reset();
	void DuplicateReset();
	void DestroyObject();
public:
	static shared_ptr<CPsychokinesis>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
	shared_ptr<CInteractiveObject>	Get_CapturedObject() {
		return m_pCapturedObject;
	}
	void							Set_CapturedObject(shared_ptr<CInteractiveObject> _obj)
	{
		m_pCapturedObject = _obj;
	}
	shared_ptr<CInteractiveObject>	Get_RecentThrowdObject() {
		return m_pRecentThrowObject;
	}
	void Set_RecentThrowObject(shared_ptr<CInteractiveObject> _obj) { m_pRecentThrowObject = _obj; }
	void Set_DuplicatedObject(shared_ptr<class CMap_Dynamic> _obj) {
		m_pDuplicatedObject = _obj;
		m_bThrowing[1] = true;
		m_fTrackPosition[1] = 0.f;
		m_iCurrentKeyframeIndex[1] = 0;
	}
	shared_ptr<class CMap_Special_Dynamic>	Get_SpecialThrowObject() { return m_pSpecialThrowObject; }
	void Set_SpecialThrowObject(shared_ptr<class CMap_Special_Dynamic> _obj) {
		m_pSpecialThrowObject = _obj;
	}


	void Set_Player(class CPlayer* p) { m_pPlayer = p; }
	_bool Get_Strong() { return m_bStrong; }
	void Set_Strong(_bool b) { m_bStrong = b; }
	_bool Get_ThrowHit() { return m_bThrowHit; }
	void Set_ThrowHit(_bool b) {
		m_bThrowHit = b;
		if (b)
			m_fThrowHitTime = 3.f;
	}
	void Set_CopyThrow() { m_bThrowing[1] = true; }
	void Set_SpecialThrow(_bool b) { m_bSpecialThrow = b; }
	_bool Get_SpecialThrow() { return m_bSpecialThrow; }
	void Set_Combo(_int i) { m_iCombo = i; }
	_int Get_Combo() { return m_iCombo; }
	_bool Get_Throwing() { return m_bThrowing[0]; }
	void Set_Capture(_bool b) { m_bCapture = b; }

	void Change_BF_Attack(_uint idx, _float fTimeDelta);
};

END