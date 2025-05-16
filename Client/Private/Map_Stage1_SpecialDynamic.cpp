#include "ClientPCH.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "GameInstance.h"
#include "Frustom.h"

#include "Map_Tool.h"
#include "Map_ToolManager.h"

#include "Model.h"
#include "PhysXEngine.h"
#include "Player.h"
#include "Camera_Player.h"
#include "PlayerState.h"
#include "PlayerSpecialThrowState.h"
#include "Psychokinesis.h"

#include "Monster.h"
#include "PhysXCollider.h"
#include "Effect_Manager.h"
#include "Scene_Mizuha.h"
#include <regex>

#include "UI_Indicator_SpcObjectAttack_Rotation_Beam.h"
#include "UI_Indicator_SpcObjectAttack_Rock.h"

#include "Effect_Manager.h"
#include "ScreenEffectManager.h"

int g_iStage1SpecialDynamicIndex = { 0 };

CMap_Stage1_SpecialDynamic::CMap_Stage1_SpecialDynamic(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CMap_Special_Dynamic(_pDevice, _pContext)
{

}

CMap_Stage1_SpecialDynamic::CMap_Stage1_SpecialDynamic(const CMap_Stage1_SpecialDynamic& _rhs)
    :CMap_Special_Dynamic(_rhs)
{
	
}

HRESULT CMap_Stage1_SpecialDynamic::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX, g_aNull);

	Add_ModelCom();
	Add_SpecialType();
	return S_OK;
}

HRESULT CMap_Stage1_SpecialDynamic::Initialize(any pArg)
{
    if (FAILED(__super::Initialize(pArg)))
    {
        MSG_RETURN(E_FAIL, "CMap_Stage1_SpecialDynamic::Initialize", "Failed to CGameObject::Initialize");
    }

	//std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring> tObjTuple =
	//	any_cast<std::tuple<SCENE, wstring, _float3, _float3, _float3, _float4x4, wstring>>(pArg);

	//m_copyTuplePivot = tObjTuple;

	//if (std::get<0>(tObjTuple) == SCENE::MAPTOOL)
	//{
	//	MSG_RETURN(E_FAIL, "CMap_Stage1_SpecialDynamic::Initialize", "Failed to CMap_Stage1_SpecialDynamic::Initialize");
	//}

	PxSetGroup(*m_pActor, (_uint)ACTORTAG::INTERACTIVE_SPECIAL);
	m_ActorDesc.strTag = ACTORTAG::INTERACTIVE_SPECIAL;
	m_ActorDesc.bThrow = false;
	m_ActorDesc.bThrowHit = false;
	m_ActorDesc.stAttackDesc.iDamage = 10;
	m_pActor->userData = &m_ActorDesc;


	ACTORDESC actorDesc;
	actorDesc.strTag = ACTORTAG::INTERACTIVE_BEAM;
	switch(m_eType)
	{
	case SPECIAL_THROW::HEAVY_EQUIPMENT:
		actorDesc.stAttackDesc.iDamage = 200.f;
		actorDesc.bActive = true;
		break;
	case SPECIAL_THROW::BEAM:
		actorDesc.stAttackDesc.iDamage = 2.f;
		actorDesc.bActive = false;
		break;
	case SPECIAL_THROW::ROCK:
		actorDesc.stAttackDesc.iDamage = 5.f;
		actorDesc.bActive = false;
		break;
	case SPECIAL_THROW::BULLDOZER:
		actorDesc.stAttackDesc.iDamage = 5.f;
		actorDesc.bActive = false;
		break;
	case SPECIAL_THROW::BEAM_PIECE:
		actorDesc.stAttackDesc.iDamage = 5.f; 
		actorDesc.bActive = false;
		break;
	}

	m_pCollider = make_shared<CPhysXCollider>();
	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::BOX;

	if(m_eType== SPECIAL_THROW::HEAVY_EQUIPMENT)
		desc.vHalfExtents = m_vExtents * 1.5f;
	else
		desc.vHalfExtents = m_vExtents * 1.1f;
	m_pCollider->Initialize(desc, actorDesc);
	SetPhysX(false);

#pragma region UI
	m_pSpcObject_Rotation_Beam = dynamic_pointer_cast<CUI_Indicator_SpcObjectAttack_Rotation_Beam>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROTATION_BEAM));
	//CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_UI)->Add(m_pSpcObject_Rotation_Beam);
	m_pSpcObject_Rock = dynamic_pointer_cast<CUI_Indicator_SpcObjectAttack_Rock>(CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_INDICATOR_SPCOBJECTATTACK_ROCK));
#pragma endregion
	
	return S_OK;
}

