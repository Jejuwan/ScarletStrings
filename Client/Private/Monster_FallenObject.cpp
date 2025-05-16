#include "ClientPCH.h"
#include "Monster_FallenObject.h"
#include "Player.h"
#include "PlayerSpecialThrowState.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "Model.h"
#include "Monster_Statue.h"
#include "Frustom.h"
#include "Psychokinesis.h"
#include "Bone.h"
#include "Camera_Player.h"
#include "UI_Indicator_SpcObjectAttack_Rotation_Beam.h"
#include "UI_Indicator_SpcObjectAttack_Rock.h"
#include "Effect_Manager.h"

CMonster_FallenObject::CMonster_FallenObject(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMap_Special_Dynamic(_pDevice, _pContext)
{
}

CMonster_FallenObject::CMonster_FallenObject(const CMonster_FallenObject& _rhs)
	: CMap_Special_Dynamic(_rhs)
	, m_Type(_rhs.m_Type)
{

}

CMonster_FallenObject::~CMonster_FallenObject()

{
}	

HRESULT CMonster_FallenObject::Initialize_Prototype(_uint Type)
{
    m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
    m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

    m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
    m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);
	switch (Type)
	{
	case 0:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_BOX, g_aNull);
		break;
	case 1:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEA, g_aNull);
		break;
	case 2:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_CRATEB, g_aNull);
		break;
	case 3:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_GRILL, g_aNull);
		break;
	case 4:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA_PIECE, g_aNull);
		break;
	case 5:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0, g_aNull);
		break;
	case 6:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0, g_aNull);
		break;
	case 7:
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_MINDROOM_LONG, g_aNull);
		break;
	default:
		cout << "dddang" << endl;
		break;
	}
	m_Type = Type;
    return S_OK;
}

HRESULT CMonster_FallenObject::Initialize(any pArg)
{
    m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
    m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
    m_tMaterialDesc.fShininess = 64.f;

    if (FAILED(CMap_Dynamic::Initialize(pArg)))
    {
        MSG_RETURN(E_FAIL, "CMap_MindRoom_Dynamic::Initialize", "Failed to CGameObject::Initialize");
    }

    //Add_ModelCom();
	auto pGameInstance = CGameInstance::Get_Instance();

    Create_Light(m_pTransform);

	if (m_Type == 4 || m_Type == 6)
	{
		std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
			any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

		m_copyTuplePivot = tObjTuple;

		PxTransform pose = PxTransform(PxIdentity);
		_float3 Pos = std::get<4>(m_copyTuplePivot);
		pose.p = PxVec3(Pos.x, Pos.y, Pos.z);

		_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

		m_Scale = std::get<2>(tObjTuple);
		m_Translation = std::get<4>(tObjTuple);
		m_Mat = std::get<5>(tObjTuple);
		m_pTransform->Set_Matrix(m_Mat);
		m_pTransform->Set_Scale(m_Scale);

		m_ActorDesc.strTag = ACTORTAG::INTERACTIVE_SPECIAL;
		m_ActorDesc.bThrow = false;
		m_ActorDesc.bThrowHit = false;
		m_ActorDesc.stAttackDesc.iDamage = 3.f;

		m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);

		m_pActor->userData = &m_ActorDesc;

		if(m_Type ==4 )
		{
			m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(3.2f + 4.f, 0.3f + 2.f, 0.5f + 2.f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
			m_vLocalPose = { 0,0.8,0 };
		}
		else if (m_Type == 6)
		{
			m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(1.f, 2.f, 1.2f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
			m_vLocalPose = { 0,1.8f,0 };
		}
		m_pShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));


		m_pActor->attachShape(*m_pShape);
		SetPhysX(true);


		m_fMass = 150000.f;
		m_fThrowForce = 150000.f;
		PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
		pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);


		m_pActorDesc = (ACTORDESC*)m_pActor->userData;
		m_tEntityDesc.fHP = 20.f;

		//m_pController->setFootPosition(PxExtendedVec3(XMVectorGetX(InitPos), XMVectorGetY(InitPos), XMVectorGetZ(InitPos)));
		m_bFetched = true;

		m_pTransform->Set_State(TRANSFORM::POSITION, Pos);
	}

	switch (m_Type)
	{
	case 3:
		m_bCanPick = true;
		m_fPickWorldDistance = 15.f;
		m_eType = SPECIAL_THROW::BEAM;
		break;
	case 4:
		m_eType = SPECIAL_THROW::BEAM;
		m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		break;
	case 5:
		m_bCanPick = true;
		m_fPickWorldDistance = 15.f;
		m_eType = SPECIAL_THROW::FINAL_ROCK;
		break;
	case 6:
		m_eType = SPECIAL_THROW::FINAL_ROCK;
		m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		break;
	case 7:
		m_bCanPick = true;
		m_fPickWorldDistance = 15.f;
		m_eType = SPECIAL_THROW::LONG;
		//m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		break;
	}
	//SetPhysX(false);
#pragma region UI
	if (CGameInstance::Get_Instance()->Current_Scene() != SCENE::MAPTOOL &&
		CGameInstance::Get_Instance()->Current_Scene() != SCENE::REALTEST)
	{
		m_pSpcObject_Rotation_Beam = dynamic_pointer_cast<CUI_Indicator_SpcObjectAttack_Rotation_Beam>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM));
		//CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_UI)->Add(m_pSpcObject_Rotation_Beam);
		m_pSpcObject_Rock = dynamic_pointer_cast<CUI_Indicator_SpcObjectAttack_Rock>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK));
	}
#pragma endregion
	return S_OK;
}

