#pragma once
#include "Client_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
END

BEGIN(Client)

class CRedLaserEye : public CGameObject
{
public:
	enum class LASER_TYPE
	{
		NONE,
		SMALL,
		BIG,
	};

public:
	typedef struct tagLaserKeyframe
	{
		_float fDuration;
		_float4 vLaserColor;
		_float3 vLaserScale;
	} LASERKEYFRAME;

private:
	explicit CRedLaserEye(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CRedLaserEye(const CRedLaserEye&);
	virtual ~CRedLaserEye() DEFAULT;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(any = g_aNull) override;
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;
	virtual HRESULT						Render_Neon() override;

public:
	virtual void						Set_Enable(_bool bEnable) override;
	void								Set_Laser_Type(LASER_TYPE eLaserType);

public:
	void								Push_Keyframe(const LASERKEYFRAME& tLaserKeyframe);

private:
	virtual HRESULT						Ready_Components() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CShader>					m_pShader;
	shared_ptr<CVIBuffer_Rect>			m_pVIBuffer_Rect;

private:
	const _float4x4*					m_pLeftEyeCombineBone;
	shared_ptr<CTransform>				m_pPlayerTransform;

private:
	shared_ptr<CTexture>				m_pSmallLaser;
	shared_ptr<CTexture>				m_pBigLaser;

private:
	LASER_TYPE							m_eLaserType = { LASER_TYPE::NONE };

private:
	queue<LASERKEYFRAME>				m_queLaserKeyframe;
	_float								m_fAccTime = { 0.f };

private:
	_float4								m_vPrevColor = { 1.5f, 0.f, 0.f, 1.f };
	_float3								m_vPrevScale = { 3.f, 1.f, 1.f };

public:
	static shared_ptr<CRedLaserEye>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>		Clone(any = g_aNull) override;
};

END