#if ACTIVATE_IMGUI
#include "ImGui_Manager.h"
static _bool bCheckOnce(true);
#endif
void CMap_Stage1_SpecialDynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if(CGameInstance::Get_Instance()->Current_Scene()!=SCENE::MAPTOOL)
	{
		m_pSpcObject_Rotation_Beam->Tick(fTimeDelta);
		m_pSpcObject_Rock->Tick(fTimeDelta);
	}

	if (m_bThrowStart)
	{
		m_fThrowReadyTime += fTimeDelta;
		switch (m_eType)
		{
		case SPECIAL_THROW::HEAVY_EQUIPMENT:
			Throw_Ready_Heavy_Equipment(fTimeDelta);
			break;
		case SPECIAL_THROW::BEAM:
			Throw_Ready_Beam(fTimeDelta);
			break;
		case SPECIAL_THROW::ROCK:
			Throw_Ready_Rock(fTimeDelta);
			break;
		case SPECIAL_THROW::BULLDOZER:
			Throw_Ready_Bulldozer(fTimeDelta);
			break;
		}
	}

	
		PxTransform p;
		if(nullptr!=m_pActor)
		{
			p.p = m_pActor->getGlobalPose().p;
			p.q = m_pActor->getGlobalPose().q;
			m_pCollider->GetActor()->setGlobalPose(p);
			m_ActorDesc.vOwnerPose = m_pTransform->Get_State(TRANSFORM::POSITION);
		}
	

	switch (m_eType)
	{
	case SPECIAL_THROW::HEAVY_EQUIPMENT:
		Tick_HeavyEquipment();
		break;
	case SPECIAL_THROW::BEAM:
		break;
	case SPECIAL_THROW::ROCK:
		Tick_Rock(fTimeDelta);
		break;
	case SPECIAL_THROW::BULLDOZER:
		Tick_Bulldozer(fTimeDelta);
		break;
	}
}

void CMap_Stage1_SpecialDynamic::Late_Tick(_float fTimeDelta)
{
    __super::Late_Tick(fTimeDelta);

	if (CGameInstance::Get_Instance()->Current_Scene() != SCENE::MAPTOOL)
	{
		m_pSpcObject_Rotation_Beam->Late_Tick(fTimeDelta);
		m_pSpcObject_Rock->Late_Tick(fTimeDelta);
	}
}

HRESULT CMap_Stage1_SpecialDynamic::Render()
{
	if (!m_bIsRender)
		return S_OK;

    if (FAILED(CMap_Special_Dynamic::Render()))
    {
        MSG_RETURN(E_FAIL, "CMap_Stage1_SpecialDynamic::Render", "Failed to CMap_Stage1_SpecialDynamic::Render");
    }

	
	return S_OK;
}

HRESULT CMap_Stage1_SpecialDynamic::Render_OutLine()
{
	return S_OK;
}

HRESULT CMap_Stage1_SpecialDynamic::Ready_Components()
{
    if (FAILED(CInteractiveObject::Ready_Components()))
    {
        MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to CGameObject::Ready_Components");
    }

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pDissolveTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_ONE_002));
	if (nullptr == m_pDissolveTexture)
	{
		MSG_RETURN(E_FAIL, "CTestCube::Ready_Components", "Failed to Get_Component: TEXTURE");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	return S_OK;
}

void CMap_Stage1_SpecialDynamic::Add_ModelCom()
{
    if (g_iStage1SpecialDynamicIndex == 0)
    {
        m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA, g_aNull);
    }
    else if (g_iStage1SpecialDynamicIndex == 1)
    {
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTG, g_aNull);
	}
	else if (g_iStage1SpecialDynamicIndex == 2)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPECIAL_ROCK0, g_aNull);
	}
	else if (g_iStage1SpecialDynamicIndex == 3)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_HEAVYEQUIPMENTH, g_aNull);
	}
	else if (g_iStage1SpecialDynamicIndex == 4)
	{
		m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_SPECIAL_BEAMA_PIECE, g_aNull);
	}
}

void CMap_Stage1_SpecialDynamic::Add_SpecialType()
{
	if (g_iStage1SpecialDynamicIndex == 0)
	{
		m_eType = SPECIAL_THROW::BEAM;
		m_vExtents = { 3.2f,0.5f,1.f };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 7.f;
		m_fPickWorldDistance = 50.f;
	}
	else if (g_iStage1SpecialDynamicIndex == 1)
	{
		m_eType = SPECIAL_THROW::HEAVY_EQUIPMENT;
		m_vExtents = { 7.f,7.f,12.f };
		m_vLocalPose = { 0,6.3f,0 };
		m_fMass = 0.006f;
		m_fPickWorldDistance = 1000.f;
		m_bCanPick = false;
	}
	else if (g_iStage1SpecialDynamicIndex == 2)
	{
		m_eType = SPECIAL_THROW::ROCK;
		m_vExtents = { 1.2f,1.3f,1.8f };
		m_vLocalPose = { 0,0.4,0 };
		m_fMass = 7000.f;
		m_fPickWorldDistance = 50.f;
	}
	else if (g_iStage1SpecialDynamicIndex == 3)
	{
		m_eType = SPECIAL_THROW::BULLDOZER;
		m_vExtents = { 4.f,2.7f,5.5f };
		m_vLocalPose = { 0.3f,2.7f,1.3f };
		m_fMass = 60.f;
		m_fPickWorldDistance = 50.f;
	}
	else if (g_iStage1SpecialDynamicIndex == 4)
	{
		m_eType = SPECIAL_THROW::BEAM_PIECE;
		m_vExtents = { 3.2f,0.3f,0.5f };
		m_vLocalPose = { 0,0.8,0 };
		m_fMass = 7.f;
		m_fPickWorldDistance = 50.f;
	}
}

