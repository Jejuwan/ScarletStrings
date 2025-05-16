#pragma once
#include "Client_Define.h"
#include "ToolSystem.h"

#if ACTIVATE_TOOL

BEGIN(Engine)
class CRenderer;
class CTransform;
class CShader;
class CModel;
class CAnimation;
class CBone;
class CChannel;
class CMesh;
class CTexture;
class CEvent;
END

BEGIN(Client)

class CTool_Model final : public IToolSystem
{
private:
	enum class MODELFOCUS	{ BONE, ANIMATION, MESH, MATERIAL, MAX };

private:
	explicit CTool_Model(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CTool_Model() DEFAULT;

public:
	virtual HRESULT							Initialize();
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;
	virtual HRESULT							Render() override;

private:
	virtual void							Tool_MenuBar() override;
	virtual void							Tool_System() override;
	virtual void							Tool_Properties() override;
	virtual void							Tool_MainControl() override;
	virtual void							Tool_SubControl() override;

private:
	HRESULT									Push_Model(const wstring& wstrFilePath, const MODEL, _matrixf mPivot);
	HRESULT									Export_Model(const wstring& wstrFilePath);

	HRESULT									Export_Animation(const wstring& wstrFilePath);

private:
	HRESULT									Bind_DummyMaterials();

private:
	shared_ptr<CRenderer>					m_pRenderer;
	shared_ptr<CShader>						m_pShader;
	vector<shared_ptr<CShader>>				m_vecShaders;
	RENDER_GROUP							m_eRenderGroup			= RENDER_GROUP::PRIORITY;
	_uint									m_iShaderPass			= 0;
	shared_ptr<CTransform>					m_pDummyTransform;

	_int									m_iAnimModelCursor		= -1;
	shared_ptr<CModel>						m_pAnimModel;
	vector<shared_ptr<CModel>>				m_vecAnimModels;
	vector<string>							m_vecAnimModelTags;

	_int									m_iNonAnimModelCursor	= -1;
	shared_ptr<CModel>						m_pNonAnimModel;
	vector<shared_ptr<CModel>>				m_vecNonAnimModels;
	vector<string>							m_vecNonAnimModelTags;

	_int									m_iBoneCursor			= -1;
	shared_ptr<CBone>						m_pBone;

	_int									m_iAnimationCursor		= -1;
	shared_ptr<CAnimation>					m_pAnimation;
	_bool									m_bAnimationPlay		= true;
	_int									m_iChannelCursor		= 0;
	shared_ptr<CChannel>					m_pChannel;
	
	_int									m_iTrackPosition		= 0;
	_int									m_iTrackPositionMax		= 0;
	_int									m_iEventType			= 0;
	_int									m_iEventCursor			= 0;
	shared_ptr<CEvent>						m_pEvent;
	_float									m_fSoundVolume			= 0;
	_float									m_fAnimSpeed			= 0;
	string									m_strSoundFileName;
	
	_int									m_iKeyFrameCursor		= 0;
	KEYFRAME								m_tKeyFrame{};
	_int									m_iCompareAnimationIdx	= 0;

	_int									m_iMeshCursor			= -1;
	shared_ptr<CMesh>						m_pMesh;

	_int									m_iMaterialCursor		= -1;
	MATERIAL								m_tMaterial{};
	_int									m_iTextureCursor		= -1;
	shared_ptr<CTexture>					m_pTexture;

	shared_ptr<CModel>						m_pModel;
	MODELFOCUS								m_eFocus				= MODELFOCUS::MAX;

private:
	ImGuiFileDialog							m_imfdEmbed_Load;
	ImGuiFileDialog							m_imfdEmbed_Export;

public:
	static shared_ptr<CTool_Model>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
