#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModelRenderer : public CComponent
{
private:
	explicit CModelRenderer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CModelRenderer() DEFAULT;

public:
	HRESULT									Initialize(any = g_aNull);

public:
	shared_ptr<class CTransform>			Add_Instance(const wstring& wstrTag, any pMaterialDesc = g_aNull);
	void									Set_MaterialDesc(const wstring& wstrTag, MATERIALDESC* pMaterialDesc);
	//void									Update_Instance();

public:
	HRESULT									Add_RenderObject(const RENDER_GROUP, const INSTMODELDESC& tInstModelDesc, shared_ptr<class CTransform>);
	HRESULT									Add_RenderObject(const RENDER_GROUP, const INSTMODELMESHDESC& tInstModelMeshDesc, shared_ptr<class CTransform>);
	HRESULT									Render(const RENDER_GROUP);
	HRESULT									Render(const RENDER_GROUP, shared_ptr<class CLight> pShadow);

	void									Hide_Mesh(const wstring& wstrTag, _uint iMeshIndex);

#ifdef _DEBUG
public:
	void									Iterate_Models(function<_bool(wstring, shared_ptr<CModel>)> _funcCallback);
#endif

public:
	void									Clear_RenderGroup();

public:
	void									Bake_Physics(const wstring& wstrTag, shared_ptr<class CTransform> , _bool CameraIntersect = true);
	vector<PxRigidStatic*>					Create_Actor(const wstring& wstrTag, shared_ptr<class CTransform>, _bool CameraIntersect);
	
	vector<PxRigidStatic*>					Create_Actor(PxTransform vTransform, PxMeshScale vScale, shared_ptr<class CModel> pModel, _bool CameraIntersect);

public:
	HRESULT									Create_Model_Instance(const wstring& wstrTag, shared_ptr<class CModel> pModel, _uint iNumInstance);

private:
	unordered_map<wstring, INSTANCERENDERDESC>			m_umapModelRenderGroup[IDX(RENDER_GROUP::MAX)];
	unordered_map<wstring, INSTANCERENDERDESC>		m_umapModelMeshRenderGroup[IDX(RENDER_GROUP::MAX)];

private:
	unordered_map<wstring, pair<shared_ptr<class CModel>, shared_ptr<class CVIBufferInstance_Model>>> m_umapInstances;

	shared_ptr<class CShader>												m_pShader;
	vector<shared_ptr<ACTORDESC>>											m_vecActorDesc;

public:
	static shared_ptr<CModelRenderer>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>			Clone(any = g_aNull) override;
};

END