void CMap_Stage1_SpecialDynamic::Throw_Ready_Heavy_Equipment(_float fTimeDelta)
{
	_bool b = m_pActor->getActorFlags().isSet(PxActorFlag::eDISABLE_SIMULATION);

	//cout << m_fThrowReadyTime << endl;
	auto pGameInstance = CGameInstance::Get_Instance();
	if (m_fThrowReadyTime > 1.f && m_fThrowReadyTime < 2.f)
	{
		if(!m_bExecuted[93])
		{
			m_bExecuted[93] = true;
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_DumpTruck_LiftUp.wav"), g_fPlayerSound);
		}
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 8.f);
		if (m_fThrowReadyTime < 1.5f)
			m_pTransform->Rotate(TRANSFORM::RIGHT, fTimeDelta * 1.5f);
		else
			m_pTransform->Rotate(TRANSFORM::RIGHT, fTimeDelta * 1.f);
	}
	else if (m_fThrowReadyTime >= 2.f && m_fThrowReadyTime < 4.f)
	{
		if (!m_bExecuted[1])
		{
			m_bExecuted[1] = true;
			m_vHeavyPrePos = m_pTransform->Get_State(TRANSFORM::POSITION);
		}
		
		_float3 vMonPos = static_pointer_cast<CTransform>(dynamic_pointer_cast<CScene_Mizuha>(pGameInstance->Get_Current_Scene_Ptr())->Get_DispenPerry()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
		vMonPos.y = 10.f;
		m_pTransform->Set_State(TRANSFORM::POSITION,(Function::Lerp(m_pTransform->Get_State(TRANSFORM::POSITION),
			vMonPos, min(1.f,(m_fThrowReadyTime -2.f)/2.f))));
		m_pTransform->Rotate_Lerp(TRANSFORM::RIGHT, fTimeDelta * 0.25f);
		if (m_fThrowReadyTime > 3.3f)
		{
			static_pointer_cast<CPlayerSpecialThrowState>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerState())->Set_Throw(true);
		}
	/*	if (m_fThrowReadyTime < 3.f)
		{
			m_pTransform->Translate({ 0,(m_fThrowReadyTime - 2.f) * -fTimeDelta * 3.f,0,0 });
		}
		else
		{
			m_pTransform->Translate({ 0,(m_fThrowReadyTime - 3.f) * fTimeDelta * 3.f,0,0 });
		}*/
	}
	else if (m_fThrowReadyTime >= 4.f && m_fThrowReadyTime < 5.f)
	{
		_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
		_float3 vMonPos = static_pointer_cast<CTransform>(dynamic_pointer_cast<CScene_Mizuha>(pGameInstance->Get_Current_Scene_Ptr())->Get_DispenPerry()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
		m_pTransform->Set_State(TRANSFORM::POSITION, { vMonPos.x, vPos.y, vMonPos.z });
		m_pTransform->Translate({ 0,Function::Lerp(fTimeDelta * 50.f,0.f,(min(1.f,m_fThrowReadyTime - 4.f))),0,0 });
	}
	else if(m_fThrowReadyTime >= 5.f)
	{
		if (!m_bExecuted[2])
		{
			m_bExecuted[2] = true;
			m_pCollider->GetActorDesc().bActive = true;
			Throw();
		}
		if(!m_pCollider->GetActorDesc().bThrowHit)
		{
			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 vMonPos = static_pointer_cast<CTransform>(dynamic_pointer_cast<CScene_Mizuha>(pGameInstance->Get_Current_Scene_Ptr())->Get_DispenPerry()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			m_pTransform->Set_State(TRANSFORM::POSITION, { vMonPos.x, vPos.y, vMonPos.z });
			m_pTransform->Translate({ 0,-fTimeDelta * 100.f,0,0 });
		}
		else
		{
			if(!m_bExecuted[50])
			{
				m_bExecuted[50] = true;
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_DumpTruck_Smash.wav"), g_fPlayerSound);
				
				ThrowHit();
				CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::SUNNY_FILTER);
				m_ActorDesc.bThrowHit = true;

				_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
				_float3 Dir = vPlayerPos - Pos;
				Dir *= 0.5f;
				Pos += Dir;
				Pos.y -= 5.f;

				/*CEffect_Manager::Get_Instance()->Fetch(VFX::DUMP_PARTICLE, Pos);
				CEffect_Manager::Get_Instance()->Fetch(VFX::DUMP_PARTICLE1, Pos);
				CEffect_Manager::Get_Instance()->Fetch(VFX::DUMP_HIT0, Pos);
				CEffect_Manager::Get_Instance()->Fetch(VFX::DUMP_HIT1, Pos);*/

				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_DUMP,
					initializer_list<any>{Pos, Pos, Pos, Pos});
			}
		}
	}
}

