#pragma once
#include "Client_Define.h"
#include "ToolSystem.h"

//#define TOOL_DEMO

#if ACTIVATE_TOOL

BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CGameObject;
class CEffect;
class CEffectRegular;
class CEffectInstance;
class CModel;
class CMesh;
class CVIBufferInstance_Mesh;
class ICurves;
END

BEGIN(Client)

class CTool_Effect final : public IToolSystem
{
private:
	explicit CTool_Effect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CTool_Effect() DEFAULT;

public:
	virtual HRESULT										Initialize();
	virtual void										Tick(_float fTimeDelta) override;
	virtual void										Late_Tick(_float fTimeDelta) override;
	virtual HRESULT										Render() override;

private:
	virtual void										Tool_MenuBar() override;
	virtual void										Tool_System() override;
	virtual void										Tool_Properties() override;
	virtual void										Tool_MainControl() override;
	virtual void										Tool_SubControl() override;

private:
	void												Push_SpriteSheet();
	void												Push_EffectRegular(EFFECT, any tDesc);
	void												Push_EffectRegular(shared_ptr<CEffectRegular>, const string& strTag = string());
	void												Push_EffectInstance(EFFECT, any tDesc);
	void												Push_EffectInstance(shared_ptr<CEffectInstance>, const string& strTag = string());

	HRESULT												Save_Effects(string strResourcePath);

	HRESULT												Load_LoadTexture(const string& strLoadTextureFilePath);
	HRESULT												Load_LoadMesh(const string& strLoadMeshFilePath);
	HRESULT												Load_LoadMeshInstance(const string& strLoadMeshFilePath);
	shared_ptr<CEffect>									Load_Effect(const string& strFilePath);
	void												Load_Effects(EFFECT);

	HRESULT												Export_MeshInstance(const string& strFilePath);
	HRESULT												Export_LoadTexture(const string& strLoadTextureFilePath);
	HRESULT												Export_Effect(shared_ptr<CEffect>, const string& strFilePath);

	HRESULT												Push_TrailTarget(const string& strFilePath);

	HRESULT												Bind_DummyMaterials();

#ifdef TOOL_DEMO
	HRESULT												Demo_Plot();
	HRESULT												Demo_Ease();
	HRESULT												Demo_Curve();
#endif

private:
	shared_ptr<CRenderer>								m_pRenderer;
	shared_ptr<CShader>									m_pEffectShader;
	shared_ptr<CShader>									m_pModelShader;
	vector<shared_ptr<CShader>>							m_vecShaders;
	_uint												m_iModelShaderPass				= 0;
	RENDER_GROUP										m_eModelRenderGroup				= RENDER_GROUP::MAX;

	_int												m_iLoadTextureCursor			= -1;
	shared_ptr<CTexture>								m_pLoadTexture;
	vector<shared_ptr<CTexture>>						m_vecLoadTextures;
	vector<string>										m_vecLoadTextureTags;
	
	_int												m_iLoadMeshCursor				= -1;
	shared_ptr<CMesh>									m_pLoadMesh;
	vector<shared_ptr<CMesh>>							m_vecLoadMeshes;
	vector<string>										m_vecLoadMeshTags;

	_int												m_iLoadMeshInstanceCursor		= -1;
	shared_ptr<CVIBufferInstance_Mesh>					m_pLoadMeshInstance;
	vector<shared_ptr<CVIBufferInstance_Mesh>>			m_vecLoadMeshInstances;
	vector<string>										m_vecLoadMeshInstanceTags;

	_int												m_iCurvePlotCursor				= -1;
	shared_ptr<class CCurvePlot>						m_pCurvePlot;
	vector<shared_ptr<class CCurvePlot>>				m_vecCurvePlots;
	vector<string>										m_vecCurvePlotTags;

	_int												m_iSpriteSheetCursor			= -1;
	shared_ptr<CTexture>								m_pSpriteSheet;
	vector<shared_ptr<CTexture>>						m_vecSpriteSheets;
	vector<string>										m_vecSpriteSheetTags;

	_int												m_iEffectRegularCursor			= -1;
	shared_ptr<CEffectRegular>							m_pEffectRegular;
	vector<shared_ptr<CEffectRegular>>					m_vecEffectRegulars;
	vector<string>										m_vecEffectRegularTags;

	_int												m_iEffectInstanceCursor			= -1;
	shared_ptr<CEffectInstance>							m_pEffectInstance;
	vector<shared_ptr<CEffectInstance>>					m_vecEffectInstances;
	vector<string>										m_vecEffectInstanceTags;		

	shared_ptr<CEffect>									m_pEffect;
	_bool												m_bEffectRepeat					= false;
	_bool												m_bEffectTick					= false;

	_float4x4											m_mEffectRegularFetch			= g_mUnit;

	shared_ptr<CTransform>								m_pFetchTransform;
	shared_ptr<CTransform>								m_pDummyTransform;
	shared_ptr<CModel>									m_pTargetModel;
	_uint												m_iAnimationIndex				= 0u;
	_char												m_szBone0[MAX_PATH]{};
	_char												m_szBone1[MAX_PATH]{};

	shared_ptr<class CCurvePlot>						m_pNewCurvePlot;
	shared_ptr<class CCurvePlot>						m_pCurrentCurvePlot;
	shared_ptr<class CCurveContainer>					m_pCurveContainer;

	map<shared_ptr<CEffect>, string, std::owner_less<>>	m_mapEffectPath;

#ifdef TOOL_DEMO
	_bool												m_bDemoPlot						= false;
	_bool												m_bDemoCurve					= false;
	_bool												m_bDemoEase						= false;
#endif

private:
	ImGuiFileDialog										m_imfdEmbed_Load;
	ImGuiFileDialog										m_imfdEmbed_Export;

public:
	static shared_ptr<CTool_Effect>						Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
};

END

#endif