void CMonster_FallenObject::Tick(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	
	__super::Tick(fTimeDelta);

	if (CGameInstance::Get_Instance()->Current_Scene() != SCENE::MAPTOOL &&
		CGameInstance::Get_Instance()->Current_Scene() != SCENE::REALTEST)
	{
		m_pSpcObject_Rotation_Beam->Tick(fTimeDelta);
		m_pSpcObject_Rock->Tick(fTimeDelta);
	}

	m_fTime += fTimeDelta;

	if (m_Type == 3 && m_fTime < 4.f)
	{
		if (m_pTransform->Get_State(TRANSFORM::POSITION).y > 1.2f)
			m_pTransform->Translate({ 0,-fTimeDelta * 30.f,0 });
		else if (!m_bExecuted[18])
		{
			m_bExecuted[18] = true;
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_CarCrushed.wav"), g_fPlayerSound * 2.f);
		}
	}
	if (m_Type == 5 && m_fTime < 4.f)
	{
		if (m_pTransform->Get_State(TRANSFORM::POSITION).y > 1.8f)
			m_pTransform->Translate({ 0,-fTimeDelta * 30.f,0 });
		else if (!m_bExecuted[18])
		{
			m_bExecuted[18] = true;
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_CarCrushed.wav"), g_fPlayerSound * 2.f);
		}
	}
	if (m_Type == 7 && m_fTime < 4.f)
	{
		if (m_pTransform->Get_State(TRANSFORM::POSITION).y > 1.2f)
			m_pTransform->Translate({ 0,-fTimeDelta * 30.f,0 });
		else if (!m_bExecuted[18])
		{
			m_bExecuted[18] = true;
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_CarCrushed.wav"), g_fPlayerSound * 2.f);
		}
	}


	if (nullptr != m_pActor && m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
	{
		_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
		_float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());

		PxTransform p;
		p.p = PxVec3(vPos.x, vPos.y, vPos.z);
		p.q = PxQuat(q.x, q.y, q.z, q.w);

		m_pActor->setKinematicTarget(p);


	}
	else if(nullptr != m_pActor && !(m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC))
	{
		Set_Gravity(true);
	}
	//else if (nullptr != m_pActor && !(m_pActor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC))
	//{
	//	PxMat44 worldMat(m_pActor->getGlobalPose());
	//	m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
	//	Set_Gravity(true);
	//	_float4 q = { m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w };
	//	_float4 p = { m_pActor->getGlobalPose().p.x,m_pActor->getGlobalPose().p.y,m_pActor->getGlobalPose().p.z,1.f };
	//	_float4x4 rotmat = XMMatrixRotationQuaternion(q);

	//	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	//	_float3 OriginScale = m_pTransform->Get_Scale();
	//	m_pTransform->Set_Matrix(rotmat);
	//	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(p.x, p.y, p.z, 1.f));
	//	m_pTransform->Set_Scale(OriginScale);
	//}

	if (m_bFetched)
		m_fFetchedTime += fTimeDelta;

	if (m_bRandomize)
	{
		//_vector Position = XMVector3Normalize(m_pTransform->Get_State(TRANSFORM::POSITION));
		//_vector Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		//_vector Right = XMVector3Normalize(XMVector3Cross(Up, Position));
		//_matrix Pivot = XMMatrixRotationY(XMConvertToRadians(5.f));
		//Right = XMVector3TransformCoord(Right, Pivot);
		////Right = XMVector3Normalize(Right -= XMVector3Normalize(Position));
		//auto Velocity = m_pActor->getLinearVelocity();
		////m_pActor->setAngularVelocity(PxVec3(XMVectorGetX(Right * 5), XMVectorGetY(Right), XMVectorGetZ(Right)));
		////m_pActor->setAngularDamping(0.05f);
		//m_pActor->setMaxLinearVelocity(20.f);
		//m_pActor->setMaxAngularVelocity(20.f);
		//m_pActor->clearForce();
		//m_pActor->addForce(PxVec3(XMVectorGetX(Right) * m_fThrowForce * 0.25f, XMVectorGetY(Right) * m_fThrowForce * 0.25f, XMVectorGetZ(Right) * m_fThrowForce * 0.25f));
	}

	if(!m_bInteractive)
	{
		if(m_pActor)
		{
			//PxTransform Transform = m_pActor->getGlobalPose();

			//if (m_PxTransform.isValid())
			//	m_pActor->setGlobalPose(m_PxTransform);
			if (m_pActorDesc->bHit)
			{
				--m_tEntityDesc.fHP;
				m_pActorDesc->bHit = false;
			}
		}
	}

	if (m_fThrowTime < 0.f)
	{
		m_bIsRender = false;
	}

	if (nullptr != m_pActor)
	{
		ACTORDESC* pActorDesc = (ACTORDESC*)m_pActor->userData;
		if (m_bSlowStart)
		{
			auto pGameInstance = CGameInstance::Get_Instance();

			if (!m_bSlowEnable)
			{
				_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
				pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale * 0.1f);
				pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.3f);
				m_bSlowEnable = true;
			}

			m_fSlowTime -= fTimeDelta;
			if (m_fSlowTime < 0.f)
			{
				m_fSlowTime = 0.1f;
				m_bSlowStart = false;
				m_bSlowEnable = false;
				_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
				pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
				pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale);
				pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);
			}
		}
	}

	if (m_bThrowStart)
	{
		m_fThrowReadyTime += fTimeDelta;
		switch (m_Type)
		{
		case 3:
			Tick_SawTooth(fTimeDelta);
			break;
		case 5:
			Tick_Rock(fTimeDelta);
			break;
		case 7:
			Tick_Long(fTimeDelta);
			break;
		}
	}

	if (m_bThrow)
	{
		shared_ptr<CMonster_Statue> Monster = dynamic_pointer_cast<CMonster_Statue>(m_pTargetMonster);
		if (!Monster)
			return;
	}

	if (m_tEntityDesc.fHP <= 0.f && !m_bInteractive)
	{
		m_pActorDesc->stAttackDesc.iDamage = 11.f;
		m_pActorDesc->strTag = ACTORTAG::INTERACTIVE_SPECIAL;
		CGameInstance::Get_Instance()->Add_InteractiveSpecialObject(dynamic_pointer_cast<CMonster_FallenObject>(shared_from_this()));
		if(m_Type == 7)
		{
			m_pActorDesc->strTag = ACTORTAG::FALLEN_OBJECT;
			m_pActorDesc->bActive = false;
		}
		m_bInteractive = true;		
	}

	

}

