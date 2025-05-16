#pragma once
#include "Monster.h"
#include "physx/PxPhysicsAPI.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CTexture;
END

BEGIN(Client)

class CMonster_Statue final : public CMonster
{
public :
	enum TYPE {
		MASK_WHITE,
		MASK_BLACK,
		MASK_RED,
		TYPE_END
	};

	struct STATUEDESC {
		_float4 InitPosition;
		TYPE	MaskType;
	};
private:
	explicit CMonster_Statue(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMonster_Statue(const CMonster_Statue&);
	virtual ~CMonster_Statue() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Screen_Effect() override;
	virtual HRESULT						Render_ShadowDepth(shared_ptr<class CLight>) override;

	virtual void						Update_Motion(_float fTimeDelta) override;

	void BlackMaskPivotChance();

	virtual HRESULT						Render_OutLine() override;
	virtual wstring						Get_TypeName() { return L"·¹º§ ?? ???"; }; 
	void								ChangeNextState()
	{
		m_CurruntType = TYPE(m_CurruntType + 1);
		if (m_CurruntType >= TYPE_END)
			m_CurruntType = MASK_WHITE;
	};
	bool Get_MyTurn() {	return m_CurruntType == m_MaskType;	};
	TYPE Get_MaskType() {	return m_MaskType;	};

public:
	virtual HRESULT						Fetch(any Any = g_aNull) override;
	shared_ptr<class CPart>				Get_Part(_uint iPartIndex);
	
private :
	virtual HRESULT						Ready_Components() override;
	virtual HRESULT						Ready_Behaviors() override;
	HRESULT								Ready_Parts();

private :
	vector<shared_ptr<class CPart>>		m_vecPart;
	TYPE								m_MaskType = { TYPE::TYPE_END };
	static TYPE							m_CurruntType;
	_bool								m_BeginAct = { false };
	_bool								m_EndAct = { false };

private:
	_bool								m_bFetched = { false };

private:
	shared_ptr<class CTexture>			m_pNoiseTexture;

	_float								m_fGlitchSeed = { 0.f };

	_float								m_fSeedAccTime = { 0.f };
	_float								m_fSeedInterval = { 0.1f };

private:
	_bool								m_bExecuteGrainEffect = { false };
	_float3								m_vGrainFactor = { 0.f, 0.f, 0.f };

	_bool								m_bDecreaseGrainFactor = { false };
	_float								m_fMaxGrainFactorThreshold = { 0.3f };

	_uint								m_iPoolIdx = 0;
	_bool								m_bExecuted[10] = { false };
	weak_ptr<class CMonster_Karen>		m_pKaren;
public:
	static shared_ptr<CMonster_Statue>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;	
	void Set_PoolIdx(_uint i) { m_iPoolIdx = i; }
	_uint Get_PoolIdx(){return m_iPoolIdx;}

};

END
