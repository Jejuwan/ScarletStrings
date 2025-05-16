#include "ClientPCH.h"
#include "Map_Dynamic.h"
#include "GameInstance.h"
#include "Light.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"

#include "Monster.h"
#include "Psychokinesis.h"
#include <regex>

#include "Bone.h"
#include "Monster_DispenPerry.h"
#include "Monster.h"

#include "Effect_Manager.h"
#include "Camera_Player.h"

#include "Light_Manager.h"


CMap_Dynamic::CMap_Dynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CInteractiveObject(_pDevice, _pContext)
{
}

CMap_Dynamic::CMap_Dynamic(const CMap_Dynamic& _rhs)
	: CInteractiveObject(_rhs),
	m_vExtents(_rhs.m_vExtents),
	m_vLocalPose(_rhs.m_vLocalPose),
	m_fMass(_rhs.m_fMass),
	m_fThrowHeight(_rhs.m_fThrowHeight)
{
}

HRESULT CMap_Dynamic::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	
	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);

	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	m_tMaterialDesc.vEmissive = _float4(0.f, 0.f, 0.f, 1.f);
	m_tMaterialDesc.fShininess = 64.f;

	Add_ModelCom();

	return S_OK;
}

HRESULT CMap_Dynamic::Initialize(any pArg)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CMap_Dynamic::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	if (m_bSceneMapTool)
	{
		Using_Map_Tool();
	}

	m_fTimer += fTimeDelta * 0.1f;

	if (m_bActivePhysX)
	{
		if (nullptr != m_pActor)
		{
			//PxMat44 worldMat(m_pActor->getGlobalPose());
			m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

			_float4 q = { m_pActor->getGlobalPose().q.x,m_pActor->getGlobalPose().q.y,m_pActor->getGlobalPose().q.z,m_pActor->getGlobalPose().q.w };
			_float4 p = { m_pActor->getGlobalPose().p.x,m_pActor->getGlobalPose().p.y,m_pActor->getGlobalPose().p.z,1.f };
			_float4x4 rotmat = XMMatrixRotationQuaternion(q);

			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 OriginScale = m_pTransform->Get_Scale();
			m_pTransform->Set_Matrix(rotmat);
			m_pTransform->Set_State(TRANSFORM::POSITION, _float4(p.x, p.y, p.z, 1.f));
		    m_pTransform->Set_Scale(OriginScale);
		}
	}
	else
	{
		if (nullptr != m_pActor)
		{
			//PxMat44 worldMat(m_pActor->getGlobalPose());


			_float4 q = XMQuaternionRotationMatrix(m_pTransform->Get_WorldMatrix());
			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);

			PxTransform px;
			px.p = PxVec3(vPos.x, vPos.y, vPos.z);
			px.q = PxQuat(q.x, q.y, q.z, q.w);

			m_pActor->setGlobalPose(px);
		}
	}

	auto pGameInstance = CGameInstance::Get_Instance();

	if (nullptr != m_pActor)
	{
		ACTORDESC* pActorDesc = (ACTORDESC*)m_pActor->userData;
		if (pActorDesc != nullptr && pActorDesc->bThrowHit && m_bThrow)
		{
			if (!m_bExecuted[0])
			{
				m_bExecuted[0] = true;
				ThrowHit();
			}
		}

		//if (pActorDesc->bCapture)
		//{
		//	m_bCaptured = true;
		//}
		//else
		//{
		//	if (m_bCaptured)
		//		int a = 0;
		//}
	}

	if (m_bThrow)
	{
		m_fThrowTime -= fTimeDelta;
		if (m_fThrowTime < 1.5f)
		{
			SetPhysX(true);
			Set_Gravity(true);
		}
		if (m_fThrowTime < 0.f)
		{
			m_bDissolve = true;
			m_bInteractivable = false;

			m_iPassIndex = 16;
			m_iGlassPassIndex = 16;
		}
	}

	if (m_bThrowHit)
	{
		if (!m_bExecuted[55])
		{
			m_bExecuted[55] = true;
			m_vThrowLook = _float3(Function::RandomFloat(-1.f, 1.f), Function::RandomFloat(-1.f, 0.f), Function::RandomFloat(-1.f, 1.f));
		}
		m_ThrowHitTime -= fTimeDelta;

		if (m_ThrowHitTime > 0.f)
		{
			m_pTransform->Translate(_float3(-m_vThrowLook.x * fTimeDelta * 30.f, -m_vThrowLook.y * fTimeDelta * 30.f, -m_vThrowLook.z * fTimeDelta * 30.f));
			m_pTransform->Rotate(m_vThrowLook, fTimeDelta * 2.f);
		}

		if (m_ThrowHitTime < 0.f && !m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			SetPhysX(true);
			Set_Gravity(true);
			if (nullptr != m_pActor)
			{
				m_pActor->addForce(PxVec3(-m_vThrowLook.x * m_fThrowForce * 0.25f, -m_vThrowLook.y * m_fThrowForce * 0.25f, -m_vThrowLook.z * m_fThrowForce * 0.25f));
				m_pActor->addTorque(PxVec3(5, 5, 5));
			}
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale);

			if (m_pDuplicatedObject)
			{
				m_pDuplicatedObject->SetPhysX(true);
				m_pDuplicatedObject->Set_Gravity(true);
				m_pDuplicatedObject->Throw();
			}
		}
		if (m_ThrowHitTime < -0.3f)
		{
			m_bDissolve = true;
			m_bInteractivable = false;
			Interactive_PhychoKinesis(false);
			m_iPassIndex = 16;
			m_iGlassPassIndex = 16;
		}
	}

	if (m_bDissolve)
	{
		m_fDissolveTime += fTimeDelta * 0.5f;

		if (m_fDissolveTime >= 1.f)
		{
			m_bAlive = false;
			
			CGameInstance::Get_Instance()->Erase_Light(m_pLight);
		}
	}

	if (nullptr != m_MonsterMatrix)
	{
		if (!((ACTORDESC*)(m_pActor->userData))->bThrowHit)
		{
			//m_pActor->clearForce();
			//m_pActor->clearTorque();
			_matrix mat = g_mUnit;
			mat.r[3] = m_MonsterMatrix->row(3);

			m_vThrowLook = (XMLoadFloat4x4(m_BoneMatrix) * m_Pivot * mat).r[3] - m_pTransform->Get_State(TRANSFORM::POSITION);
			m_pActor->addForce(PxVec3(m_vThrowLook.x * m_fThrowForce * 0.005f, m_vThrowLook.y * m_fThrowForce * 0.005f, m_vThrowLook.z * m_fThrowForce * 0.005f));
		}
		else
		{
			m_MonsterMatrix = nullptr;
			((ACTORDESC*)(m_pActor->userData))->bActive = false;
		}
		//m_pActor->addForce(PxVec3(m_vThrowLook.x, m_vThrowLook.y, m_vThrowLook.z));
	}

	Duplicate_Model(fTimeDelta);

}