void CMap_Stage1_SpecialDynamic::Throw_Ready_Beam(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	_float3 vCloseMonsterPos;
	shared_ptr<CObjectLayer> pMonLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_MONSTER);
	if (pMonLayer)
	{
		_float fMinDist = 100.f;
		shared_ptr<CMonster> pCloseMonster = nullptr;
		pMonLayer->Iterate_Objects(
			[&](shared_ptr<CGameObject> _pObject)->bool
			{
				shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);

				if (nullptr == pMonster)
					return true;

				_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
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
			m_vThrowLook = vCloseMonsterPos - m_pTransform->Get_State(TRANSFORM::POSITION);
			m_vThrowLook = XMVector3Normalize({ m_vThrowLook.x,m_vThrowLook.y,m_vThrowLook.z,0.f });
		}
		else
		{
			m_vThrowLook = static_pointer_cast<CTransform>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::LOOK);
		}
		pCloseMonster->SetBulldozerHit(true);
	}

	if(!m_bBeamHit)
	{
		if (m_fThrowReadyTime < 1.f)
		{
			SetPhysX(false);
			m_pTransform->Translate({ 0.f,fTimeDelta * 12.f,0.f,0.f });
			if(!m_bExecuted[10])
			{
				m_bExecuted[10] = true;
				m_pCollider->GetActorDesc().bThrow = true;
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_Down.wav"), g_fPlayerSound);
			}
		}
		if (m_fThrowReadyTime >= 1.f && m_fThrowReadyTime < 20.f)
		{
			//m_ActorDesc.bThrow = true;
			shared_ptr<CInteractiveObject> ThisPointer = dynamic_pointer_cast<CInteractiveObject>(shared_from_gameobject());
			static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_RecentThrowObject(ThisPointer);
			if (!m_pCollider->GetActorDesc().bThrowHit)
			{
				m_pTransform->Translate(m_vThrowLook * fTimeDelta * 30.f);
			}
			else
			{
				m_pCollider->GetActorDesc().bActive = true;
				m_bBeamHit = true;
				m_fThrowReadyTime = 0.f;
				if(nullptr!=m_pActor)
				{
					pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
					m_pActor->release();
					m_pActor = nullptr;
				}
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_PitchDown.wav"), g_fPlayerSound);
			}
			if (m_fThrowReadyTime > 4.f)
			{
				static_pointer_cast<CPlayerSpecialThrowState>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerState())->Set_Throw(true);
			}
		}
	}
	else
	{
		if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 1.f)
		{
			if (!m_bExecuted[0])
			{
				m_bExecuted[0] = true;
				m_bIsRender = false;
				shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC);
				for (int i = 0; i < 5; i++)
				{
					std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
					shared_ptr<CMap_Stage1_SpecialDynamic> pBeam = static_pointer_cast<CMap_Stage1_SpecialDynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_BEAMA_PIECE, m_copyTuplePivot));
					pLayer->Add(pBeam);
					m_DuplicatedBeam.push_back(pBeam);
					shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
					_float3 val[5] = { {-0.8f,1.1f,-1.2f},{1.1f,1.f,-1.f},{-0.1f,1.f,0.1f}, {1.1f,1.f,1.f},{-0.9f,0.8f,0.9f} };
					_float3 vDir = val[i];
					vDir.y *= 1.5f;

					m_vHitDir[i] = vDir;
					pTransform->Rotate_Lerp(TRANSFORM::RIGHT, vDir.x,1.f);
					pTransform->Rotate_Lerp(TRANSFORM::UP, vDir.y,1.f);
					pTransform->Rotate_Lerp(TRANSFORM::LOOK, vDir.z,1.f);
				}
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_CapturedObject(m_DuplicatedBeam[2]);
				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_RecentThrowObject(m_DuplicatedBeam[2]);
				_float4x4 mat = m_pTransform->Get_Matrix();
				CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_BEAM,
					initializer_list<any>{mat, mat, mat,
					mat, mat, mat,
					mat, mat, mat});
			}

			for (int i = 0; i < 5; i++)
			{
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Translate(XMVectorLerp(m_vHitDir[i] * fTimeDelta * 8.f, _float3(0, 0, 0), m_fThrowReadyTime));
				pTransform->Translate({ 0,-m_fThrowReadyTime * m_fThrowReadyTime * 0.2f,0 });
			}
		}
		if (m_fThrowReadyTime >= 1.f && m_fThrowReadyTime < 1.f + m_fBeamRotTime)
		{
			if (!m_bExecuted[33])
			{
				m_bExecuted[33] = true;
				m_pSpcObject_Rotation_Beam->Set_RenderStart();
				CGameInstance::Get_Instance()->PlaySoundeffect(TEXT("SFX_SpecialAttack_H_Beam_Full.wav"),CSound_Manager::SOUND_CHARGE, g_fPlayerSound);
				CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::SUNNY_FILTER);

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

			//m_pTransform->Rotate(TRANSFORM::UP, fTimeDelta * 10.f);
			m_fBeamRotTime += fabs((pGameInstance->Get_CursorMove().x + pGameInstance->Get_CursorMove().y) * fTimeDelta*0.003f);
			m_fBeamRotTime = min(2.f, m_fBeamRotTime);
			//cout << m_fBeamRotTime <<endl;
			m_fBeamHitTime -= fTimeDelta;
			_bool bHit = false;
			for (int i = 0; i < 5; i++)
			{
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Rotate(TRANSFORM::LOOK, fTimeDelta * 10.f);
				if (m_fBeamHitTime < 0.f)
				{
					m_DuplicatedBeam[i]->Get_Collider()->GetActorDesc().bActive = true;
					m_DuplicatedBeam[i]->Get_Collider()->GetActorDesc().bThrow = true;
					if (m_DuplicatedBeam[i]->Get_Collider()->GetActorDesc().bThrowHit && !m_bBeamThrowHit && m_fThrowReadyTime > 1.f + m_fBeamRotTime*0.5f)
					{
						m_bBeamThrowHit = true;
						ThrowHit();
					}
				}
			}
			if (m_fBeamHitTime < 0.f)
			{
				CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_H_Beam_1Hit.wav"), g_fPlayerSound);
				m_fBeamHitTime = 0.4f;
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
			Throw();
			for (int i = 0; i < 5; i++)
			{
				static_pointer_cast<CMap_Stage1_SpecialDynamic>(m_DuplicatedBeam[i])->Throw();
			}
			Interactive_PhychoKinesis(false);
			m_bThrowStart = false;

			//m_pSpcObject_Rotation_Beam->Reset();
		}
	}
}
void CMap_Stage1_SpecialDynamic::Throw_Ready_Rock(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (!m_bExecuted[25])
	{
		m_bExecuted[25] = true;
		Set_Target_Monster();
		if (nullptr != m_pTargetMonster)
			m_pTargetMonster->SetBulldozerHit(true);
	}

	if (nullptr == m_pTargetMonsterTransform)
	{
		return;
	}
	if (m_fThrowReadyTime >= 0.f && m_fThrowReadyTime < 3.f)
	{
		_float3 vPos = m_pTargetMonsterTransform->Get_State(TRANSFORM::POSITION);
		vPos.y = m_pTransform->Get_State(TRANSFORM::POSITION).y;
		m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION), vPos, min(1.f, m_fThrowReadyTime / 3.f)));
		m_pTransform->Translate({ 0,fTimeDelta * 5.f,0,0 });
	}

	if (m_fThrowReadyTime >= 3.f && m_fThrowReadyTime < 5.f)
	{
		if (!m_bExecuted[11])
		{
			m_bExecuted[11] = true;
			m_pCollider->GetActorDesc().bActive = true;
			Throw();
		}
	}
	else if (m_fThrowReadyTime >= 5.3f)
	{
		if (!m_pCollider->GetActorDesc().bThrowHit)
		{
			if (!m_bExecuted[31])
			{
				m_bExecuted[31] = true;
				//m_bSlowStart = true;
			}
			_float3 vPos = m_pTargetMonsterTransform->Get_State(TRANSFORM::POSITION);
			vPos.y = m_pTransform->Get_State(TRANSFORM::POSITION).y;
			m_pTransform->Set_State(TRANSFORM::POSITION, XMVectorLerp(m_pTransform->Get_State(TRANSFORM::POSITION), vPos, min(1.f, m_fThrowReadyTime - 5.f)));
			m_pTransform->Translate({ 0,-fTimeDelta * 25.f,0,0 });
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
					initializer_list<any>{Pos, Pos, Pos, Pos});

				for (int i = 0; i < 7; i++)
				{
					_float3 scale[7] = { {0.9f,0.5,0.9f},{1.2f,0.5f,0.9f},{1.5f,0.5f,1.5f}, {0.9f,0.5f,0.9f},{1.1f,0.5f,1.2f},{0.9f,0.5f,0.9f},{0.7f,0.5f,1.1f} };
					shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC);
					std::get<2>(m_copyTuplePivot) = scale[i] * 1.f;
					std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
					shared_ptr<CMap_Stage1_SpecialDynamic> pBeam = static_pointer_cast<CMap_Stage1_SpecialDynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0, m_copyTuplePivot));
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
					pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
					m_pActor->release();
					m_pActor = nullptr;
				}
				m_bIsRender = false;
			}

			for (int i = 0; i < 7; i++)
			{
				shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM));
				pTransform->Translate({ m_vRockHitDir[i].x * fTimeDelta * 20.f,m_vRockHitDir[i].y * fTimeDelta * 20.f,m_vRockHitDir[i].z * fTimeDelta *  20.f });
				pTransform->Rotate(TRANSFORM::RIGHT, fTimeDelta * 5.f);
				pTransform->Rotate(TRANSFORM::UP, fTimeDelta * 5.f);
				pTransform->Rotate(TRANSFORM::LOOK, fTimeDelta * 5.f);
			}

			if (m_fThrowReadyTime >= 5.2f)
			{
				if (!m_bExecuted[21])
				{
					m_fSlowTime = 2.f;
					m_bExecuted[21] = true;
					m_bSlowStart = true;
				}
			}
		}
	}

	if(m_bHitSuccess)
	{
		if (m_fThrowReadyTime < 2.f)
		{
			if (m_DuplicatedBeam.empty())
				return;

			if (!m_bExecuted[18])
			{
				for (int i = 0; i < 7; i++)
				{

					m_bExecuted[18] = true;
					m_DuplicatedBeam[i]->Get_Collider()->GetActorDesc().bActive = true;
				}
			}
			for (int i = 0; i < 7; i++)
			{
				static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM))->Translate({ m_vRockHitDir[i].x * fTimeDelta * -10.f,m_vRockHitDir[i].y * fTimeDelta * -10.f,m_vRockHitDir[i].z * fTimeDelta * -10.f });


				_float3 vDir = m_pTargetMonsterTransform->Get_State(TRANSFORM::POSITION) - static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
				vDir = XMVector3Normalize(vDir);
				static_pointer_cast<CTransform>(m_DuplicatedBeam[i]->Get_Component(COMPONENT::TRANSFORM))->Translate({ vDir.x * fTimeDelta * 10.f,vDir.y * fTimeDelta * 10.f,vDir.z * fTimeDelta * 10.f });
				if (m_DuplicatedBeam[i]->Get_Collider()->GetActorDesc().bThrowHit)
				{
					if (nullptr != m_DuplicatedBeam[i]->Get_Actor())
					{
						pGameInstance->Get_PhysXEngine()->GetScene()->removeActor(*m_DuplicatedBeam[i]->Get_Actor());
						m_DuplicatedBeam[i]->Get_Actor()->release();
						m_DuplicatedBeam[i]->Reset_Actor();
						m_DuplicatedBeam[i]->Set_Render(false);
					}

					if (i == 0)
						m_fDebrisFallTime += fTimeDelta;

					if (!m_bExecuted[26])
					{
						m_bExecuted[26] = true;

						_float3 RandomPos[10] = {};
						CGameInstance::Get_Instance()->PlaySoundW(TEXT("Environment_SFX_RockCurshed.wav"), g_fPlayerSound);
						for (_int i = 0; i < 10; i++)
						{
							RandomPos[i] = m_pTransform->Get_State(TRANSFORM::POSITION);
							std::random_device rd;
							std::mt19937 gen(rd());
							std::uniform_real_distribution<_float> xDist(-2.5f, 2.5f);
							std::uniform_real_distribution<_float> yDist(-1.f, 1.f);
							std::uniform_real_distribution<_float> zDist(-2.5f, 2.5f);
							RandomPos[i].x += xDist(gen);
							RandomPos[i].y += yDist(gen);
							RandomPos[i].z += zDist(gen);
						}

						CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::SPERICAL_ROCK1,
							initializer_list<any>{RandomPos[0], RandomPos[1], RandomPos[2],
							RandomPos[3], RandomPos[4], RandomPos[5]
							, RandomPos[6], RandomPos[7], RandomPos[8], RandomPos[9]});

						for (int i = 0; i < 10; i++)

						{
							shared_ptr<CObjectLayer> pLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC);
							std::get<2>(m_copyTuplePivot) = _float3(0.05f, 0.1f, 0.1f);
							std::get<4>(m_copyTuplePivot) = m_pTransform->Get_State(TRANSFORM::POSITION);
							shared_ptr<CMap_Stage1_SpecialDynamic> pBeam = static_pointer_cast<CMap_Stage1_SpecialDynamic>(pGameInstance->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_MAP_SPECIAL_ROCK0, m_copyTuplePivot));
							pLayer->Add(pBeam);
							m_Debris.push_back(pBeam);
							m_vDebrisHitDir[i] = { Function::RandomFloat(-3.f,3.f),Function::RandomFloat(-3.f,3.f),Function::RandomFloat(-3.f,3.f) };
						}
						
						m_fSlowTime = 0.3f;
						m_bSlowStart = true;
					}
				}
			}

			for (int i = 0; i < m_Debris.size(); i++)
			{
				static_pointer_cast<CTransform>(m_Debris[i]->Get_Component(COMPONENT::TRANSFORM))->Translate({ m_vDebrisHitDir[i].x * fTimeDelta * 10.f, m_vDebrisHitDir[i].y * fTimeDelta * 10.f - m_fDebrisFallTime * m_fDebrisFallTime * 5.f, m_vDebrisHitDir[i].z * fTimeDelta * 10.f });
			}
		}
		else if (m_fThrowReadyTime >= 2.f)
		{
			if (!m_bExecuted[20])
			{
				m_bExecuted[20] = true;
				for (int i = 0; i < m_Debris.size(); i++)
				{
					m_Debris[i]->Set_Alive(false);
				}
			}
		}
	}
}