void CMonster_FallenObject::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_bDissolve || m_bAura)
	{
		m_fAuraTimeFactor += fTimeDelta * 0.5f;
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	}
	else
	{
		Add_RenderObject(RENDER_GROUP::NONBLEND);
		Add_RenderObject(RENDER_GROUP::OUTLINE);
	}

	if (CGameInstance::Get_Instance()->Current_Scene() != SCENE::MAPTOOL &&
		CGameInstance::Get_Instance()->Current_Scene() != SCENE::REALTEST)
	{
		m_pSpcObject_Rotation_Beam->Late_Tick(fTimeDelta);
		m_pSpcObject_Rock->Late_Tick(fTimeDelta);
	}
}

HRESULT CMonster_FallenObject::Render()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (!m_bIsRender)
		return S_OK;


	if (FAILED(CGameInstance::Get_Instance()->Bind_LightDirection(CGameInstance::Get_Instance()->Current_Scene(), m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to Bind_LightDirection");
	}

	if (m_bAura)
	{
		m_iPassIndex = 18;

		if (FAILED(m_pAuraTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, "texAura")))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_ShaderResourceView:Aura");
		}

		if (FAILED(m_pShader->Bind_RawValue("vAuraColor", &m_vAuraColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Color");
		}
		if (FAILED(m_pShader->Bind_Float("fAuraBlendFactor", m_fAuraBlendFactor)))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Blend Factor");
		}
		if (FAILED(m_pShader->Bind_Float("fAuraTimeFactor", m_fAuraTimeFactor)))
		{
			MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render", "Failed to Bind_RawValue: Aura Time Factor");
		}
	}

	if (m_bDissolve)
	{
		m_iPassIndex = 16;

		if (FAILED(m_pDissolveTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDISSOLVE)))
		{
			return E_FAIL;
		}

		if (FAILED(m_pShader->Bind_Float("g_fAccTime", m_fDissolveTime)))
		{
			return E_FAIL;
		}
	}

	if (FAILED(CGameObject::Render(m_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Render", "Failed to CGameObject::Render");
	}

	if (FAILED(m_pModel->Bind_ShaderResourceViews(0, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CModel::Render", "Failed to Bind_ShaderResourceViews");
	}

	return S_OK;
}

HRESULT CMonster_FallenObject::Render_OutLine()
{
	return S_OK;
}

HRESULT CMonster_FallenObject::Fetch(any Any)
{
	if (!Any.has_value())
		return E_FAIL;
	FALLENDESC Desc = any_cast<FALLENDESC>(Any);
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if(!Desc.Randomize)
	{
		m_pTransform->Rotate(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(Desc.InitPos.w));
		Desc.InitPos.w = 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, Desc.InitPos);
	}
	else
	{
		m_pTransform->Rotate(XMQuaternionRotationRollPitchYaw(Function::RandomFloat(-XM_PI, XM_PI), Function::RandomFloat(-XM_PI, XM_PI), Function::RandomFloat(-XM_PI, XM_PI)));
		Desc.InitPos.w = 1.f;

		m_pTransform->Set_State(TRANSFORM::POSITION, Desc.InitPos);
	}

	_float4x4 mat = m_pTransform->Get_Matrix();
	_vector Rotation, Scale, Pos;
	XMMatrixDecompose(&Scale, &Rotation, &Pos, mat);

	PxTransform pose = PxTransform(PxIdentity);
	pose.p = PxVec3(mat._41, mat._42, mat._43);
	_float4 vQuat;
	XMStoreFloat4(&vQuat, Rotation);
	pose.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_vLocalPose = _float3(0.f, 2.5f, 0.f);
	_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
	m_pActor = pGameInstance->Get_PhysXEngine()->GetPhysics()->createRigidDynamic(pose);
	switch (m_Type)
	{
	case 0:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(/*2.5f, 2.5f, 2.5f*/0.5f,0.5f,0.5f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		m_bInteractive = true;
		m_vLocalPose = _float3(0.f, 0.4f, 0.f);
		CGameInstance::Get_Instance()->Add_InteractiveObject(dynamic_pointer_cast<CMonster_FallenObject>(shared_from_this()));
		break;
	case 1:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(1.6f , 0.6f, 0.6f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		m_bInteractive = true;
		m_vLocalPose = _float3(0.f, 0.5f, 0.f);
		CGameInstance::Get_Instance()->Add_InteractiveObject(dynamic_pointer_cast<CMonster_FallenObject>(shared_from_this()));
		break;
	case 2:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(0.6f, 0.8f, 0.6f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		m_bInteractive = true;
		m_vLocalPose = _float3(0.f, 0.6f, 0.f);
		CGameInstance::Get_Instance()->Add_InteractiveObject(dynamic_pointer_cast<CMonster_FallenObject>(shared_from_this()));
		break;
	case 3:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(3.f, 3.f, 0.3f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		m_vLocalPose = _float3(0.f, 0.5f, 0.f);
		break;
	case 5:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(1.f, 2.f, 1.2f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		m_vLocalPose = { 0,1.8f,0.f };
		m_pTransform->Set_Scale(XMVectorSet(2.f, 2.f, 2.f,0.f));
		break;
	case 7:
		m_pShape = pGameInstance->Get_PhysXEngine()->GetPhysics()->createShape(PxBoxGeometry(2.4f, 7.f, 2.4f), *pGameInstance->Get_PhysXEngine()->GetMaterial());
		m_vLocalPose = { 0,6.5f,0.f };
		m_pTransform->Set_Scale(XMVectorSet(2.f, 2.f, 2.f, 0.f));
		//m_pTransform->Rotate(TRANSFORM::UP, XMConvertToRadians(90.f));
		break;
	default:
		break;
	}	
	m_pShape->setLocalPose(PxTransform(PxVec3(m_vLocalPose.x, m_vLocalPose.y, m_vLocalPose.z)));

	m_pActor->attachShape(*m_pShape);
	SetPhysX(false);

	m_ActorDesc.strTag = ACTORTAG::FALLEN_OBJECT;
	m_ActorDesc.bThrow = false;
	m_ActorDesc.bThrowHit = false;
	m_ActorDesc.stAttackDesc.iDamage = 10;
	m_pActor->userData = &m_ActorDesc;

	m_fMass = 15000000000.f;
	m_fThrowForce = 150000.f;
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, m_fMass);
	pGameInstance->Get_PhysXEngine()->GetScene()->addActor(*m_pActor);


	m_ActorDesc.strTag = ACTORTAG::FALLEN_OBJECT;
	m_ActorDesc.bThrow = false;
	m_ActorDesc.bThrowHit = false;
	m_ActorDesc.stAttackDesc.iDamage = 10;
	m_pActor->userData = &m_ActorDesc;

	m_pActorDesc = (ACTORDESC*)m_pActor->userData;
	m_pActorDesc->stAttackDesc.iDamage = 11.f;
	m_tEntityDesc.fHP = 20.f;

	//m_pController->setFootPosition(PxExtendedVec3(XMVectorGetX(InitPos), XMVectorGetY(InitPos), XMVectorGetZ(InitPos)));
	m_bRandomize = Desc.Randomize;
	if (Desc.Randomize)
	{
		//m_pTransform->Set_Scale(XMVectorSet(5.f, 5.f, 5.f, 0.f));
		m_pActor->setGlobalPose(PxTransform(Desc.InitPos.x, Desc.InitPos.y, Desc.InitPos.z, PxQuat(XMConvertToRadians(0.f), PxVec3(0.f, 0.f, 1.f))));
		m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
		m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		//m_pActor->addTorque(PxVec3(m_fThrowForce * Function::RandomFloat(-0.25f, 0.25f), m_fThrowForce * Function::RandomFloat(-0.25f, 0.25f), m_fThrowForce * Function::RandomFloat(-0.25f, 0.25f)));
	}
	else
	{
		m_pActor->setGlobalPose(PxTransform(0.f, 15.f, 0.f, PxQuat(XMConvertToRadians(90.f), PxVec3(0.f, 0.f, 1.f))));
		m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

		//Set_Gravity(true);
		m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		SetPhysX(true);
		if (m_Type == 3)
		{
			m_pActor->setGlobalPose(PxTransform(0.f, 15.f, 0.f, PxQuat(XMConvertToRadians(90.f), PxVec3(0.f, 1.f, 0.f))));
			m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			m_pTransform->Rotate({ 1,0,0 }, XMConvertToRadians(90.f));
		}
		if (m_Type == 5)
		{
			m_pActor->setGlobalPose(PxTransform(0.f, 15.f, 0.f, PxQuat(XMConvertToRadians(0.f), PxVec3(0.f, 0.f, 1.f))));
			m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			m_pTransform->Rotate({ 1,0,0 }, XMConvertToRadians(90.f));
		}
		if (m_Type == 7)
		{
			m_pActor->setGlobalPose(PxTransform(0.f, 15.f, 0.f, PxQuat(XMConvertToRadians(0.f), PxVec3(0.f, 0.f, 1.f))));
			m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		}
		//m_pModel->Set_Animation(0);
	}

	m_bFetched = true; 

	PxSetGroup(*m_pActor, (_uint)ACTORTAG::FALLEN_OBJECT);

	return S_OK;
}

_bool CMonster_FallenObject::Return()
{
	return !m_bAlive;
}

HRESULT CMonster_FallenObject::Ready_Components()
{
	if (FAILED(CMap_Dynamic::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster_FallenObject::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster_FallenObject::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster_FallenObject::Ready_Components", "Failed to Get_Component: SHADER");
	}
	
	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster_FallenObject::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pDissolveTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002));
	if (nullptr == m_pDissolveTexture)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

shared_ptr<CMonster_FallenObject> CMonster_FallenObject::Create(ComPtr<ID3D11Device>_pDevice, ComPtr<ID3D11DeviceContext> _pContext,_uint Type)
{
	shared_ptr<CMonster_FallenObject> pInstance = make_private_shared(CMonster_FallenObject, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(Type)))
	{
		MSG_RETURN(nullptr, "CMonster_FallenObject::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMonster_FallenObject::Clone(any pArg)
{
	shared_ptr<CMonster_FallenObject> pInstance = make_private_shared_copy(CMonster_FallenObject, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMonster_FallenObject::Clone", "Failed to Initialize");
	}

	return pInstance;
}

void CMonster_FallenObject::Set_Target_Statue()
{
	shared_ptr<CObjectLayer> pMonLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER);
	if (pMonLayer)
	{
		_float fMinDist = 5000.f;
		m_pTargetMonster = nullptr;
		_float3 vCloseMonsterPos;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);
				cout << "pMonster" << endl;
				if (nullptr == pMonster || pMonster->Get_Type() != CMonster::MONSTERTYPE::STATUE)
					return true;

				_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				//if (dynamic_pointer_cast<CMonster_DispenPerry>(_pObject))
				//{
				//shared_ptr<CModel> pModel = pMonster->Get_Component<CModel>(COMPONENT::MODEL);
				//_matrix mat = g_mUnit;
				//mat.r[3] = _float4(vMonsterPos, 1.f);
				//mat = pModel->Get_Bone(pMonster->GetWeakBoneName().c_str())->Get_CombinedTransformation() * pModel->Get_Pivot() * mat;
				//vMonsterPos = mat.r[3];
				PxVec3 vPos = pMonster->GetBodyCollider()->GetActor()->getGlobalPose().p;
				vMonsterPos = _float3(vPos.x, vPos.y, vPos.z);
				//}
				if (!pMonster->Get_LockOnAble())	
				{
					cout << "LOCKONABLEFALSE" << endl;
					return true;
				}
				_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				shared_ptr<CTransform> pPlayerCamTransform = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Get_Component(COMPONENT::TRANSFORM));
				_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vMonsterPos));
				if (fCurDist < fMinDist)
				{
					fMinDist = fCurDist;
					m_pTargetMonster = pMonster;
					m_pTargetMonsterTransform = static_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
					vCloseMonsterPos = vMonsterPos;
					m_vHoodPos = vMonsterPos;
			/*		if (dynamic_pointer_cast<CMonster_Statue>(pMonster) && dynamic_pointer_cast<CMonster_Statue>(pMonster)->Get_MaskType() != CMonster_Statue::TYPE::MASK_WHITE)
						m_vHoodPos += -m_pTransform->Get_State(TRANSFORM::LOOK)*5.f;*/
					cout << "HOODPOS" << endl;
				}
				return true;
			}
		);
	}
}

void CMonster_FallenObject::Tick_SawTooth(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (m_fThrowReadyTime > 1.f && m_fThrowReadyTime < 2.f)
	{
		if(!m_bExecuted[0])
		{
			m_bExecuted[0] = true;
			//SetPhysX(false);
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_Down.wav"), g_fPlayerSound);
		}
		m_pTransform->Translate({ 0,fTimeDelta*1.f,0,0 });
	}
	else if (m_fThrowReadyTime >= 2.f && !m_ActorDesc.bThrowHit)
	{
		if (!m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			Set_ThrowLook();
			Throw();
			//m_ActorDesc.bActive = true;
			SetPhysX(true);
			m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		}
		m_pTransform->Translate(m_vThrowLook * fTimeDelta * 50.f);
	}
	if(m_ActorDesc.bThrowHit)
	{
		if (!m_bExecuted[2])
		{
			m_bExecuted[2] = true;
			m_fThrowReadyTime = 0.f;
			m_bIsRender = false;
			pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
			m_pActor->release();
			m_pActor = nullptr;
			shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC);
			for (int i = 0; i < 5; i++)
			{
				std::get<2>(m_copyTuplePivot) = m_pTransform->Get_Scale();
				std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
				std::get<5>(m_copyTuplePivot) = m_pTransform->Get_Matrix();
				shared_ptr<CMap_Special_Dynamic> pBeam = static_pointer_cast<CMap_Special_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA_PIECE, m_copyTuplePivot));
				pLayer->Add(pBeam);
				m_DuplicatedBeam.push_back(pBeam);
				
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				_float3 val[5] = { {-0.8f,1.1f,-1.2f},{1.1f,1.f,-1.f},{-0.1f,1.f,0.1f}, {1.1f,1.f,1.f},{-0.9f,0.8f,0.9f} };
				_float3 vDir = val[i];
				vDir.y *= 1.5f;

				m_vHitDir[i] = vDir;
				pTransform->Rotate_Lerp(TRANSFORM::RIGHT, vDir.x, 1.f);
				pTransform->Rotate_Lerp(TRANSFORM::UP, vDir.y, 1.f);
				pTransform->Rotate_Lerp(TRANSFORM::LOOK, vDir.z, 1.f);
			}
			static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_CapturedObject(m_DuplicatedBeam[2]);
			static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_RecentThrowObject(m_DuplicatedBeam[2]);
			_float4x4 mat = m_pTransform->Get_Matrix();
			CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_BEAM,
				initializer_list<any>{mat, mat, mat,
				mat, mat, mat,
				mat, mat, mat});
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_PitchDown.wav"), g_fPlayerSound);
		}

		if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 1.f)
		{
			for (int i = 0; i < 5; i++)
			{
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Translate(XMVectorLerp(m_vHitDir[i] * fTimeDelta * 8.f, _float3(0, 0, 0), m_fThrowReadyTime));
				pTransform->Translate({ 0,-m_fThrowReadyTime * m_fThrowReadyTime * 0.2f,0 });
				shared_ptr<CTransform> pPlayerTransform = static_pointer_cast<CTransform>(pGameInstance->Get_Player()->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Translate(pPlayerTransform->Get_State(TRANSFORM::LOOK) * -fTimeDelta * 5.f);
			}
		}
		if (m_fThrowReadyTime >= 1.f && m_fThrowReadyTime < 1.f + m_fBeamRotTime)
		{
			if (!m_bExecuted[33])
			{
				m_bExecuted[33] = true;
				m_pSpcObject_Rotation_Beam->Set_RenderStart();
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("SFX_SpecialAttack_H_Beam_Full.wav"), CSound_Manager::SOUND_CHARGE, g_fPlayerSound);
				for (_int i = 0; i < m_DuplicatedBeam.size(); i++)
				{
					_float4x4 mat = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM))->Get_Matrix();

					CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_BEAM,
						initializer_list<any>{mat, mat, mat,
						mat, mat, mat,
						mat, mat, mat});
				}
			}

			if (0.5f < m_fBeamRotTime)
			{
				m_pSpcObject_Rotation_Beam->Set_RotateMouse();
			}

			for (int i = 0; i < 5; i++)
			{
				static_pointer_cast<CMap_Special_Dynamic>(m_DuplicatedBeam[i])->Throw();
			}

			//m_pTransform->Rotate(TRANSFORM::UP, fTimeDelta * 10.f);
			m_fBeamRotTime += fabs((pGameInstance->Get_CursorMove().x + pGameInstance->Get_CursorMove().y) * fTimeDelta * 0.003f);
			m_fBeamRotTime = min(6.f, m_fBeamRotTime);
			m_fBeamHitTime -= fTimeDelta;
			_bool bHit = false;
			for (int i = 0; i < 5; i++)
			{
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Rotate(TRANSFORM::LOOK, fTimeDelta * 10.f);
				//pTransform->Translate(-m_vHitDir[i] * fTimeDelta * 18.f);
				if (m_fBeamHitTime < 0.f)
				{
					m_DuplicatedBeam[i]->Get_ActorDesc().bActive = true;
					m_DuplicatedBeam[i]->Get_ActorDesc().bThrow = true;
					if (m_DuplicatedBeam[i]->Get_ActorDesc().bThrowHit && !m_bBeamThrowHit && m_fThrowReadyTime > 1.f + m_fBeamRotTime * 0.5f)
					{
						m_fSlowTime = 0.3f;
						m_bBeamThrowHit = true;
						m_bSlowStart = true;
						//ThrowHit();
					}
				}
			}
			if (m_fBeamHitTime < 0.f)
			{
				m_fBeamHitTime = 0.4f;
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_1Hit.wav"), g_fPlayerSound);
			}
		}
		else if (m_fThrowReadyTime >= 1.f + m_fBeamRotTime)
		{
			if (!m_bExecuted[34])
			{
				m_pSpcObject_Rotation_Beam->Set_Finish();
				m_bExecuted[34] = true;
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_Finish.wav"), g_fPlayerSound);
			}
			static_pointer_cast<CPlayerSpecialThrowState>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerState())->Set_Throw(true);
				for (int i = 0; i < 5; i++)
				{
					m_DuplicatedBeam[i]->Get_Actor()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
					m_DuplicatedBeam[i]->Set_Throw(true);
					m_DuplicatedBeam[i]->Set_ThrowTime(1.f);
				}
			//Throw();
			//Interactive_PhychoKinesis(false);
			m_bThrowStart = false;
		}
	}

}

