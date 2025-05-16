#pragma once
#include "Client_Define.h"
#include "GameObject.h"

#ifdef _DEBUG

BEGIN(Client)

class CBattleFieldGenerator : public CGameObject
{
private:
	explicit CBattleFieldGenerator(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CBattleFieldGenerator(const CBattleFieldGenerator&);
	virtual ~CBattleFieldGenerator() DEFAULT;

public:
	virtual HRESULT									Initialize_Prototype() override;
	virtual HRESULT									Initialize(any = g_aNull) override;
	virtual void									Tick(_float fTimeDelta) override;
	virtual void									Late_Tick(_float fTimeDelta) override;
	virtual HRESULT									Render() override;

public:
	void											Bind_MapTool(shared_ptr<class CMap_Tool> pMapTool);

private:
	virtual HRESULT									Ready_Components() override;

protected:
	virtual HRESULT									Import(const wstring& wstrImportPath);
	virtual HRESULT									Export(const wstring& wstrExportPath);

private:
	void											Create_BattleField(_vector vLeft, _vector vRight, shared_ptr<CTransform> pTransform);

#if ACTIVATE_IMGUI
	void											Render_File_Dialog();
#endif

private:
	_float3											m_vCurrentPoint = { 0.f, 0.f, 0.f };
	_bool											m_bPickingMode = { false };

private:
	shared_ptr<class CMap_Tool>						m_pMapTool;

private:
	std::list<shared_ptr<class CPointGizmo>>		m_lstPointGizmo;
	std::list<shared_ptr<class CBattleFieldWall>>	m_lstBattleField;

private:
	shared_ptr<class CPointGizmo>					m_pSelectedGizmo;
	_int											m_iSelectedGizmoIndex;

private:
#if ACTIVATE_IMGUI
	ImGuiFileDialog									m_imfdEmbed_Load;
	ImGuiFileDialog									m_imfdEmbed_Export;
#endif

public:
	static shared_ptr<CBattleFieldGenerator>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CGameObject>					Clone(any = g_aNull) override;
};

END

#endif