void CMap_Stage1_SpecialDynamic::Throw_Ready_Bulldozer(_float fTimeDelta)
{
	if (!m_bExecuted[13])
	{
		m_bExecuted[13] = true;
		Set_Target_Monster();
		m_pCollider->GetActorDesc().bActive = true;
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_Tracktor0.wav"), g_fPlayerSound);
	}

	if (nullptr == m_pTargetMonster)
	{
		static_pointer_cast<CPlayerSpecialThrowState>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerState())->Set_Throw(true);
		return;
	}

	if (m_fThrowReadyTime > 0.5f && !m_bExecuted[25])
	{
		m_bExecuted[25] = true;
		Throw();
		m_pTransform->LookAt_Interpolation(m_pTargetMonsterTransform);
	}

	if (m_fThrowReadyTime > 1.f && m_fThrowReadyTime<=5.f)
	{
		/* ������ �� */
		m_fTimer += fTimeDelta;
		if (m_fTimer >= 0.25f)
		{
			_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 Dir = vPlayerPos - Pos;

			Dir *= 0.4f;
			Pos += Dir;
			Pos.y += 1.f;

			CEffect_Manager::Get_Instance()->Fetch(VFX::BULLDOZER_SMOKE, Pos);
			m_fTimer = 0.f;
		}

		m_fBeamHitTime -= fTimeDelta;
		if (m_bExecuted[15] && m_fBeamHitTime < 0.f)
		{
			/* ���ο� �ɸ� �� */
			CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::SUNNY_FILTER);

			_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			_float3 Pos = m_pTransform->Get_State(TRANSFORM::POSITION);
			_float3 Dir = vPlayerPos - Pos;

			Dir *= 0.4f;
			Pos += Dir;
			Pos.y += 2.f;

			//CEffect_Manager::Get_Instance()->Fetch(VFX::SPECIAL_BEAM1, Pos);

			m_pCollider->GetActorDesc().bThrow = true;
			m_pCollider->GetActorDesc().bActive = true;
			m_fBeamHitTime = 0.6f;
			m_pTargetMonster->Get_NormalActorDesc()->bThrowHit = true;
			m_bSlowStart = true;
			static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Camera()->Set_Shake(0.1f, 3.f);
		}

		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 15.f);
		if (m_fThrowReadyTime > 4.5f && m_fThrowReadyTime < 5.f)
		{
			//m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 10.f);
		}
	}
	if (m_fThrowReadyTime > 5.f)
	{
		if(!m_bExecuted[14])
		{
			SetPhysX(true);
			Set_Gravity(true);
			m_bExecuted[14] = true;
			m_ActorDesc.bThrow = true;
			static_pointer_cast<CPlayerSpecialThrowState>(static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_PlayerState())->Set_Throw(true);
			m_pActor->addForce({ 0,500000.f,0 });
			CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_SpecialAttack_Tracktor1.wav"), g_fPlayerSound);
		}

	}
	else if (m_pCollider->GetActorDesc().bThrowHit)
	{
		if (!m_bExecuted[15])
		{
			m_bExecuted[15] = true;
			m_bSlowStart = true;
			Interactive_PhychoKinesis(false);
		}
		if(nullptr!=m_pTargetMonster)
		{
			m_pTargetMonsterTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * fTimeDelta * 15.f);
			_float3 vPos = m_pTargetMonsterTransform->Get_State(TRANSFORM::POSITION);
			if(nullptr!=m_pTargetMonster->Get_Controller())
				m_pTargetMonster->Get_Controller()->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
			m_pTargetMonster->SetBulldozerHit(true);
		}
	}
	
}