void CMonster_FallenObject::Tick_Rock(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	if (m_ActorDesc.bThrowHit)
	{
		if (pGameInstance->Key_Down('A') || pGameInstance->Key_Down('D'))
		{
			m_iHitCount++;
		}
		if (m_iHitCount > 10)
		{
			if (!m_bExecuted[10])
			{
				m_bExecuted[10] = true; 
				m_fSlowTime = 0.f;
				m_bHitSuccess = true;
				m_fThrowReadyTime = 0.f;
				for (int i = 0; i < m_DuplicatedBeam.size(); i++)
				{
					m_DuplicatedBeam[i]->Get_ActorDesc().bThrow = true;
				}
				m_ActorDesc.bThrow = true;
			}
		}
	}

	if (!m_bExecuted[25])
	{
		m_bExecuted[25] = true;
		Set_Target_Statue();
		if (nullptr != m_pTargetMonster)
			m_pTargetMonster->SetBulldozerHit(true);
		m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	}

	if (nullptr == m_pTargetMonsterTransform)
	{
		return;
	}
 	if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 3.f)
	{
		_float3 vPos = m_vHoodPos;
		vPos.y = m_pTransform->Get_State(TRANSFORM::POSITION).y;
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION), vPos, min(1.f, m_fThrowReadyTime / 3.f)));
		m_pTransform->Translate({ 0,fTimeDelta * 50.f,0,0 });
	}

	if (m_fThrowReadyTime >= 3.f && m_fThrowReadyTime < 5.f)
	{
		if (!m_bExecuted[11])
		{
			m_bExecuted[11] = true;
		}
	}
	else if (m_fThrowReadyTime >= 5.3f)
	{
		if (!m_ActorDesc.bThrowHit)
		{
			if (!m_bExecuted[31])
			{  
				Throw();
				m_bExecuted[31] = true;
				//m_bSlowStart = true;
			}
			_float3 vPos = m_vHoodPos;
			
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION), vPos, min(1.f, m_fThrowReadyTime - 5.2f)));
			//m_pTransform->Translate({0,-fTimeDelta * 150.f,0,0});
		}
		else
		{
			if (!m_bExecuted[17])
			{
				m_bExecuted[17] = true;
				Interactive_PhychoKinesis(false);

				m_pSpcObject_Rock->Set_RenderStart();

				_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK0,
					initializer_list<any>{Pos, Pos, Pos,Pos});

				for (int i = 0; i < 7; i++)
				{
					_float3 scale[7] = { {0.9f,0.5,0.9f},{1.2f,0.5f,0.9f},{1.5f,0.5f,1.5f}, {0.9f,0.5f,0.9f},{1.1f,0.5f,1.2f},{0.9f,0.5f,0.9f},{0.7f,0.5f,1.1f} };
					shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC);
					std::get<2>(m_copyTuplePivot) = scale[i] * 1.f;
					std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
					std::get<5>(m_copyTuplePivot) = m_pTransform->Get_Matrix();
					shared_ptr<CMap_Special_Dynamic> pBeam = static_pointer_cast<CMap_Special_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0_COPY, m_copyTuplePivot));
					pLayer->Add(pBeam);
					m_DuplicatedBeam.push_back(pBeam);
					_float fRotX = Function::RandomFloat(0.f, 3.14f); _float fRotY = Function::RandomFloat(0.f, 3.14f); _float fRotZ = Function::RandomFloat(0.f, 3.14f);
					static_pointer_cast<CTransform>(pBeam->Get_Component(COMPONENT::TRANSFORM))->Rotate(TRANSFORM::RIGHT, fRotX);
					static_pointer_cast<CTransform>(pBeam->Get_Component(COMPONENT::TRANSFORM))->Rotate(TRANSFORM::UP, fRotY);
					static_pointer_cast<CTransform>(pBeam->Get_Component(COMPONENT::TRANSFORM))->Rotate(TRANSFORM::LOOK, fRotZ);
					_float3 val[7] = { {-0.8f,1.1f,-1.2f},{1.1f,1.f,-1.f},{-0.1f,1.f,0.1f}, {1.1f,1.f,1.f},{-0.9f,0.8f,0.9f},{0.1f,0.9f,1.f},{1.f,1.f,-0.1f} };
					m_vRockHitDir[i] = val[i];
				}
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_RockCurshed.wav"), g_fPlayerSound);
				if (nullptr != m_pActor)
				{
					//pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
					//m_pActor->release();
					//m_pActor = nullptr;
				}
				m_bIsRender = false;
			}

			for (int i = 0; i < 7; i++)
			{
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Translate({ m_vRockHitDir[i].x * fTimeDelta * 15.f,m_vRockHitDir[i].y * fTimeDelta * 15.f,m_vRockHitDir[i].z * fTimeDelta * 15.f });
				pTransform->Rotate(TRANSFORM::RIGHT, fTimeDelta * 5.f);
				pTransform->Rotate(TRANSFORM::UP, fTimeDelta * 5.f);
				pTransform->Rotate(TRANSFORM::LOOK, fTimeDelta * 5.f);
			}

			if (m_fThrowReadyTime >= 5.2f)
			{
				if (!m_bExecuted[21]) 
				{
					m_fSlowTime = 15.f;
					m_bExecuted[21] = true;
					m_bSlowStart = true;
				}
			}
		}
	}

	if (m_bHitSuccess)
	{
		if (m_fThrowReadyTime < 4.f)
		{
			if (m_DuplicatedBeam.empty())
				return;

			if (!m_bExecuted[18])
			{
				for (int i = 0; i < 7; i++)
				{

					m_bExecuted[18] = true;
					m_DuplicatedBeam[i]->Get_ActorDesc().bActive = true;
					m_DuplicatedBeam[i]->Get_ActorDesc().bThrow = true;
				}
			}
			m_fDebrisFallTime += fTimeDelta;
			for (int i = 0; i < m_DuplicatedBeam.size(); i++)
			{
				shared_ptr<CTransform> copyTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				//copyTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(copyTransform->Get_State(TRANSFORM::POSITION), m_vHoodPos, min(1.f, m_fThrowReadyTime)));

				_float3 vDir = m_vHoodPos - static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				vDir = XMVector3Normalize(vDir);
				static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM))->Translate({ vDir.x * fTimeDelta * 100.f,vDir.y * fTimeDelta * 100.f,vDir.z * fTimeDelta * 100.f });
				if (m_DuplicatedBeam[i]->Get_ActorDesc().bThrowHit)
				{
					if (!m_bExecuted[26])
					{
						m_bExecuted[26] = true;
						for(int i=0;i< m_DuplicatedBeam.size();i++)
						{
							if (nullptr != m_DuplicatedBeam[i]->Get_Actor())
							{
								//pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_DuplicatedBeam[i]->Get_Actor());
								//m_DuplicatedBeam[i]->Get_Actor()->release();
								//m_DuplicatedBeam[i]->Reset_Actor();
								m_DuplicatedBeam[i]->Set_Render(false);
							}
						}

							CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_RockCurshed.wav"), g_fPlayerSound);
						//for (int i = 0; i < 100; i++)
						//{
						//	shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC);
						//	std::get<2>(m_copyTuplePivot) = _float3(0.05f, 0.1f, 0.1f)/*_float3(1.f,1.f,1.f)*/;
						//	std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
						//	std::get<5>(m_copyTuplePivot) = m_pTransform->Get_Matrix();
						//	shared_ptr<CMap_Special_Dynamic> pBeam = static_pointer_cast<CMap_Special_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0_COPY, m_copyTuplePivot));
						//	pLayer->Add(pBeam);
						//	m_Debris.push_back(pBeam);
						//	m_vDebrisHitDir[i] = { Function::RandomFloat(-3.f,3.f),Function::RandomFloat(-3.f,3.f),Function::RandomFloat(-3.f,3.f) };
						//}
						/*m_fSlowTime = 15.f;
						m_bSlowStart = true;*/
						m_DuplicatedBeam[i]->Get_ActorDesc().bThrowHit = false;
					}
				}
			}
			//for (int i = 0; i < m_Debris.size(); i++)
			//{
			//	static_pointer_cast<CTransform>(m_Debris[i]->Get_Component(COMPONENT::TRANSFORM))->Translate({ m_vDebrisHitDir[i].x * fTimeDelta * 50.f, m_vDebrisHitDir[i].y * fTimeDelta * 50.f /*- m_fDebrisFallTime * m_fDebrisFallTime * 5.f*/, m_vDebrisHitDir[i].z * fTimeDelta * 50.f });
			//}
		}
		else if (m_fThrowReadyTime >= 4.f)
		{
			if (!m_bExecuted[20]) 
			{
				m_bExecuted[20] = true;
				for (int i = 0; i < 7; i++)
				{
					m_DuplicatedBeam[i]->Set_Alive(false);
				}
		/*		for (int i = 0; i < m_Debris.size(); i++)
				{
					m_Debris[i]->Set_Alive(false);
				}*/
				m_bAlive = false;
			}
			m_bThrowStart = false;
		}
	}
}

