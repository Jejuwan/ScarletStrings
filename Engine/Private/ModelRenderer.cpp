#include "EnginePCH.h"
#include "ModelRenderer.h"

#include "GraphicDevice.h"
#include "Component_Manager.h"
#include "GameInstance.h"
#include "PhysXEngine.h"

#include "Model.h"
#include "VIBufferInstance_Model.h"

#include "Shader.h"
#include "Light.h"

CModelRenderer::CModelRenderer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::MODEL_RENDERER)
{
}

HRESULT CModelRenderer::Initialize(any)
{
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstModel.hlsl"), VTXMESHINSTTRANSFORM::tElements, VTXMESHINSTTRANSFORM::iNumElement);

	return S_OK;
}

shared_ptr<CTransform> CModelRenderer::Add_Instance(const wstring& wstrTag, any pMaterialDesc)
{
	if (wstrTag == TEXT(""))
		int a = 0;
	auto pTransform = CTransform::Create(m_pDevice, m_pContext);

	auto& pModel = m_umapInstances[wstrTag].first;
	if (pMaterialDesc.has_value())
	{
		if (pMaterialDesc.type() == typeid(MATERIALDESC*))
		{
			pModel->Initialize_MaterialDesc(any_cast<MATERIALDESC*>(pMaterialDesc));
		}
	}

	//const auto& pInstanceBuffer = m_umapInstances[wstrTag].second;
	//pInstanceBuffer->Add_Transform(pTransform);

	return pTransform;
}

void CModelRenderer::Set_MaterialDesc(const wstring& wstrTag, MATERIALDESC* pMaterialDesc)
{
	auto& pModel = m_umapInstances[wstrTag].first;
	pModel->Initialize_MaterialDesc(pMaterialDesc);
}

//void CModelRenderer::Update_Instance()
//{
//	for (auto& ModelInstancePair : m_umapInstances)
//	{
//		auto& wstrModelTag = ModelInstancePair.first;
//		auto pModel = ModelInstancePair.second.first;
//		auto pInstanceBuffer = ModelInstancePair.second.second;
//
//		if (pInstanceBuffer)
//			pInstanceBuffer->Update_InstanceBuffer();
//	}
//}

HRESULT CModelRenderer::Add_RenderObject(const RENDER_GROUP eRenderGroup, const INSTMODELDESC& tInstModelDesc, shared_ptr<class CTransform> pTransform)
{
	const wstring& wstrModelTag = tInstModelDesc.wstrModelTag;
	auto& fpBindResource = tInstModelDesc.fpBindResource;
	_uint iPassIndex = tInstModelDesc.iPassIndex;

	auto& tInstanceDesc = m_umapModelRenderGroup[IDX(eRenderGroup)][wstrModelTag];
	if (nullptr == tInstanceDesc.fpBindResource)
	{
		tInstanceDesc.fpBindResource = fpBindResource;
	}

	tInstanceDesc.vecTransform.push_back(pTransform);

	tInstanceDesc.iPassIndex = iPassIndex;

	return S_OK;
}

HRESULT CModelRenderer::Add_RenderObject(const RENDER_GROUP eRenderGroup, const INSTMODELMESHDESC& tInstModelMeshDesc, shared_ptr<class CTransform> pTransform)
{
	const wstring& wstrModelTag = tInstModelMeshDesc.wstrModelTag;
	auto& fpBindResource = tInstModelMeshDesc.fpBindResource;

	auto& tInstanceDesc = m_umapModelMeshRenderGroup[IDX(eRenderGroup)][wstrModelTag];
	if (nullptr == tInstanceDesc.fpBindResource)
	{
		tInstanceDesc.fpBindResource = fpBindResource;
	}

	tInstanceDesc.vecTransform.push_back(pTransform);

	tInstanceDesc.vecPassIndex = tInstModelMeshDesc.vecPassIndex;
	tInstanceDesc.vecMeshIndex = tInstModelMeshDesc.vecMeshIndex;

	return S_OK;
}