void CMap_Stage1_SpecialDynamic::Tick_HeavyEquipment()
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (nullptr != m_pActor)
	{
		ACTORDESC* pActorDesc = (ACTORDESC*)m_pActor->userData;
		if (m_pCollider->GetActorDesc().bThrowHit && m_pCollider->GetActorDesc().bThrow)
		{
			if (!m_bExecuted[53])
			{
				m_bExecuted[53] = true;
				ThrowHit();
			}
		}
	}

	if (pGameInstance->Current_Scene() == SCENE::MIZUHA)
	{
		if (static_pointer_cast<CModel>(dynamic_pointer_cast<CScene_Mizuha>(pGameInstance->Get_Current_Scene_Ptr())
			->Get_DispenPerry()->Get_Component(COMPONENT::MODEL))->Is_AnimationPlaying((_uint)ANIMATION::DISPEN_PERRY::ATK_LEAP_JITABATA))
			m_bCanPick = true;
	}
}

void CMap_Stage1_SpecialDynamic::Tick_Rock(_float fTimeDelta)
{
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
			else
			{
				if (pGameInstance->Key_Down('A') || pGameInstance->Key_Down('D'))
				{
					m_iHitCount++;
				}
				if (m_iHitCount > 10)
				{
					if (!m_bExecuted[66])
					{
						m_bExecuted[66] = true;
						m_pSpcObject_Rock->Set_KeyInput(); // �� ���� ��
						CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::SUNNY_FILTER);
					}
					m_fSlowTime = 0.f;
					m_bHitSuccess = true;
					m_fThrowReadyTime = 0.f;
					for(int i=0;i< m_DuplicatedBeam.size();i++)
					{
						m_DuplicatedBeam[i]->Get_Collider()->GetActorDesc().bThrow = true;
					}
					m_ActorDesc.bThrow = true;
				}
			}
		}
}