void CMonster_FallenObject::Tick_Long(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (m_bSlowStart)
	{
		if (pGameInstance->Key_Down('D'))
		{
			m_iHitCount++;
		}
		if (m_iHitCount > 8)
		{
			if (!m_bExecuted[33])
			{
				m_bExecuted[33] = true;
				m_fSlowTime = 0.f;
				m_bHitSuccess = true;
				m_fThrowReadyTime = 0.f;

				if (!m_bExecuted[66])
				{
					m_bExecuted[66] = true;
					m_pSpcObject_Rock->Set_KeyInput();
				}
			}
		}
	}

	CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player());
	shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(pPlayer->Get_Component(COMPONENT::TRANSFORM));
	if (!m_bExecuted[25])
	{
		m_bExecuted[25] = true;
		Set_Target_Statue();
		if (nullptr != m_pTargetMonster)
			m_pTargetMonster->SetBulldozerHit(true);
		SetPhysX(true);
		m_pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		Throw();
	}

	if (nullptr == m_pTargetMonsterTransform)
	{
		return;
	}

	if(!m_bHitSuccess)
	{
		if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 2.f)
		{
			if (!m_bExecuted[10])
			{
				m_bExecuted[10] = true;
				m_vLongPos1[0] = m_pTransform->Get_State(TRANSFORM::POSITION);
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_Down.wav"), g_fPlayerSound*2.f);
			}
			_float3 vPos = m_vHoodPos;
			vPos += m_pTargetMonsterTransform->Get_State(TRANSFORM::LOOK) * 3.f - m_pTargetMonsterTransform->Get_State(TRANSFORM::RIGHT) * 13.f - m_pTargetMonsterTransform->Get_State(TRANSFORM::UP) * 3.f;
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_vLongPos1[0], vPos, min(1.f, m_fThrowReadyTime / 2.f)));
			//m_pTransform->Translate({ 0,fTimeDelta * 50.f,0,0 });
			m_pTransform->LookAtDir(pTransform->Get_State(TRANSFORM::LOOK));
			m_pTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(90.f));
		}

		if (m_fThrowReadyTime >= 2.f && m_fThrowReadyTime < 2.5f)
		{
			if (!m_bExecuted[11])
			{
				m_bExecuted[11] = true;
				m_vLongPos1[1] = m_pTransform->Get_State(TRANSFORM::POSITION);
				m_pTransform->LookAtDir(pTransform->Get_State(TRANSFORM::LOOK));
				m_pTransform->Rotate(TRANSFORM::RIGHT, XMConvertToRadians(90.f));
				m_ActorDesc.bActive = true;
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Set_Shake_NonDelta(0.3f, 10.f);
			}
			_float3 vPos = m_vHoodPos;
			vPos += m_pTargetMonsterTransform->Get_State(TRANSFORM::LOOK) * 3.f + m_pTargetMonsterTransform->Get_State(TRANSFORM::RIGHT) * 18.f - m_pTargetMonsterTransform->Get_State(TRANSFORM::UP) * 3.f;
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_vLongPos1[1], vPos, min(1.f, (m_fThrowReadyTime - 1.9f) / 0.5f)));
			if (m_fThrowReadyTime > 2.25f && !m_bExecuted[41])
			{
				m_bExecuted[41] = true;
				_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK0,
					initializer_list<any>{Pos, Pos, Pos, Pos});
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_DumpTruck_Smash.wav"), g_fPlayerSound*2.f);
			}
		}
		if (m_fThrowReadyTime >= 2.f && m_fThrowReadyTime < 3.f)
		{
			if (m_fThrowReadyTime > 2.5f && !m_bExecuted[20])
			{
				m_bExecuted[20] = true;
				m_ActorDesc.bActive = false;
				m_bSlowStart = true;
				m_fSlowTime = 15.f;
			}
			m_pTransform->Rotate({ 0,1,0 }, -fTimeDelta * Function::Lerp(5.f, 0.f, min(1.f, (m_fThrowReadyTime - 1.9f) / 1.f)));
		}

		if (m_fThrowReadyTime > 2.5f)
		{
			m_pSpcObject_Rock->Set_RenderStart(); 
		}
	}
	else
	{
		if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 0.5f)
		{
			if (!m_bExecuted[14])
			{
				m_bExecuted[14] = true;
				m_vLongPos1[4] = m_pTransform->Get_State(TRANSFORM::POSITION);
				//Throw();
				m_ActorDesc.bActive = true;
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Set_Shake_NonDelta(0.3f, 10.f);
			}
			if (m_fThrowReadyTime > 0.25f && !m_bExecuted[42])
			{
				m_bExecuted[42] = true;
				_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK0,
					initializer_list<any>{Pos, Pos, Pos, Pos});
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_DumpTruck_Smash.wav"), g_fPlayerSound);
			}
			_float3 vPos = m_vHoodPos;
			vPos += m_pTargetMonsterTransform->Get_State(TRANSFORM::LOOK) * 3.f + m_pTargetMonsterTransform->Get_State(TRANSFORM::RIGHT) * 18.f - m_pTargetMonsterTransform->Get_State(TRANSFORM::UP) * 3.f;
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_vLongPos1[4], m_vLongPos1[1], min(1.f, (m_fThrowReadyTime) / 0.5f)));
		}
		if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 1.5f)
		{
			if (m_fThrowReadyTime > 0.5f && !m_bExecuted[21])
			{
				m_bExecuted[21] = true;
				m_ActorDesc.bActive = false;
				m_bSlowStart = true;
			}
			/*	_float3 vPos = m_vHoodPos;
				vPos += m_pTargetMonsterTransform->Get_State(TRANSFORM::LOOK) * 7.f - m_pTargetMonsterTransform->Get_State(TRANSFORM::RIGHT) * 15.f - m_pTargetMonsterTransform->Get_State(TRANSFORM::UP) * 3.f;
				m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_vLongPos1[5], vPos, min(1.f, (m_fThrowReadyTime - 5.9f) / 1.f)));*/
			m_pTransform->Rotate({ 0,1,0 }, fTimeDelta * Function::Lerp(6.f, 0.f, min(1.f, (m_fThrowReadyTime) / 1.5f)));
		}
		else if (m_fThrowReadyTime >= 1.5f)
		{
			static_pointer_cast<CPlayerSpecialThrowState>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerState())->Set_Throw(true);
			m_bDissolve = true;
			m_bThrowStart = false;
		}
	}
}