void CMap_Dynamic::Late_Tick(_float fTimeDelta)
{
	if (m_bDead)
		return;

	__super::Late_Tick(fTimeDelta); 

	Add_RenderObject(RENDER_GROUP::NONBLEND);
	Add_RenderObject(RENDER_GROUP::SHADOW);

	if (m_bFocused)
	{
		Add_RenderObject(RENDER_GROUP::OUTLINE);
		m_bFocused = false;
	}
}

HRESULT CMap_Dynamic::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CMap_Tutorial_Dynamic::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CMap_Dynamic::Render_OutLine()
{
	if (FAILED(m_pShader->Bind_Float(SHADER_OUTLINE_SIZE, m_fOutLineSize)))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_Float");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_OUTLINE_COLOR, &m_vOutlineColor, sizeof(_float3))))
	{
		MSG_RETURN(E_FAIL, "CArashi::Render", "Failed to Bind_RawValue");
	}

	if (FAILED(__super::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CKyoka::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

void CMap_Dynamic::Interactive_PhychoKinesis(_bool bInteractive)
{
	__super::Interactive_PhychoKinesis(bInteractive);

	m_bAura = bInteractive;
	if (m_bAura && false == m_bDissolve)
	{
		m_iPassIndex = 18;
	}
	else if (false == m_bAura)
	{
		m_iPassIndex = 15;
	}

	//cout << m_iPassIndex << endl;
}

HRESULT CMap_Dynamic::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMap_Dynamic::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pAuraTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_AURA_005));
	if (nullptr == m_pAuraTexture)
	{
		MSG_RETURN(E_FAIL, "CMap_Dynamic::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	return S_OK;
}

void CMap_Dynamic::Duplicate_Model(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	if (m_bReadyDuplicate)
	{
		m_fDuplicateTime -= fTimeDelta;
		if (m_fDuplicateTime < 0.f)
		{
			m_bReadyDuplicate = false;
			shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC);
			wstring strPrototype;
			if (m_strName == TEXT("CarA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARA;
			else if (m_strName == TEXT("CarB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARB;
			else if (m_strName == TEXT("CarC"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARC;
			else if (m_strName == TEXT("CarD"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARD;
			else if (m_strName == TEXT("CarD_Brk"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CARD_Brk;
			else if (m_strName == TEXT("ConcreteBarrierA"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERA;
			else if (m_strName == TEXT("ConcreteBarrierB"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_CONCRETEBARRIERB;
			else if (m_strName == TEXT("SeiranCar"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR;
			else if (m_strName == TEXT("SeiranCar_Brk"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_SEIRANCAR_BRK;
			else if (m_strName == TEXT("Tractor"))
				strPrototype = PROTOTYPE_GAMEOBJECT_MAP_TRACTOR;
			m_pDuplicatedObject = static_pointer_cast<CMap_Dynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), strPrototype, m_copyTuplePivot));
			pLayer->Add(m_pDuplicatedObject);
			if (!m_bStrong)
				m_pDuplicatedObject->Throw();
			else
			{
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_DuplicatedObject(m_pDuplicatedObject);
				m_pDuplicatedObject->SetPhysX(false);
			}
			Interactive_PhychoKinesis(true);
			m_pDuplicatedObject->Interactive_PhychoKinesis(true);
		}
	}
}

void CMap_Dynamic::Using_Map_Tool()
{
	m_strSelectedName = CMap_ToolManager::Get_Instance()->Get_SelectName();
	if (m_strName == m_strSelectedName)
	{
		/* Scale, Radian, Translation, matrix */
		m_tuplePivot = CMap_ToolManager::Get_Instance()->Get_TuplePivot();

		m_strObjName = std::get<0>(m_tuplePivot);
		m_Scale = std::get<1>(m_tuplePivot);
		m_Rotation = std::get<2>(m_tuplePivot);
		m_Translation = std::get<3>(m_tuplePivot);
		m_Mat = std::get<4>(m_tuplePivot);
		m_pTransform->Set_Matrix(m_Mat);

		_matrix RPY = XMMatrixIdentity();
		RPY *= XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		RPY *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
		RPY *= XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_pTransform->Set_Matrix(RPY);


		if (CMap_ToolManager::Get_Instance()->Get_Emplace())
		{
			CMap_ToolManager::OBJECTDESC objDesc;
			ZeroMemory(&objDesc, sizeof(CMap_ToolManager::OBJECTDESC));

			objDesc.strName = m_strName;
			objDesc.Scale = m_Scale;
			objDesc.Rotation = m_Rotation;
			objDesc.Translation = m_Translation;
			objDesc.matrix = m_pTransform->Get_Matrix();
			objDesc.bSave = false;
			CMap_ToolManager::Get_Instance()->Emplace_Object(&objDesc);

			CMap_ToolManager::Get_Instance()->Set_Emplace(false);
		}
	}
	auto pGameInstance = CMap_ToolManager::Get_Instance();

	if (pGameInstance->Get_AllClear())
	{
		m_bDead = true;
		pGameInstance->Set_AllClear(false);
	}

	if (pGameInstance->Get_Delete())
	{

		if (m_strName == pGameInstance->Get_SelectName())
		{
			pGameInstance->Set_Delete(false);

			m_bDead = true;
		}
	}

}

void CMap_Dynamic::SetPhysX(_bool b)
{
	if (b)
	{
		m_bActivePhysX = true;
		if (nullptr != m_pActor)
		{
			m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
		}
	}
	else
	{
		m_bActivePhysX = false;
		if (nullptr != m_pActor)
		{
			m_pActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		}
		Set_Gravity(false);
	}
}

void CMap_Dynamic::Throw()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	PxSetGroupCollisionFlag((_uint)ACTORTAG::INTERACTIVE, (_uint)ACTORTAG::INTERACTIVE, false);
	/*if(pGameInstance->Current_Scene() == SCENE::MINDROOM)
		PxSetGroupCollisionFlag((_uint)ACTORTAG::INTERACTIVE, (_uint)ACTORTAG::STATIC, false);*/

	m_bInteractivable = false;
	m_bThrow = true;
	//m_ActorDesc.bCapture = false;
	shared_ptr<CInteractiveObject> ThisPointer = dynamic_pointer_cast<CInteractiveObject>(shared_from_gameobject());
	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_RecentThrowObject(ThisPointer);

	//if(!m_bActivePhysX)
	//{
	m_bActivePhysX = true;
	SetPhysX(true);
	Set_Gravity(false);
	m_ActorDesc.strTag = ACTORTAG::INTERACTIVE;
	m_ActorDesc.bThrow = true;
	if(nullptr!=m_pActor)
		m_pActor->userData = &m_ActorDesc;


	//eunchan : start throw
	if(static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_Kinesis()->Get_RecentThrowdObject()!=nullptr)
	{
		shared_ptr<CTransform> pEffTransform = static_pointer_cast<CTransform>(static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_Kinesis()->Get_RecentThrowdObject()->Get_Component(COMPONENT::TRANSFORM));
		CEffect_Manager::Get_Instance()->Fetch(VFX::VF_KINESIS_FLOAT_PARTICLE, pEffTransform);
		CEffect_Manager::Get_Instance()->Fetch(VFX::PSY_START_DISTORTION, pEffTransform);

		if (static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_PlayerDesc().eSAS == PLAYER_SAS::ELECTRIC)
		{
			CEffect_Manager::Get_Instance()->Fetch(VFX::ELEC_PSY_PARTICLE1, pEffTransform);
		}

	}

	shared_ptr<CObjectLayer> pMonLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER);
	if (pMonLayer)
	{
		_float fMinDist;
		fMinDist = 60.f;
		if (pGameInstance->Current_Scene() == SCENE::MINDROOM)
			fMinDist = 500.f;
		shared_ptr<CMonster> pCloseMonster = nullptr;
		_float3 vCloseMonsterPos;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);

				if (nullptr == pMonster)
					return true;

				_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				shared_ptr<CModel> pModel = pMonster->Get_Component<CModel>(COMPONENT::MODEL);
				_matrix mat = g_mUnit;
				mat.r[3] = _float4(vMonsterPos, 1.f);
				mat = pModel->Get_Bone(pMonster->GetWeakBoneName().c_str())->Get_CombinedTransformation() * pModel->Get_Pivot() * mat;
				vMonsterPos = mat.r[3];
				if (!pMonster->Get_LockOnAble())
					return true;
				_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vMonsterPos));
				if (fCurDist < fMinDist)
				{
					fMinDist = fCurDist;
					pCloseMonster = pMonster;
					vCloseMonsterPos = vMonsterPos;
				}

				return true;
			});

		if (nullptr != pCloseMonster)
		{
			_float fHeight = 0.f;
			switch (pCloseMonster->Get_Type())
			{
			case CMonster::FLY:
				fHeight = 0.f;
				break;
			default:
				fHeight = 1.f;
				break;
			}
			vCloseMonsterPos.y += fHeight;

			shared_ptr<CModel> pModel = pCloseMonster->Get_Component<CModel>(COMPONENT::MODEL);
			_matrix mat = g_mUnit;
			m_MonsterMatrix = static_pointer_cast<CTransform>(pCloseMonster->Get_Component(COMPONENT::TRANSFORM))->Get_WorldMatrixPtr();
			//_float3 vMonsterPos = static_pointer_cast<CTransform>(pCloseMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			//mat.r[3] = _float4(vMonsterPos, 1.f);
			mat.r[3] = m_MonsterMatrix->row(3);

			m_BoneMatrix = pModel->Get_Bone(pCloseMonster->GetWeakBoneName().c_str())->Get_CombinedTransformationPointer();
			m_Pivot = pModel->Get_Pivot();
			m_vThrowLook = (XMLoadFloat4x4(m_BoneMatrix) * m_Pivot * mat).r[3] - m_pTransform->Get_State(TRANSFORM::POSITION);
			//m_vThrowLook = vCloseMonsterPos - m_pTransform->Get_State(TRANSFORM::POSITION);
			m_vThrowLook = XMVector3Normalize({ m_vThrowLook.x,m_vThrowLook.y,m_vThrowLook.z,0.f });
		}
		else
		{
			m_vThrowLook = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::LOOK);
		}
		if(nullptr!=m_pActor)
			m_pActor->addForce(PxVec3(m_vThrowLook.x * m_fThrowForce, m_vThrowLook.y * m_fThrowForce, m_vThrowLook.z * m_fThrowForce));
	}
	//}
}

_bool CMap_Dynamic::Return()
{
	if (!m_bAlive)
	{
		if (nullptr != m_pActor)
		{
			CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
			m_pActor->release();
			m_pActor = nullptr;
		}
		return true;
	}
	else
		return false;
}

void CMap_Dynamic::ThrowHit()
{
	PxSetGroupCollisionFlag((_uint)ACTORTAG::INTERACTIVE, (_uint)ACTORTAG::INTERACTIVE, true);
	auto pGameInstance = CGameInstance::Get_Instance();
	CPlayer* pPlayer = static_cast<CPlayer*>(pGameInstance->Get_Player());
	m_bThrowHit = true;
	SetPhysX(false);

	_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
	pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
	pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
	pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
	pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
	pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.3f);
	
	Interactive_PhychoKinesis(false);

	shared_ptr<CCamera_Player> pCam = static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_Camera();
	_float4x4 originMat = m_pTransform->Get_Matrix();

	m_pTransform->Translate(XMVector3Normalize(pCam->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION)));
	_float3 effPos = m_pTransform->Get_State(TRANSFORM::POSITION);

	m_pTransform->Set_Matrix(originMat);

	//CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::HIT_REGULAR1,
	//	initializer_list<any>{_float3(0, 0, 0), _float3(0, 0, 0), mat, mat, mat});

	//eunchan :  throwhit
//	CEffect_Manager::Get_Instance()->Fetch(VFX::VF_KINESIS_HIT_PARTICLE, effPos);
	pCam->Set_Shake(0.5f, 5.f);

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_CarCrushed.wav"), g_fPlayerSound*2.f);
}

void CMap_Dynamic::Cancel()
{
	SetPhysX(true);
	m_ActorDesc.bCapture = false;
	if (nullptr != m_pActor)
	{
		m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
		m_pActor->addForce(PxVec3(0, -5, 0));
	}
	if (m_pDuplicatedObject)
		m_pDuplicatedObject->Cancel();
	Interactive_PhychoKinesis(false);
}

void CMap_Dynamic::Destroy()
{
	m_bDissolve = true;
	if (m_pDuplicatedObject)
		m_pDuplicatedObject->Destroy();
	Interactive_PhychoKinesis(false);
}

shared_ptr<CMap_Dynamic> CMap_Dynamic::Duplicate()
{
	m_bReadyDuplicate = true;

	if (m_bStrong)
		m_fDuplicateTime = 2.3f;
	else
		m_fDuplicateTime = 0.5f;
	return nullptr;

}

void CMap_Dynamic::Set_Target_Monster()
{
	shared_ptr<CObjectLayer> pMonLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER);
	if (pMonLayer)
	{
		_float fMinDist = 60.f;
		m_pTargetMonster = nullptr;
		_float3 vCloseMonsterPos;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);

				if (nullptr == pMonster || pMonster->GetDead())
					return true;

				_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				//if (dynamic_pointer_cast<CMonster_DispenPerry>(_pObject))
				//{
				shared_ptr<CModel> pModel = pMonster->Get_Component<CModel>(COMPONENT::MODEL);
				_matrix mat = g_mUnit;
				mat.r[3] = _float4(vMonsterPos, 1.f);
				mat = pModel->Get_Bone(pMonster->GetWeakBoneName().c_str())->Get_CombinedTransformation() * pModel->Get_Pivot() * mat;
				vMonsterPos = mat.r[3];
				//}
				_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vMonsterPos));
				if (fCurDist < fMinDist)
				{
					fMinDist = fCurDist;
					m_pTargetMonster = pMonster;
					m_pTargetMonsterTransform = static_pointer_cast<CTransform>(m_pTargetMonster->Get_Component(COMPONENT::TRANSFORM));
					vCloseMonsterPos = vMonsterPos;
				}
				return true;
			}
		);
	}
}