void CMap_Stage1_SpecialDynamic::Tick_Bulldozer(_float fTimeDelta)
{
	if (nullptr != m_pActor)
	{
		ACTORDESC* pActorDesc = (ACTORDESC*)m_pActor->userData;
		if (m_bSlowStart)
		{
			auto pGameInstance = CGameInstance::Get_Instance();

			if(!m_bSlowEnable)
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
}

shared_ptr<CMap_Stage1_SpecialDynamic> CMap_Stage1_SpecialDynamic::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _int iIndex)
{
	shared_ptr<CMap_Stage1_SpecialDynamic> pInstance = make_private_shared(CMap_Stage1_SpecialDynamic, _pDevice, _pContext);

	g_iStage1SpecialDynamicIndex = iIndex;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_SpecialDynamic::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CMap_Stage1_SpecialDynamic::Clone(any pArg)
{
	shared_ptr<CMap_Stage1_SpecialDynamic> pInstance = make_private_shared_copy(CMap_Stage1_SpecialDynamic, *this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_RETURN(nullptr, "CMap_Stage1_SpecialDynamic::Clone", "Failed to Initialize");
	}

	return pInstance;
}

void CMap_Stage1_SpecialDynamic::Throw()
{
	m_ActorDesc.bThrow = true;
	//m_ActorDesc.bCapture = false;
	m_pCollider->GetActorDesc().bThrow = true;

	shared_ptr<CInteractiveObject> ThisPointer = dynamic_pointer_cast<CInteractiveObject>(shared_from_gameobject());
	static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_RecentThrowObject(ThisPointer);
	if (nullptr != m_pActor)
	{
		m_ActorDesc.strTag = ACTORTAG::INTERACTIVE_SPECIAL;
		m_pActor->userData = &m_ActorDesc;

		static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_SpecialThrow(false);
		static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Set_CapturedObject(nullptr);
		switch (m_eType)
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
		}

		static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Get_Kinesis()->Reset();
	}
}

void CMap_Stage1_SpecialDynamic::ThrowHit()
{
	//m_ThrowHitTime = 1.f;
	auto pGameInstance = CGameInstance::Get_Instance();
	_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
	switch (m_eType)
	{
	case SPECIAL_THROW::HEAVY_EQUIPMENT:
		m_ThrowHitTime = 0.2f;
		m_bThrowHit = true;

		pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.1f);
		break;
	case SPECIAL_THROW::ROCK:
		m_bThrowHit = true;
		break;
	default:
		m_bThrowHit = true;
		pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_SPECIALDYNAMIC, fDefaultTimeScale * 0.1f);
		pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 0.1f);
		break;
	}

}

_bool CMap_Stage1_SpecialDynamic::Return()
{
	if (!m_bAlive)
	{
		if (nullptr != m_pActor)
		{
			CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->removeActor(*m_pActor);
			m_pActor->release();
			m_pActor = nullptr;
		}
		if (nullptr != m_pCollider)
		{
			CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->removeActor(*m_pCollider->GetActor());
			m_pCollider->GetActor()->release();
			m_pCollider = nullptr;
		}
		return true;
	}
	else
		return false;
}