HRESULT CModelRenderer::Render(const RENDER_GROUP eRenderGroup)
{
	for (auto& tInstance : m_umapModelRenderGroup[IDX(eRenderGroup)])
	{
		const wstring& wstrTag = tInstance.first;
		auto fpBindResourceCall = tInstance.second.fpBindResource;

		auto& vecInstanceTransform = tInstance.second.vecTransform;
		_uint iDrawCount = vecInstanceTransform.size()	;
		_uint iPassIndex = tInstance.second.iPassIndex;

		const auto& tModelInstancePair = m_umapInstances[wstrTag];
		auto pModel = tModelInstancePair.first;
		auto pInstanceBuffer = tModelInstancePair.second;
		pInstanceBuffer->Update_InstanceBuffer(vecInstanceTransform);
		vecInstanceTransform.clear();

		if (fpBindResourceCall && FAILED(fpBindResourceCall(pModel, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to RenderCall");
		}

		pInstanceBuffer->Bind_InstanceBuffer(1);

		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to Bind_Matrix : View");
		}
		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to Bind_Matrix : View");
		}

		if (FAILED(m_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to CShader::Bind_Vector");
		}

		pModel->Render_Instance_Model(iDrawCount, m_pShader, iPassIndex);
	}
	m_umapModelRenderGroup[IDX(eRenderGroup)].clear();

	for (auto& tInstance : m_umapModelMeshRenderGroup[IDX(eRenderGroup)])
	{
		const wstring& wstrTag = tInstance.first;
		auto fpBindResourceCall = tInstance.second.fpBindResource;

		auto& vecInstanceTransform = tInstance.second.vecTransform;
		_uint iDrawCount = vecInstanceTransform.size();

		vector<_uint> vecMeshIndex = tInstance.second.vecMeshIndex;
		vector<_uint> vecPassIndex = tInstance.second.vecPassIndex;

		const auto& tModelInstancePair = m_umapInstances[wstrTag];
		auto pModel = tModelInstancePair.first;
		auto pInstanceBuffer = tModelInstancePair.second;
		pInstanceBuffer->Update_InstanceBuffer(vecInstanceTransform);
		vecInstanceTransform.clear();

		if (fpBindResourceCall && FAILED(fpBindResourceCall(pModel, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to RenderCall");
		}




		pInstanceBuffer->Bind_InstanceBuffer(1);

		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to Bind_Matrix : View");
		}
		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to Bind_Matrix : View");
		}

		if (FAILED(m_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to CShader::Bind_Vector");
		}

		for (size_t i = 0; i < vecMeshIndex.size(); ++i)
		{
			pModel->Render_Instance_Model(iDrawCount, m_pShader, vecPassIndex[i], vecMeshIndex[i]);
		}
	}
	m_umapModelMeshRenderGroup[IDX(eRenderGroup)].clear();

	return S_OK;
}

HRESULT CModelRenderer::Render(const RENDER_GROUP eRenderGroup, shared_ptr<CLight> pShadow)
{
	for (auto& tInstance : m_umapModelRenderGroup[IDX(eRenderGroup)])
	{
		const wstring& wstrTag = tInstance.first;
		auto fpBindResourceCall = tInstance.second.fpBindResource;

		auto& vecInstanceTransform = tInstance.second.vecTransform;
		_uint iDrawCount = vecInstanceTransform.size();

		_uint iPassIndex = tInstance.second.iPassIndex;

		const auto& tModelInstancePair = m_umapInstances[wstrTag];
		auto pModel = tModelInstancePair.first;
		auto pInstanceBuffer = tModelInstancePair.second;
		pInstanceBuffer->Update_InstanceBuffer(vecInstanceTransform);
		vecInstanceTransform.clear();

		if (fpBindResourceCall && FAILED(fpBindResourceCall(pModel, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CModelRenderer::Render", "Failed to RenderCall");
		}

		pInstanceBuffer->Bind_InstanceBuffer(1);

		if (FAILED(pShadow->Bind_ShadowMatrix(m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render_ShadowDepth", "Failed to CLight::Bind_ShadowMatrix");
		}

		if (FAILED(m_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
		{
			MSG_RETURN(E_FAIL, "CModel::Bind_Buffer", "Failed to CShader::Bind_Vector");
		}

		pModel->Render_Instance_Model(iDrawCount, m_pShader, iPassIndex);
	}
	m_umapModelRenderGroup[IDX(eRenderGroup)].clear();

	return S_OK;
}

void CModelRenderer::Hide_Mesh(const wstring& wstrTag, _uint iMeshIndex)
{
	m_umapInstances[wstrTag].first->Hide_Mesh(iMeshIndex);
}

#ifdef _DEBUG
void CModelRenderer::Iterate_Models(function<_bool(wstring, shared_ptr<CModel>)> funcCallback)
{
	for (auto& Iter : m_umapInstances)
	{
		auto pModel = Iter.second.first;
		if (false == funcCallback(Iter.first, pModel))
		{
			return;
		}
	}
}
#endif

void CModelRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < IDX(RENDER_GROUP::MAX); ++i)
	{
		for (auto& RenderDescPair : m_umapModelRenderGroup[i])
		{
			INSTANCERENDERDESC& tInstanceDesc = RenderDescPair.second;
			tInstanceDesc.vecTransform.clear();
		}
		for (auto& RenderDescPair : m_umapModelMeshRenderGroup[i])
		{
			INSTANCERENDERDESC& tInstanceDesc = RenderDescPair.second;
			tInstanceDesc.vecTransform.clear();
		}
	}
}

void CModelRenderer::Bake_Physics(const wstring& wstrTag, shared_ptr<class CTransform> pTransform, _bool CameraIntersect)
{
	vector<PxRigidStatic*> vecActor = Create_Actor(wstrTag, pTransform, CameraIntersect);
	for (size_t i = 0; i < vecActor.size(); ++i)
	{
		CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->addActor(*vecActor[i]);
	}
}

vector<PxRigidStatic*> CModelRenderer::Create_Actor(const wstring& wstrTag, shared_ptr<class CTransform> pTransform, _bool CameraIntersect)
{
	auto pModel = m_umapInstances[wstrTag].first;

	_float4x4 mat = pTransform->Get_Matrix();
	_vector Rotation, Scale, Pos;
	XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

	*(_float4*)mat.m[0] = XMVector3Normalize(*(_float4*)mat.m[0]);
	*(_float4*)mat.m[1] = XMVector3Normalize(*(_float4*)mat.m[1]);
	*(_float4*)mat.m[2] = XMVector3Normalize(*(_float4*)mat.m[2]);
	Rotation = XMQuaternionRotationMatrix(mat);

	PxTransform vTransform = PxTransform(PxIdentity);
	vTransform.p = PxVec3(mat._41, mat._42, mat._43);
	_float4 vQuat;
	XMStoreFloat4(&vQuat, Rotation);
	vTransform.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	
	vector<PxRigidStatic*> vecActor = Create_Actor(vTransform, PxMeshScale(PxVec3(XMVectorGetX(Scale), XMVectorGetY(Scale), XMVectorGetZ(Scale))), pModel, CameraIntersect);

	return vecActor;
}

vector<PxRigidStatic*> CModelRenderer::Create_Actor(PxTransform vTransform, PxMeshScale vScale, shared_ptr<class CModel> pModel, _bool CameraIntersect)
{
	vector<PxRigidStatic*> vecActor;
	vecActor.reserve(pModel->Get_TriangleMeshes().size());

	for (int i = 0; i < pModel->Get_TriangleMeshes().size(); i++)
	{
		PxRigidStatic* hfActor = CGameInstance::Get_Instance()->Get_PhysXEngine()->GetPhysics()->createRigidStatic(vTransform);

		PxTriangleMeshGeometry hfGeom(pModel->Get_TriangleMeshes()[i], vScale);

		PxShape* hfShape = PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, *CGameInstance::Get_Instance()->Get_PhysXEngine()->GetMaterial());

		if (!CameraIntersect)
		{
			shared_ptr<ACTORDESC> ActorDesc = make_shared<ACTORDESC>();
			hfActor->userData = ActorDesc.get();
			m_vecActorDesc.push_back(ActorDesc);
		}

		//PxFilterData interactiveLayerFilterData;
		//interactiveLayerFilterData.word0 = (_uint)ACTORTAG::STATIC;
		//hfShape->setSimulationFilterData(interactiveLayerFilterData);
		//PxSetGroup(*hfActor, (_uint)ACTORTAG::STATIC);

		vecActor.push_back(hfActor);
	}

	return vecActor;
}

HRESULT CModelRenderer::Create_Model_Instance(const wstring& wstrTag, shared_ptr<CModel> pModel, _uint iNumInstance)
{
	m_umapInstances[wstrTag] = { pModel, CVIBufferInstance_Model::Create(m_pDevice, m_pContext, iNumInstance) };

	return S_OK;
}

shared_ptr<CModelRenderer> CModelRenderer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CModelRenderer> pInstance = make_private_shared(CModelRenderer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CModelRenderer::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CModelRenderer::Clone(any)
{
	return shared_from_componenet();
}
