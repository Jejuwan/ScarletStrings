#pragma once
#include "Client_Define.h"
#include "KeyframeGenerator.h"

#ifdef _DEBUG

BEGIN(Client)

class CCameraKeyframeGenerator final : public CKeyframeGenerator
{
private:
	explicit CCameraKeyframeGenerator(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CCameraKeyframeGenerator(const CCameraKeyframeGenerator&);
	virtual ~CCameraKeyframeGenerator() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

private:
	virtual HRESULT									Ready_Components() override;

protected:
	virtual HRESULT									Import(const wstring& wstrImportPath) override;

private:
	std::vector<shared_ptr<class CDebugCamera>>		m_vecCinematicCamera;
	
	_bool											m_bModifyMode = { false };
	_uint											m_iSelectedKeyframe;

public:
	static shared_ptr<CCameraKeyframeGenerator>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END

#endif