void CMonster_FallenObject::Throw()
{
	m_ActorDesc.bThrow = true;
	//m_ActorDesc.bCapture = false;

	shared_ptr<CInteractiveObject> ThisPointer = dynamic_pointer_cast<CInteractiveObject>(shared_from_gameobject());
	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_RecentThrowObject(ThisPointer);
	if (nullptr != m_pActor)
	{
		m_pActor->userData = &m_ActorDesc;
	}
		static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_SpecialThrow(false);
		//static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_CapturedObject(nullptr);
		/*switch (m_eType)
		{
		case SPECIAL_THROW::HEAVY_EQUIPMENT:
			m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
			m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
			m_pActor->addForce(PxVec3(0, -1.f * 150.f, 0));
			break;
		case SPECIAL_THROW::ROCK:
			break;
		case SPECIAL_THROW::BEAM:
			m_bThrow = true;
			SetPhysX(true);
			Set_Gravity(true);
			break;
		case SPECIAL_THROW::BEAM_PIECE:
			m_bThrow = true;
			SetPhysX(true);
			Set_Gravity(true);
			break;
		}*/

		//	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Reset();
}

void CMonster_FallenObject::ThrowHit()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
	switch (m_eType)
	{
	default:
		m_bThrowHit = true;

		pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.3f);
		break;
	}
}