void CMap_Dynamic::Set_ThrowLook()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CObjectLayer> pMonLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER);
	if (pMonLayer)
	{
		_float fMinDist;
		fMinDist = 60.f;
		if (pGameInstance->Current_Scene() == SCENE::MINDROOM)
			fMinDist = 500.f;
		shared_ptr<CMonster> pCloseMonster = nullptr;
		_float3 vCloseMonsterPos;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);

				if (nullptr == pMonster || pMonster->Get_Type() != CMonster::MONSTERTYPE::STATUE)
					return true;

				_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				shared_ptr<CModel> pModel = pMonster->Get_Component<CModel>(COMPONENT::MODEL);
				_matrix mat = g_mUnit;
				mat.r[3] = _float4(vMonsterPos, 1.f);
				mat = pModel->Get_Bone(pMonster->GetWeakBoneName().c_str())->Get_CombinedTransformation() * pModel->Get_Pivot() * mat;
				vMonsterPos = mat.r[3];
				if (!pMonster->Get_LockOnAble())
					return true;
				_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vMonsterPos));
				if (fCurDist < fMinDist)
				{
					fMinDist = fCurDist;
					pCloseMonster = pMonster;
					vCloseMonsterPos = vMonsterPos;
				}

				return true;
			});

		if (nullptr != pCloseMonster)
		{
			_float fHeight = 0.f;
			switch (pCloseMonster->Get_Type())
			{
			case CMonster::FLY:
				fHeight = 0.f;
				break;
			default:
				fHeight = 1.f;
				break;
			}
			vCloseMonsterPos.y += fHeight;

			shared_ptr<CModel> pModel = pCloseMonster->Get_Component<CModel>(COMPONENT::MODEL);
			_matrix mat = g_mUnit;
			m_MonsterMatrix = static_pointer_cast<CTransform>(pCloseMonster->Get_Component(COMPONENT::TRANSFORM))->Get_WorldMatrixPtr();
			//_float3 vMonsterPos = static_pointer_cast<CTransform>(pCloseMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			//mat.r[3] = _float4(vMonsterPos, 1.f);
			mat.r[3] = m_MonsterMatrix->row(3);

			m_BoneMatrix = pModel->Get_Bone(pCloseMonster->GetWeakBoneName().c_str())->Get_CombinedTransformationPointer();
			m_Pivot = pModel->Get_Pivot();
			m_vThrowLook = (XMLoadFloat4x4(m_BoneMatrix) * m_Pivot * mat).r[3] - m_pTransform->Get_State(TRANSFORM::POSITION);
			//m_vThrowLook = vCloseMonsterPos - m_pTransform->Get_State(TRANSFORM::POSITION);
			m_vThrowLook = XMVector3Normalize({ m_vThrowLook.x,m_vThrowLook.y,m_vThrowLook.z,0.f });
		}
		else
		{
			m_vThrowLook = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::LOOK);
		}
	}
}

void CMap_Dynamic::Set_Gravity(_bool b)
{
	if (nullptr != m_pActor)
	{
		m_pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !b);
		//if not addforce, not fall even if gravity active
		if (b)
			m_pActor->addForce(PxVec3(0, -5, 0));
	}
}


void CMap_Dynamic::Set_DuplicatedObjectPos()
{
	std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
}
