#pragma once
#include "Client_Define.h"
#include "KeyframeGenerator.h"

BEGIN(Engine)
class CObjectLayer;
END

BEGIN(Client)

class CObjectKeyframeGenerator final : public CKeyframeGenerator
{
private:
	explicit CObjectKeyframeGenerator(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CObjectKeyframeGenerator(const CObjectKeyframeGenerator&);
	virtual ~CObjectKeyframeGenerator() DEFAULT;

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
	_bool											Iterate_Layers(pair<wstring, shared_ptr<CObjectLayer>>);

private:
	wstring											wstrSelectedObjectLayer;
	wstring											wstrSelectedObjectTag;

private:
	weak_ptr<CGameObject>							m_pSelectedObject;

	shared_ptr<CRenderer>							m_pRenderer;

public:
	static shared_ptr<CObjectKeyframeGenerator>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END