#pragma once
#include "Map_Dynamic.h"

BEGIN(Client)
class CMap_Special_Dynamic :
    public CMap_Dynamic
{
protected:
	explicit CMap_Special_Dynamic(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CMap_Special_Dynamic(const CMap_Special_Dynamic&);
	virtual ~CMap_Special_Dynamic() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_OutLine() override;

protected:
	virtual HRESULT						Ready_Components() override;

protected:
	shared_ptr<CTransform> m_pCamTransform;

	SPECIAL_THROW m_eType = SPECIAL_THROW::MAX;
	_float m_fThrowReadyTime = 0.f;
	_bool m_bThrowStart = false;
	_bool							m_bIsRender = { true };

	_float m_fWorldThreshHold = 0.f;
	_float m_fScreenThreshHold = 0.f;
private:
	void	Add_ModelCom();
	void	Add_SpecialType();
public:
	static shared_ptr<CMap_Special_Dynamic>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _int iIndex);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;

public:
	virtual void Throw() override;
	virtual void ThrowHit() override;
	void Set_ThrowStart(_bool b) { m_bThrowStart = b; }
	void Set_ThrowTime(_float f) { m_fThrowTime = f; }
	SPECIAL_THROW	Get_Type() { return m_eType; }

	void	Set_WorldThreshHold(_float f) { m_fWorldThreshHold = f; };
	void	Set_ScreenThreshHold(_float f) { m_fScreenThreshHold = f; }
	_float  Get_WorldThreshHold() { return m_fWorldThreshHold; }
	_float  Get_ScreenThreshHold() { return m_fScreenThreshHold; }
	void Set_Render(_bool b) { m_bIsRender = b; }
};
END
