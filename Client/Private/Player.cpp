#include "ClientPCH.h"
#include "Player.h"
#include "GameInstance.h"
#include "PhysXEngine.h"
#include "PhysXCollider.h"
#include "PlayerControllerHitReport.h"
#include "Camera_Player.h"
#include "PlayerIdleState.h"
#include "PlayerHurtState.h"
#include "Monster.h"
#include "Psychokinesis.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"
#include "UI_Indicator_BrainTalk_Mizuha.h"
#include "Light.h"
#include "PlayerWeapon.h"
#include "PlayerTentacle.h"
#include "UI_SkillSlot.h"
#include "PlayerControllerFilterCallBack.h"
#include "Bone.h"
#include "BrainFieldTentacle.h"
#include "ImGui_Manager.h"
#include "ScreenEffectManager.h"
#include "PlayerBrainFieldExitState.h"

CPlayer::CPlayer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CPlayer::CPlayer(const CPlayer& _rhs)
	:CGameObject(_rhs)
{

}

CPlayer::~CPlayer()
{
	int a = 6;
}

HRESULT CPlayer::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior |= BIT(BEHAVIOR::ANIMATOR);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL] = make_pair(PROTOTYPE_COMPONENT_MODEL_PLAYER, g_aNull);

	return S_OK;
}

HRESULT CPlayer::Initialize(any eScene)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}


	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	m_pPlayerControllerHitReport = make_shared<CPlayerControllerHitReport>();

	m_pTransform->Set_State(TRANSFORM::POSITION, { 0,5,0,1 });

	PxCapsuleControllerDesc cDesc;
	cDesc.material = pGameInstance->Get_PhysXEngine()->GetMaterial();
	cDesc.position = { 0.f,5.f,10.f };
	cDesc.height = 1.0;
	cDesc.radius = 0.5f;
	cDesc.slopeLimit = XMConvertToRadians(50.f);
	cDesc.contactOffset = 0.1f;
	cDesc.stepOffset = 0.02f;
	cDesc.reportCallback = m_pPlayerControllerHitReport.get();

	m_pController = pGameInstance->CreateController(&cDesc);
	m_ActorDesc.strTag = ACTORTAG::PLAYER;
	m_pController->getActor()->userData = &m_ActorDesc;
	PxShape* pShape;
	m_pController->getActor()->getShapes(&pShape, 1);
	PxFilterData pFilterData;
	pFilterData.word0 = (_uint)ACTORTAG::PLAYER;
	pShape->setQueryFilterData(pFilterData);

	m_ControllerFilterCallBack = make_shared<CPlayerControllerFilterCallBack>();
	m_ControllerFilters.mFilterCallback = m_ControllerFilterCallBack.get();
	m_CCTFilterCallBack = make_shared<PlayerCCTFilterCallBack>();
	m_ControllerFilters.mCCTFilterCallback = m_CCTFilterCallBack.get();
	m_ControllerFilters.mFilterFlags = PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER;
	m_ControllerFilters.mFilterData = NULL;

	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(pGameInstance->Current_Scene(), LAYER_CAMERA);
	m_pCamera = static_pointer_cast<CCamera_Player>(pLayer->Get_GameObject(0));

	m_eCurState = make_shared<CPlayerIdleState>();
	m_eCurState->Entry(*this);

	pGameInstance->Set_Player(this);
	m_pModel->SetRootBoneIndex(m_pModel->Get_BoneIndex("Reference"), m_pModel->Get_BoneIndex("Eff01"));

	Initialize_Weapon();
	Initialize_Tentacle();
	Initialize_FootCollider();

	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);
	shared_ptr<CObjectLayer> pKinesisLayer = CGameInstance::Get_Instance()->Add_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PSYCHOKINESIS);

	m_pModel->Hide_Mesh(7);

	m_pPsychokinesis = dynamic_pointer_cast<CPsychokinesis>(
		CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_PSYCHOKINESIS, m_pTransform));
	pKinesisLayer->Add(m_pPsychokinesis);
	m_pPsychokinesis->Set_Player(this);

	m_PlayerDesc.iHp = 750;
	m_PlayerDesc.iKinesis = 100;
	m_PlayerDesc.fAnimSpeed = 2.5f;
	m_PlayerDesc.fSpeed = 7.f;
	m_PlayerDesc.fDashSpeed = 12.f;
	m_PlayerDesc.fJumpHeight = 7.f;
	m_PlayerDesc.fJumpForwardSpeed = 6.f;
	m_PlayerDesc.fJumpRunForwardSpeed = 8.f;
	m_PlayerDesc.fJumpDashForwardSpeed = 12.f;
	m_pSkillSlot = dynamic_pointer_cast<CUI_SkillSlot>(CGameInstance::Get_Instance()->Clone_GameObject(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_UI_SKILLSLOT));
	m_pSkillSlot->Initialize();
	m_PlayerDesc.fJumpAtkHeight = 7.f;
	m_PlayerDesc.fUpperAtkHeight = 10.f;
	m_PlayerDesc.fGravity = 30.f;

	m_PlayerDesc.eSAS = PLAYER_SAS::NONE;
	for (int i = 0; i < 4; i++)
		m_PlayerDesc.SAS_Gauge[i] = 100.f;
	m_PlayerDesc.bSAS_Enable = false;
	m_PlayerDesc.SAS_GaugeIncreaseTime = 12.f;
	m_PlayerDesc.SAS_GaugeDecreaseTime = 10.f;

	m_PlayerDesc.bInvincibility = false;

	m_PlayerDesc.fCritRate = 0.2f;

	m_PlayerDesc.bDriveMode = false;
	m_PlayerDesc.fDriveGauge = 100.f;
	m_PlayerDesc.fDriveGaugeIncrease = 0.5f;
	m_PlayerDesc.fDriveGaugeDecreaseTime = 3.f;

	m_PlayerDesc.fHealValue = 30.f;

	m_PlayerDesc.AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::FORTH] = true;
	m_PlayerDesc.AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::ONECLICKCHARGE] = true;
	m_PlayerDesc.AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::JUMPDASHATK] = true;
	m_PlayerDesc.AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::JUMPDOWNATK] = true;
	m_PlayerDesc.AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::JUSTDODGEATK] = true;
	m_PlayerDesc.AcquiredSkill[(_uint)PLAYER_ADDITIVE_SKILL::SECONDTHROW] = true;

	m_PlayerDesc.bJustDodge = false;
	m_PlayerDesc.fJustDodgeMaxTime = 0.3f;
	m_PlayerDesc.fJustDodgeTime = m_PlayerDesc.fJustDodgeMaxTime;

	m_PlayerDesc.fMaxKinesisGauge = 100.f;
	m_PlayerDesc.fKinesisGauge = m_PlayerDesc.fMaxKinesisGauge;
	m_PlayerDesc.fKinesisConsume = 20.f;
	m_PlayerDesc.fKinesisNormalIncrease = 1.f;
	m_PlayerDesc.fKinesisBigIncrease = 4.f;

	// AMB
	m_vecMeshPassIndexPair.push_back({ 0, 17 });
	m_vecMeshPassIndexPair.push_back({ 2, 17 });
	m_vecMeshPassIndexPair.push_back({ 3, 17 });

	m_vecMeshPassIndexPair.push_back({ 1, 0 });
	m_vecMeshPassIndexPair.push_back({ 6, 0 });

	m_vecDriveModeMeshPassIndexPair.push_back({ 4, 0 });
	m_vecDriveModeMeshPassIndexPair.push_back({ 5, 0 });


	PxSetGroup(*m_pController->getActor(), (_uint)ACTORTAG::PLAYER);
	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Get_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER) < 0.05f)
		return;

	__super::Tick(fTimeDelta);
	PxTransform pt = m_pController->getActor()->getGlobalPose();
	m_pSkillSlot->Tick(fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Down('O'))
	{
		_float4x4 mOriginal = m_pTransform->Get_Matrix();
		m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), -XM_PIDIV2);
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT) * 0.1f);
		_float4x4 mBeam = m_pTransform->Get_Matrix();
		m_pTransform->Set_Matrix(mOriginal);
	//	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::EM800_BEAM, initializer_list<any>
	//	{mOriginal, mOriginal, mOriginal, mOriginal, mBeam, mBeam, mBeam, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal});
	//	{mOriginal, mOriginal, mBeam, mBeam, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal, mOriginal});
	}
	 

	if (CGameInstance::Get_Instance()->Key_Down('I'))
	{
		SAS_Overdrive();
	}

//#ifdef _DEBUG
	if (CGameInstance::Get_Instance()->Key_Down(VK_OEM_5))
	{
		m_bDebug = !m_bDebug;
		if (!m_bDebug)
		{
			_float3 vPos = m_pTransform->Get_State(TRANSFORM::POSITION);
			m_pController->setPosition(PxExtendedVec3(vPos.x, vPos.y, vPos.z));
		}
	}

	if (m_bDebug)
	{
		if (CGameInstance::Get_Instance()->Key_Hold('W'))
		{
			m_pTransform->Translate(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::LOOK) * fTimeDelta * (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT) ? 100.f : 10.f));
		}
		if (CGameInstance::Get_Instance()->Key_Hold('A'))
		{
			m_pTransform->Translate(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::RIGHT) * fTimeDelta * -(CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT) ? 100.f : 10.f));
		}
		if (CGameInstance::Get_Instance()->Key_Hold('S'))
		{
			m_pTransform->Translate(-CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::LOOK) * fTimeDelta * (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT) ? 100.f : 10.f));
		}
		if (CGameInstance::Get_Instance()->Key_Hold('D'))
		{
			m_pTransform->Translate(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::RIGHT) * fTimeDelta * (CGameInstance::Get_Instance()->Key_Hold(VK_LSHIFT) ? 100.f : 10.f));
		}

		return;
	}
//#endif
	if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI() &&
		!CUI_Manager::Get_Instance()->Get_IsShop())
	{
		if (CGameInstance::Get_Instance()->Key_Down('1'))
		{
			if (m_PlayerDesc.eSAS != PLAYER_SAS::ACCELERATION && m_PlayerDesc.SAS_Gauge[(_uint)PLAYER_SAS::ACCELERATION] > 20.f)
			{
				auto pUIManager = CUI_Manager::Get_Instance();
				pUIManager->Play_SAS_Cutscene(CHARACTER::ARASHI, std::bind(&CPlayer::SAS_Entry, this, PLAYER_SAS::ACCELERATION));
				m_pSkillSlot->Activate_Skill_Acceleration();
			}
			else
			{
				SAS_Exit();
			}
		}
		if (CGameInstance::Get_Instance()->Key_Down('2'))
		{
			if (m_PlayerDesc.eSAS != PLAYER_SAS::ELECTRIC && m_PlayerDesc.SAS_Gauge[(_uint)PLAYER_SAS::ELECTRIC] > 20.f)
			{
				auto pUIManager = CUI_Manager::Get_Instance();
				pUIManager->Play_SAS_Cutscene(CHARACTER::SHIDEN, std::bind(&CPlayer::SAS_Entry, this, PLAYER_SAS::ELECTRIC));
				m_pSkillSlot->Activate_Skill_Electric();
			}
			else
			{
				SAS_Exit();
			}
		}
		if (CGameInstance::Get_Instance()->Key_Down('3') && m_PlayerDesc.SAS_Gauge[(_uint)PLAYER_SAS::STEALTH] > 20.f)
		{
			if (m_PlayerDesc.eSAS != PLAYER_SAS::STEALTH)
			{
				auto pUIManager = CUI_Manager::Get_Instance();
				pUIManager->Play_SAS_Cutscene(CHARACTER::KAGERO, std::bind(&CPlayer::SAS_Entry, this, PLAYER_SAS::STEALTH));
				m_pSkillSlot->Activate_Skill_Stealth();

				auto pBrainTalk = CUI_Manager::Get_Instance()->Get_Indicator_BrainTalk_Mizuha();
				pBrainTalk->Set_BrainTalk_SpecialSituation(CUI_Indicator_BrainTalk_Mizuha::BT_KAGERO_MONSTER_LOST_US);
			}
			else
			{
				SAS_Exit();
			}
		}
		if (CGameInstance::Get_Instance()->Key_Down('4'))
		{
			if (m_PlayerDesc.eSAS != PLAYER_SAS::REPLICATION && m_PlayerDesc.SAS_Gauge[(_uint)PLAYER_SAS::REPLICATION] > 20.f)
			{
				auto pUIManager = CUI_Manager::Get_Instance();
				pUIManager->Play_SAS_Cutscene(CHARACTER::KYOKA, std::bind(&CPlayer::SAS_Entry, this, PLAYER_SAS::REPLICATION));
				m_pSkillSlot->Activate_Skill_Clone();
			}
			else
			{
				SAS_Exit();
			}
		}
	}

	if (CGameInstance::Get_Instance()->Key_Down('5'))
	{
		for (shared_ptr<CPlayerWeapon> elem : m_Weapons)
		{
			elem->GetCollider()->GetActorDesc().stAttackDesc.iDamage = 10;
		}
	}

	if (CGameInstance::Get_Instance()->Key_Down('6'))
	{
		m_PlayerDesc.bRideStair = !m_PlayerDesc.bRideStair;
	}


	if (m_ActorDesc.bHit && *m_ActorDesc.stHitDesc.AttackColliderActive && !m_PlayerDesc.bInvincibility)
	{
		m_ActorDesc.bHit = false;
		m_eForceChangeState = make_shared<CPlayerHurtState>(m_ActorDesc.stHitDesc.eHurtType);
	}

	if (m_bBrainFieldExitStart)
	{
		m_fBrainFieldExitAccTime += fTimeDelta;
		if (m_fBrainFieldExitAccTime >= m_fBrainFieldScreenEffect)
		{
			CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::COLOR_REVERSE);
		}
		if (m_fBrainFieldExitAccTime >= m_fBrainFieldExitDelay)
		{
			m_bBrainFieldExitStart = false;
			m_fBrainFieldExitAccTime = 0.f;
			m_eForceChangeState = make_shared<CPlayerBrainFieldExitState>();
		}

		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RIPPLE);
		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RIPPLE_TINY);
		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RGB_SHIFT);
	}

	_float3 posss = m_pTransform->Get_State(TRANSFORM::POSITION);

	shared_ptr<CPlayerState> pState = m_eCurState->Transition(*this);

	if (nullptr == m_eForceChangeState)
	{
		if (nullptr != pState)
		{
			m_eCurState->Exit(*this);
			m_ePreState = m_eCurState;
			m_eCurState = pState;
			m_eCurState->Entry(*this);
		}
	}
	else
	{
		m_eCurState->Exit(*this);
		m_ePreState = m_eCurState;
		m_eCurState = m_eForceChangeState;
		m_eCurState->Entry(*this);
		m_eForceChangeState = nullptr;
	}
	m_eCurState->Update(*this, fTimeDelta);

	LockOn();
	SAS_Tick(fTimeDelta);
	Drive_Tick(fTimeDelta);
	Just_Dodge_Tick(fTimeDelta);

#pragma region FootCollider
	_matrix BoneMatrix = m_pModel->Get_Bone("RightFoot")->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	_matrix PivotMatrix = m_pModel->Get_Pivot();

	_float4x4 WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();

	PxTransform p;
	p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
	_float4 vQuat = XMQuaternionRotationMatrix(WeaponMat);
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_pFootCollider->GetActor()->setGlobalPose(p);
#pragma endregion


	BoneMatrix = m_pModel->Get_Bone("Hips")->Get_CombinedTransformation();
	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	BoneMatrix.r[3] += XMVector3Normalize(BoneMatrix.r[2]) * 10.f;

	PivotMatrix = m_pModel->Get_Pivot();

	WeaponMat = XMMatrixScaling(100.f, 100.f, 100.f) *
		BoneMatrix
		* PivotMatrix
		* m_pTransform->Get_Matrix();

	p;
	p.p = PxVec3(WeaponMat._41, WeaponMat._42, WeaponMat._43);
	vQuat = XMQuaternionRotationMatrix(WeaponMat);
	p.q = PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w);
	m_pController->getActor()->setGlobalPose(p);


	if (pGameInstance->Current_Scene() == SCENE::REALTEST)
	{
		m_PlayerDesc.fKinesisGauge = 100.f;
	}

	//ImGui::Begin("Material Settings");
	//if (ImGui::CollapsingHeader("Player Material"))
	//{
	//	_uint iNumMesh = m_pModel->Get_NumMeshes();
	//	for (_uint i = 0; i < iNumMesh; ++i)
	//	{
	//		string MeshName = m_pModel->Get_MeshName(i);
	//		if (ImGui::TreeNode(MeshName.c_str()))
	//		{
	//			SHADERDESC tShaderDesc = m_pModel->Get_ShaderDesc(i);
	//			_bool bDirtyFlag = false;
	//			if (ImGui::SliderFloat4(("Diffuse##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vDiffuse.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Ambient##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vAmbient.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Specular##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vSpecular.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat4(("Emissive##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.vEmissive.x, 0.f, 1.f))
	//				bDirtyFlag = true;
	//			if (ImGui::SliderFloat(("Shininess##" + std::to_string(i)).c_str(), &tShaderDesc.tMaterialDesc.fShininess, 0.f, 1.f))
	//				bDirtyFlag = true;

	//			if (bDirtyFlag)
	//			{
	//				m_pModel->Set_ShaderDesc(i, tShaderDesc);
	//			}

	//			ImGui::TreePop();
	//		}
	//	}

	//	if (ImGui::Button("Save"))
	//	{
	//		m_pModel->Export_ShaderMaterial();
	//	}
	//}

	//ImGui::End();

	if (m_PlayerDesc.bBrainField == true)
	{
		m_fEffect_Timer += fTimeDelta;

		if (m_fEffect_Timer >= 0.5f)
		{

			shared_ptr pPlayerTransform = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM));

			_float3 Pos = pPlayerTransform->Get_State(TRANSFORM::POSITION);
			Pos.y += 2.f;

			const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Head")->Get_CombinedTransformationPointer();

			_float4x4 mat2 = XMLoadFloat4x4(mat) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTransform->Get_Matrix();

			mat2._42 += 0.3f;

			CEffect_Manager::Get_Instance()->Fetch(VFX::BRAINFIELD_BRAINITER, mat2);

			m_fEffect_Timer = 0.f;
		}
	}
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	//m_pPsychokinesis->Late_Tick(fTimeDelta);
	m_pSkillSlot->Late_Tick(fTimeDelta);

	if (m_bTransparency)
	{
		Add_RenderObject(RENDER_GROUP::BLEND);
	}
	else
	{
		Add_RenderObject(RENDER_GROUP::NONBLEND);
		Add_RenderObject(RENDER_GROUP::SHADOW);
		Add_RenderObject(RENDER_GROUP::NEON);

		if (m_bEnableOutLine)
		{
			Add_RenderObject(RENDER_GROUP::OUTLINE);
		}
	}

	if (m_bDecreaseOutLine)
	{
		if (m_vOutlineColor.x > m_vOutLineColorMinThreshold.x)
			m_vOutlineColor.x -= fTimeDelta * 0.5f;
		if (m_vOutlineColor.y > m_vOutLineColorMinThreshold.y)
			m_vOutlineColor.y -= fTimeDelta * 0.5f;
		if (m_vOutlineColor.z > m_vOutLineColorMinThreshold.z)
			m_vOutlineColor.z -= fTimeDelta * 0.5f;

		if (m_vOutlineColor.x <= m_vOutLineColorMinThreshold.x && m_vOutlineColor.y <= m_vOutLineColorMinThreshold.y
			&& m_vOutlineColor.z <= m_vOutLineColorMinThreshold.z)
		{
			m_vOutlineColor = m_vOutLineColorMinThreshold;

			m_bEnableOutLine = false;
			m_bDecreaseOutLine = false;
		}
	}

	for (auto Iter = m_lstRimEffectDesc.begin(); Iter != m_lstRimEffectDesc.end();)
	{
		auto& tRimEffectDesc = *Iter;
		tRimEffectDesc.fAccTime += fTimeDelta;

//#ifdef _DEBUG
//		if (CImGui_Manager::Get_Instance()->Is_Enable())
//		{
//			ImGui::SliderFloat3("Transparency Rim Color", &tRimEffectDesc.vRimColor.x, 0.f, 2.f);
//			ImGui::SliderFloat("Transparency Rim Strength", &tRimEffectDesc.fRimStrength, 0.f, 5.f);
//			ImGui::SliderFloat("Transperancy Rim Power", &tRimEffectDesc.fRimPower, 0.f, 15.f);
//		}
//#endif

		if (tRimEffectDesc.fAccTime >= tRimEffectDesc.fDuration) 
		{
			Iter = m_lstRimEffectDesc.erase(Iter);
		}
		else
		{
			++Iter;
		}
	}
}

HRESULT CPlayer::Render()
{
	if (m_bTransparency)
	{
		_float4 vCamPosition = *(_float4*)CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).m[3];
		if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_RawValue("g_vRimColor", &m_vRimColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_Float("g_fRimStrength", m_fRimStrength)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_Float");
		}
		if (FAILED(m_pShader->Bind_Float("g_fRimPower", m_fRimPower)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to Bind_Float");
		}

		if (FAILED(__super::Render(10)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to CGameObject::Render");
		}
	}
	else
	{
		if (FAILED(Render_Player()))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to CPlayer::Render_Player");
		}
	}

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth(shared_ptr<CLight> _pShadow)
{
	//if (FAILED(__super::Render(2)))
	//{
	//	MSG_RETURN(E_FAIL, "CPlayer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	//}

	if (FAILED(__super::Render_ShadowDepth(_pShadow)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_ShadowDepth", "Failed to CGameObject::Render_ShadowDepth");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CPlayer::Render_OutLine()
{
	if (false == CUI_Manager::Get_Instance()->Get_OnOffSwitch_RenderUI())
	{
		if (FAILED(m_pShader->Bind_RawValue("g_vOutlineColor", reinterpret_cast<const void*>(&m_vOutlineColor), sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render_OutLine", "Failed to Bind::OutlineColor");
		}

		if (FAILED(m_pShader->Bind_Float("g_fOutLineSize", m_fOutlineThin)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render_OutLine", "Failed to CGameObject::Bind_Float");
		}

		if (FAILED(__super::Render(3)))
		{
			MSG_RETURN(E_FAIL, "CTestCube::Render_OutLine", "Failed to CGameObject::Render_OutLine");
		}
	}

	return S_OK;
}

HRESULT CPlayer::Render_Neon()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_Neon", "Failed to Bind_OnShader");
	}

	if (FAILED(m_pModel->Bind_BoneMatrices(7, m_pShader, SHADER_BONE)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_Neon", "Failed to Bind_BoneMatrices");
	}

	if (FAILED(m_pModel->Bind_ShaderResourceViews(7, m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_Neon", "Failed to Bind_ShaderResourceViews");
	}

	m_pModel->Render(7, m_pShader, 9);

	if (false == m_lstRimEffectDesc.empty() && FAILED(Render_RimPlayer()))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render", "Failed to CPlayer::Render_RimPlayer");
	}

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CPlayer::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CPlayer::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CPlayer::Ready_Components", "Failed to Get_Component: Model");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	m_pCircularMask = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Mask/CircularMask.dds"));
	if (nullptr == m_pCircularMask)
	{
		MSG_RETURN(E_FAIL, "CPlayer::Ready_Components", "Failed to Create: CircularMask");
	}

	return S_OK;
}

HRESULT CPlayer::Initialize_Weapon()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER);

	for (int i = 0; i < 10; i++)
	{
		m_Weapons.push_back(nullptr);

		string boneName = "WeaponEff0";
		if (i == 9)
			boneName = "WeaponEff";
		boneName += std::to_string(i + 1);
		WEAPONDESC desc;
		desc.szBoneName = boneName;

		m_Weapons[i] = CGameInstance::Get_Instance()->Clone_GameObject<CPlayerWeapon>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_PLAYER_WEAPON, desc);
		if (FAILED(pLayer->Add(m_Weapons[i])))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to CObjectLayer::Add");
		}
	}
	return S_OK;
}

HRESULT CPlayer::Initialize_FootCollider()
{
	m_pFootCollider = make_shared<CPhysXCollider>();

	CPhysXCollider::PHYSXCOLLIDERDESC desc;
	desc.eType = CPhysXCollider::TYPE::CAPSULE;
	desc.fCapsuleHeight = 1.f;
	desc.fCapsuleRadius = 0.5f;

	ACTORDESC attackDesc;
	attackDesc.strTag = ACTORTAG::PLAYER_KICK;
	attackDesc.stAttackDesc.iDamage = 5.f;
	m_pFootCollider->Initialize(desc, attackDesc);
	return S_OK;
}


HRESULT CPlayer::Initialize_Tentacle()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Find_Layer(CGameInstance::Get_Instance()->Current_Scene(), LAYER_PLAYER);

	_float3 movePos[5] = {
		_float3(-0.2f,0.2f,0.f),
		_float3(0.2f,0.3f,0.f),
		_float3(0.f,0.f,0.f),
		_float3(-0.2f,-0.2f,0.f),
		_float3(0.2f,-0.3f,0.f)
	};

	string boneName[5] = { "LeftScarf","RightScarf","Spine1","LeftWear4","RightWear4" };
	for (int i = 0; i < 5; i++)
	{
		m_Tentacles.push_back(nullptr);

		TENTACLEDESC desc;
		desc.szBoneName = boneName[i];
		desc.index = i + 1;

		m_Tentacles[i] = CGameInstance::Get_Instance()->Clone_GameObject<CPlayerTentacle>(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_PLAYER_TENTACLE, desc);
		if (FAILED(pLayer->Add(m_Tentacles[i])))
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to CObjectLayer::Add");
		}
	}

	if (CGameInstance::Get_Instance()->Current_Scene() == SCENE::MINDROOM || CGameInstance::Get_Instance()->Current_Scene() == SCENE::REALTEST)
	{
		const _float4x4* pTentacleCombineBone = m_pModel->Get_Bone("BackHair")->Get_CombinedTransformationPointer();

		auto pGameInstance = CGameInstance::Get_Instance();
		auto pBrainFieldTentacleLayer = pGameInstance->Add_Layer(pGameInstance->Current_Scene(), LAYER_PLAYER_BRAINFIELD_TENTACLE);
		for (_uint i = 0; i < 3; ++i)
		{
			auto pBrainFieldTentacle = pGameInstance->Clone_GameObject<CBrainFieldTentacle>(pGameInstance->Current_Scene(), PROTOTYPE_GAMEOBJECT_PLAYER_BRAINFIELD_TENTACLE, pTentacleCombineBone);
			if (FAILED(pBrainFieldTentacleLayer->Add(pBrainFieldTentacle)))
			{
				MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to CObjectLayer::Add");
			}

			m_vecBrainFieldTentacles.push_back(pBrainFieldTentacle);
			pBrainFieldTentacle->Set_Enable(false);
		}
	}

	return S_OK;
}

HRESULT CPlayer::Enable_WeaponVFX(_bool _bElec)
{
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff01", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff02", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff03", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff04", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff05", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff06", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff07", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff08", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff09", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(!_bElec ? VFX::PLAYER_WEAPON_DIFFUSE : VFX::PLAYER_WEAPON_DIFFUSE_ELEC,	make_tuple(shared_from_gameobject(), "WeaponEff10", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff01", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff02", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff03", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff04", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff05", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff06", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff07", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff08", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff09", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_DISTORTION,											make_tuple(shared_from_gameobject(), "WeaponEff10", _float4x4(XMMatrixTranslation(-10.f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+10.f, 0.f, 0.f))));

	for (auto& p : m_Weapons)
	{
		if (!_bElec)
		{
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE0, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
		}
		else
		{
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE_ELEC0, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE_ELEC1, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE_ELEC2, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE_ELEC3, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE_ELEC4, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
			CEffect_Manager::Get_Instance()->Fetch(VFX::PLAYER_WEAPON_PARTICLE_ELEC5, make_pair(static_pointer_cast<CGameObject>(p), "RootNode"));
		}

		p->Apply_Effect(_bElec);
//
	//	LIGHTDESC t;
	//	t.eLightType = LIGHTTYPE::POINT;
	//	t.fRange = 2.5f;
	//	t.vDiffuse = _float4(.5f, .1f, 1.f, 1.f);
	//	t.vAmbient = _float4();
	//	t.vSpecular = _float4();
	//	t.fAttenuation0 = .1f;
	//	t.fAttenuation1 = .01f;
	//	t.fAttenuation2 = .001f;
	//	t.fLifeTime = 10.5f;
	//	t.fWeight = 1.f;

		//	auto pLight = CGameInstance::Get_Instance()->Add_Light(SCENE::TEST, t, p->Get_Component<CTransform>(COMPONENT::TRANSFORM));
		//	CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
		//		[&](_float fTimeDelta)->_bool
		//		{
		//			LIGHTDESC original = pLight->Get_LightDesc();
		//			original.fRange = std::clamp(original.fRange, 0.f, 2.5f);
		//			if (original.fRange <= 0.f)
		//			{
		//				CGameInstance::Get_Instance()->Erase_Light(pLight);
		//				return false;
		//			}
		//			pLight->Set_LightDesc(original);
		//			return true;
		//		}
		//	);
	}

	return S_OK;
}

void CPlayer::Enable_ChargeVFX()
{
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff01", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff02", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff03", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff04", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff05", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff06", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff07", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff08", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff09", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DIFFUSE,	make_tuple(shared_from_gameobject(), "WeaponEff10", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff01", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff02", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff03", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff04", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff05", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff06", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff07", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff08", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION, make_tuple(shared_from_gameobject(), "WeaponEff09", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
	CEffect_Manager::Get_Instance()->Fetch(VFX::STEALTHCHARGE_TRAIL_DISTORTION,	make_tuple(shared_from_gameobject(), "WeaponEff10", _float4x4(XMMatrixTranslation(-2.5f, 0.f, 0.f)), _float4x4(XMMatrixTranslation(+2.5f, 0.f, 0.f))));
}

void CPlayer::Set_WeaponEmissive(_float4 vColor)
{
	for (auto pWeapon : m_Weapons)
	{
		pWeapon->Set_Emissive(vColor);
	}
}

void CPlayer::SAS_Overdrive()
{
	_vector vA	= XMVectorSet(-sqrtf(3.f), 0.f, 1.f, 0.f);
	_vector vB	= XMVectorSet(+sqrtf(3.f), 0.f, 1.f, 0.f);
	_vector vC	= XMVectorSet(0.f, 0.f, -2.f, 0.f);
	_matrix mA	= XMMatrixAffineTransformation(XMVectorReplicate(1.f), vA,
				XMQuaternionRotationRollPitchYaw(XMConvertToRadians(-15.f), XMConvertToRadians(-30.f), XMConvertToRadians(-15.f)), vA);
	_matrix mB	= XMMatrixAffineTransformation(XMVectorReplicate(1.f), vB,
				XMQuaternionRotationRollPitchYaw(XMConvertToRadians(-15.f), XMConvertToRadians(+30.f), XMConvertToRadians(+15.f)), vB);
	_matrix mC	= XMMatrixAffineTransformation(XMVectorReplicate(1.f), vC,
				XMQuaternionRotationRollPitchYaw(XMConvertToRadians(30.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)), vC);
	_matrix mQ	= XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(XM_PIDIV4, 0.f, 0.f));
	mA			= XMMatrixMultiply(XMMatrixTranslation(0.f, 50.f, 0.f), XMMatrixMultiply(mA, mQ));
	mB			= XMMatrixMultiply(XMMatrixTranslation(0.f, 50.f, 0.f), XMMatrixMultiply(mB, mQ));
	mC			= XMMatrixMultiply(XMMatrixTranslation(0.f, 50.f, 0.f), XMMatrixMultiply(mC, mQ));
	_vector vQ	= XMQuaternionRotationRollPitchYaw(-XM_PIDIV2, 0.f, 0.f);
	auto argA	= make_tuple(shared_from_gameobject(), _float4x4(mA), _float4(vQ), _float3(0.f, 1.25f, 0.f));
	auto argB	= make_tuple(shared_from_gameobject(), _float4x4(mB), _float4(vQ), _float3(0.f, 1.25f, 0.f));
	auto argC	= make_tuple(shared_from_gameobject(), _float4x4(mC), _float4(vQ), _float3(0.f, 1.25f, 0.f));
	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::DRIVE, initializer_list<any>
	{argA, argA, argA, argA, argA, argA, argA, argA, argA, argA, argA, argA});
	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::DRIVE, initializer_list<any>
	{argB, argB, argB, argB, argB, argB, argB, argB, argB, argB, argB, argB});
	CEffect_Manager::Get_Instance()->Fetch(MULTIVFX::DRIVE, initializer_list<any>
	{argC, argC, argC, argC, argC, argC, argC, argC, argC, argC, argC, argC});
}

void CPlayer::Enable_BrainField(_bool bEnableBrainField)
{
	m_bEnableBrainField = bEnableBrainField;

	if (m_bEnableBrainField)
	{
		for (auto pBrainFieldTentacle : m_vecBrainFieldTentacles)
		{
			pBrainFieldTentacle->Set_Enable(true);
		}

		auto tShaderDesc = m_pModel->Get_ShaderDesc(5);
		tShaderDesc.tMaterialDesc.vDiffuse = _float4(2.f, 0.f, 0.f, 1.f);
		tShaderDesc.tMaterialDesc.vAmbient = _float4(2.f, 0.f, 0.f, 1.f);
		m_pModel->Set_ShaderDesc(5, tShaderDesc);

		m_vecMeshPassIndexPair[0] = {0, 28};
		m_vecMeshPassIndexPair[1] = {2, 28};
		m_vecMeshPassIndexPair[2] = {3, 28};
	}
	else
	{
		for (auto pBrainFieldTentacle : m_vecBrainFieldTentacles)
		{
			pBrainFieldTentacle->Set_Enable(false);
		}

		m_vecMeshPassIndexPair[0] = { 0, 17 };
		m_vecMeshPassIndexPair[1] = { 2, 17 };
		m_vecMeshPassIndexPair[2] = { 3, 17 };
	}
}

void CPlayer::Set_Gravity(_float fGravity)
{
	m_PlayerDesc.fGravity = fGravity;
}

_float CPlayer::Get_Gravity()
{
	return m_PlayerDesc.fGravity;
}

#ifdef _DEBUG
void CPlayer::Free_Move()
{
	m_bDebug = true;
}

void CPlayer::Debug_Transparency_Parameters()
{
	ImGui::SliderFloat3("Transparency Rim Color", &m_vRimColor.x, 0.f, 2.f);
	ImGui::SliderFloat("Transparency Rim Strength", &m_fRimStrength, 0.f, 5.f);
	ImGui::SliderFloat("Transperancy Rim Power", &m_fRimPower, 0.f, 15.f);
}

#endif

HRESULT CPlayer::Render_Player()
{
	if (FAILED(Bind_Transform(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to CGameObject::Bind_Transform");
	}

	if (m_bEnableBrainField || m_PlayerDesc.bDriveMode)
	{
		for (_uint i = 0; i < (_uint)m_vecDriveModeMeshPassIndexPair.size(); ++i)
		{
			_uint iMeshIndex = m_vecDriveModeMeshPassIndexPair[i].first;
			_uint iPassIndex = m_vecDriveModeMeshPassIndexPair[i].second;
			if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderResourceViews");
			}
			if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderOptions");
			}
			if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
			{
				MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_BoneMatrices");
			}

			if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
			{
				MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Render Model");
			}
		}
	}

	for (_uint i = 0; i < m_vecMeshPassIndexPair.size(); ++i)
	{
		_uint iMeshIndex = m_vecMeshPassIndexPair[i].first;
		_uint iPassIndex = m_vecMeshPassIndexPair[i].second;

		if ((m_bEnableBrainField || m_PlayerDesc.bDriveMode) && (iMeshIndex == 0 || iMeshIndex == 3))
			continue;

		if (FAILED(m_pModel->Bind_ShaderResourceViews(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderResourceViews");
		}
		if (FAILED(m_pModel->Bind_ShaderOptions(iMeshIndex, m_pShader)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_ShaderOptions");
		}
		if (FAILED(m_pModel->Bind_BoneMatrices(iMeshIndex, m_pShader, SHADER_BONE)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Bind_BoneMatrices");
		}

		if (FAILED(m_pModel->Render(iMeshIndex, m_pShader, iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CArashi_Teammate::Render", "Failed to Render Model");
		}
	}

	return S_OK;
}

HRESULT CPlayer::Render_RimPlayer()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
	{
		MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to CShader::Bind_Vector");
	}

	for (auto& tRimEffectDesc : m_lstRimEffectDesc)
	{
		_bool bTransparency = tRimEffectDesc.bTransparency;
		_float3 vRimColor = tRimEffectDesc.vRimColor;
		_float3 vScale = tRimEffectDesc.vScale;

		_float fRimStrength = tRimEffectDesc.fRimStrength;
		_float fRimPower = tRimEffectDesc.fRimPower;

		if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, XMMatrixScaling(vScale.x, vScale.y, vScale.z) * m_pTransform->Get_Matrix())))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to CShader::Bind_Matrix");
		}

		if (FAILED(m_pShader->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float3))))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to Bind_RawValue");
		}
		if (FAILED(m_pShader->Bind_Float("g_fRimStrength", fRimStrength)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to Bind_Float");
		}
		if (FAILED(m_pShader->Bind_Float("g_fRimPower", fRimPower)))
		{
			MSG_RETURN(E_FAIL, "CPlayer::Render_RimPlayer", "Failed to Bind_Float");
		}

		_uint iPassIndex = 23;
		if (bTransparency)
		{
			iPassIndex = 24;
		}
		
		for (_uint i = 0; i < m_pModel->Get_NumMeshes(); ++i)
		{
			if (m_bEnableBrainField && (i == 0))
				continue;

			if (i == 2 || i == 3)
				continue;

			if (FAILED(m_pModel->Bind_ShaderOptions(i, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CGameObject::Render_RimPlayer", "Failed to Bind_ShaderOptions");
			}
			if (FAILED(m_pModel->Bind_BoneMatrices(i, m_pShader, SHADER_BONE)))
			{
				MSG_RETURN(E_FAIL, "CModel::Render_RimPlayer", "Failed to Bind_BoneMatrices");
			}
			if (FAILED(m_pModel->Bind_ShaderResourceViews(i, m_pShader)))
			{
				MSG_RETURN(E_FAIL, "CModel::Render_RimPlayer", "Failed to Bind_ShaderResourceViews");
			}

			if (FAILED(m_pModel->Render(i, m_pShader, iPassIndex)))
			{
				MSG_RETURN(E_FAIL, "CModel::Render_RimPlayer", "Failed to Render");
			}
		}
	}

	return S_OK;
}

void CPlayer::Set_Transparency(_bool bTansparency)
{
	m_bTransparency = bTansparency;
	
	for (size_t i = 0; i < m_Tentacles.size(); ++i)
	{
		m_Tentacles[i]->Set_Transparency(bTansparency);
	}

	m_vRimColor = { 1.f, 1.f, 1.f };
	m_fRimStrength = 0.5f;
	m_fRimPower = 5.f;
}

void CPlayer::Blink_Transparency(_float fTimeDelta)
{
	m_vRimColor = { 0.6f, 1.6f, 1.8f };

	m_fRimStrength = 1.f;

	m_fRimPower -= fTimeDelta * 5.f;
	if (m_fRimPower <= m_fBlinkPowerMaxThreshold)
	{
		m_fRimPower = m_fBlinkPowerMinThreshold;
	}
}

void CPlayer::Exit_BrainField()
{
	m_bBrainFieldExitStart = true;
	m_fBrainFieldExitAccTime = 0.f;
}

void CPlayer::Add_RimEffect(const RIMEFFECTDESC& tRimEffectDesc)
{
	m_lstRimEffectDesc.emplace_back(tRimEffectDesc);
}

void CPlayer::Clear_RimEffect()
{
	m_lstRimEffectDesc.clear();
}

void CPlayer::Enable_OutLine(_bool bOutLine, _float fTimeDelta)
{
	if (bOutLine)
	{
		m_bEnableOutLine = true;
		m_bDecreaseOutLine = false;

		if (m_vOutlineColor.x < m_vOutLineColorMaxThreshold.x)
			m_vOutlineColor.x += fTimeDelta * 0.5f;
		if (m_vOutlineColor.y < m_vOutLineColorMaxThreshold.y)
			m_vOutlineColor.y += fTimeDelta * 0.5f;
		if (m_vOutlineColor.z < m_vOutLineColorMaxThreshold.z)
			m_vOutlineColor.z += fTimeDelta * 0.5f;
	}
	else
	{
		m_bDecreaseOutLine = true;
	}
}

shared_ptr<CPlayer> CPlayer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CPlayer> pInstance = make_private_shared(CPlayer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTestCube::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPlayer::Clone(any eScene)
{
	shared_ptr<CPlayer> pInstance = make_private_shared_copy(CPlayer, *this);

	if (FAILED(pInstance->Initialize(eScene)))
	{
		MSG_RETURN(nullptr, "CTestCube::Clone", "Failed to Initialize");
	}

	return pInstance;
}

void CPlayer::LockOn()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();
	shared_ptr<CObjectLayer> pMonLayer = pGameInstance->Find_Layer(pGameInstance->Current_Scene(), LAYER_MONSTER);
	//shared_ptr<CObjectPool> pMonLayer = pGameInstance->Find_Pool(pGameInstance->Current_Scene(), LAYER_MONSTER);
	_float fMinDist = 30.f;
	m_pLockedOnMonster = nullptr;

	if (pGameInstance->Key_Down('Q') && SCENE::HIDEOUT != CGameInstance::Get_Instance()->Current_Scene() && SCENE::SUOH != CGameInstance::Get_Instance()->Current_Scene())
	{
		if (!m_pCamera->Get_LockOn())
		{
			pMonLayer->Iterate_Objects(
				[&](shared_ptr<CGameObject> _pObject)->bool
				{
					shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(_pObject);

					if (nullptr == pMonster)
						return true;

					_float3 vPlayerPos = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
					_float3 vMonsterPos = static_pointer_cast<CTransform>(pMonster->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
					_float fCurDist = XMVectorGetX(XMVector3Length(vPlayerPos - vMonsterPos));
					if (fCurDist < fMinDist)
					{
						fMinDist = fCurDist;
						m_pLockedOnMonster = pMonster;
					}

					return true;
				});
			if (nullptr != m_pLockedOnMonster && m_pLockedOnMonster->Get_LockOnAble())
			{
				m_pCamera->Set_LockOn(true);
				m_pCamera->Set_LockOnTransform(static_pointer_cast<CTransform>(m_pLockedOnMonster->Get_Component(COMPONENT::TRANSFORM)));
				m_pCamera->Set_LockedMonster(m_pLockedOnMonster);

				static_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Player())->Set_FinishMonster(m_pLockedOnMonster);
			}
			else
			{
				m_pCamera->Set_LockOn(false);
				m_pCamera->Set_LockOnTransform(nullptr);
				m_pCamera->Set_LockedMonster(nullptr);
			}
		}
		else
		{
			m_pCamera->Set_LockOn(false);
			m_pCamera->Set_LockOnTransform(nullptr);
			m_pCamera->Set_LockedMonster(nullptr);
		}
	}

	if (nullptr != m_pLockedOnMonster && !m_pLockedOnMonster->Get_LockOnAble())
	{
		m_pCamera->Set_LockOn(false);
		m_pCamera->Set_LockOnTransform(nullptr);
		m_pCamera->Set_LockedMonster(nullptr);
	}
}

void CPlayer::SAS_Entry(PLAYER_SAS _sas)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	SAS_Exit();

	m_PlayerDesc.fSasTimeAcc[(_uint)_sas] = 0.f;
	m_PlayerDesc.bCallSym[(_uint)_sas] = false;
	m_PlayerDesc.bSAS_Enable = true;
	m_PlayerDesc.eSAS = _sas;

	string boneName[5] = { "LeftScarf","RightScarf","Spine1","LeftWear4","RightWear4" };
	for (_int i = 0; i < 5; ++i)
	{
	//	CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_COG, make_pair(shared_from_gameobject(), boneName[i].c_str()));
		CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_COG, make_tuple(shared_from_gameobject(), boneName[i].c_str(),
			_float4(XMQuaternionRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), -XM_PIDIV2))));
	}
	for (shared_ptr<CPlayerTentacle> elem : m_Tentacles)
	{
		elem->Set_Fly(true);
		elem->Set_Render(true);
		CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_PARTICLE, make_pair(static_pointer_cast<CGameObject>(elem), "cable_02_a"));
	}

	_float4x4	mOriginal = m_pTransform->Get_Matrix();
	m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) + _float3(0.f, 1.f, 0.f));

	_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
	switch (m_PlayerDesc.eSAS)
	{
	case PLAYER_SAS::ACCELERATION:
		CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_ACCELERATION, m_pTransform->Get_Matrix());
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 0.3f);
		CEffect_Manager::Get_Instance()->Fetch(VFX::AFTERIMAGE, Function::Find_Player());
		CGameInstance::Get_Instance()->Set_LUTIndex(2);
		CGameInstance::Get_Instance()->MaskingLUT(true, m_pCircularMask);
		//CGameInstance::Get_Instance()->PlaySoundW(TEXT("Voice_Arasi_Damaged2.wav"), 5.f);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Use_Acceleration.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Arasi_Use_SAS0.wav"),TEXT("Voice_Arasi_Use_SAS1.wav") }), g_fPlayerSound);
		CScreenEffectManager::Get_Instance()->Execute_ScreenEffect(SCREEN_EFFECT::RIPPLE_TINY);
		break;
	case PLAYER_SAS::STEALTH:
		CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_STEALTH, m_pTransform->Get_Matrix());
		Set_Transparency(true);
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Use_Stealth.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kagero_Use_SAS0.wav"),TEXT("Voice_Kagero_Use_SAS1.wav") }), g_fPlayerSound);
		break;
	case PLAYER_SAS::ELECTRIC:
		CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_ELECTRIC, m_pTransform->Get_Matrix());
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Use_Electric.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Siden_Use_SAS0.wav"),TEXT("Voice_Siden_Use_SAS1.wav") }), g_fPlayerSound);
		break;
	case PLAYER_SAS::REPLICATION:
		CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_REPLICATION, m_pTransform->Get_Matrix());
		CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_Use_Replication.wav"), g_fPlayerSound);
		CGameInstance::Get_Instance()->PlaySoundW(Function::Random({ TEXT("Voice_Kyoka_Use_SAS0.wav"),TEXT("Voice_Kyoka_Use_SAS1.wav") }), g_fPlayerSound);
		break;
	}

	m_pTransform->Set_Matrix(mOriginal);

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_SAS_Casting.wav"), g_fPlayerSound);
}

void CPlayer::SAS_Tick(_float fTimeDelta)
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (m_PlayerDesc.bSAS_Enable)
	{
		m_PlayerDesc.fSasTimeAcc[(_uint)m_PlayerDesc.eSAS] += fTimeDelta;
		m_PlayerDesc.SAS_Gauge[(_uint)m_PlayerDesc.eSAS] -= fTimeDelta * m_PlayerDesc.SAS_GaugeDecreaseTime;

		if (!m_PlayerDesc.bCallSym[(_uint)m_PlayerDesc.eSAS]
		&&	m_PlayerDesc.fSasTimeAcc[(_uint)m_PlayerDesc.eSAS] > 0.2f)
		{
			_float4x4 mOriginal = m_pTransform->Get_Matrix();
			m_pTransform->Translate(-m_pTransform->Get_State(TRANSFORM::LOOK) * 0.5f + _float3(0.f, 1.f, 0.f));
			CEffect_Manager::Get_Instance()->Fetch(VFX::SAS_SPLASH, m_pTransform->Get_Matrix());
			m_pTransform->Set_Matrix(mOriginal);
			m_PlayerDesc.bCallSym[(_uint)m_PlayerDesc.eSAS] = true;
		}
		if (m_PlayerDesc.SAS_Gauge[(_uint)m_PlayerDesc.eSAS] < 0.f)
		{
			m_PlayerDesc.SAS_Gauge[(_uint)m_PlayerDesc.eSAS] = 0.f;
			m_PlayerDesc.bSAS_Enable = false;
			SAS_Exit();
		}

		for (int i = 0; i < 4; i++)
		{
			if (i != (_uint)m_PlayerDesc.eSAS)
			{
				m_PlayerDesc.SAS_Gauge[i] += fTimeDelta * m_PlayerDesc.SAS_GaugeIncreaseTime;
				m_PlayerDesc.SAS_Gauge[i] = min(m_PlayerDesc.SAS_Gauge[i], 100.f);
			}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			m_PlayerDesc.SAS_Gauge[i] += fTimeDelta * m_PlayerDesc.SAS_GaugeIncreaseTime;
			m_PlayerDesc.SAS_Gauge[i] = min(m_PlayerDesc.SAS_Gauge[i], 100.f);
		}
	}
}

void CPlayer::SAS_Exit()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	m_PlayerDesc.bSAS_Enable = false;

	for (shared_ptr<CPlayerTentacle> elem : m_Tentacles)
	{
		elem->Reset();
	}

	_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
	switch (m_PlayerDesc.eSAS)
	{
	case PLAYER_SAS::ACCELERATION:
		pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale);
		CGameInstance::Get_Instance()->MaskingLUT(false);
		CGameInstance::Get_Instance()->Set_LUTIndex(4);
		m_pSkillSlot->SkillStop_Acceleration();

		CScreenEffectManager::Get_Instance()->Stop_ScreenEffect(SCREEN_EFFECT::RIPPLE_TINY);
		break;
	case PLAYER_SAS::STEALTH:
		Set_Transparency(false);
		m_pSkillSlot->SkillStop_Stealth();
		break;
	case PLAYER_SAS::ELECTRIC:
		m_pSkillSlot->SkillStop_Electic();
		break;
	case PLAYER_SAS::REPLICATION:
		m_pSkillSlot->SkillStop_Replication();
		break;
	}

	m_PlayerDesc.eSAS = PLAYER_SAS::NONE;

	CGameInstance::Get_Instance()->PlaySoundW(TEXT("SFX_Kasane_SAS_Release.wav"), g_fPlayerSound);
}

void CPlayer::Drive_Entry()
{
	m_PlayerDesc.bDriveMode = true;
	m_pModel->Hide_Mesh(4, false);
	m_pModel->Hide_Mesh(5, false);
	m_pModel->Hide_Mesh(7, false);
	m_PlayerDesc.fAnimSpeed = 3.f;
	m_PlayerDesc.fSpeed = 8.f;
}

void CPlayer::Drive_Tick(_float fTimeDelta)
{
	if (m_PlayerDesc.bDriveMode)
	{
		m_PlayerDesc.fDriveGauge -= m_PlayerDesc.fDriveGaugeDecreaseTime * fTimeDelta;

		if (m_PlayerDesc.fDriveGauge <= 0.f)
		{
			m_PlayerDesc.fDriveGauge = 0.f;
			Drive_Exit();
		}
	}
	else
	{
		m_PlayerDesc.fDriveGauge = 100.f;
	}
}

void CPlayer::Drive_Exit()
{
	m_PlayerDesc.bDriveMode = false;
	m_pModel->Hide_Mesh(4);
	m_pModel->Hide_Mesh(5);
	m_pModel->Hide_Mesh(7);
	m_PlayerDesc.fAnimSpeed = 2.5f;
	m_PlayerDesc.fSpeed = 7.f;
}

void CPlayer::Just_Dodge_Tick(_float fTimeDelta)
{
	auto pGameInstance = CGameInstance::Get_Instance();

	if (m_PlayerDesc.bJustDodge)
	{
		m_PlayerDesc.fJustDodgeTime -= fTimeDelta;


		shared_ptr<CTransform> pTransform = static_pointer_cast<CTransform>(m_pCamera->Get_Component(COMPONENT::TRANSFORM));
		_float3 vDir = pTransform->Get_State(TRANSFORM::LOOK);
		pTransform->Translate(XMVector3Normalize(vDir) * fTimeDelta * 15.f);

		if (m_PlayerDesc.fJustDodgeTime < 0.f)
		{
			m_PlayerDesc.fJustDodgeTime = m_PlayerDesc.fJustDodgeMaxTime;
			_float fDefaultTimeScale = pGameInstance->Get_TimeScale(DEFAULT_FPS);
			pGameInstance->Set_PhysXTimeScale(fDefaultTimeScale * 1.f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MONSTER, fDefaultTimeScale * 1.f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_PLAYER, fDefaultTimeScale * 1.f);
			pGameInstance->Set_TimeScale(pGameInstance->Current_Scene(), LAYER_MAP_DYNAMIC, fDefaultTimeScale * 1.f);
			pGameInstance->Set_Effect_TimeScale(fDefaultTimeScale * 1.f);
			m_PlayerDesc.bJustDodge = false;
			m_pCamera->Set_LerpTime(0.075f);
		}
	}
}

void CPlayer::Free()
{
	//if (nullptr != m_eCurState)
	//{
	//	delete m_eCurState;
	//	m_eCurState = nullptr;
	//}
}

void CPlayer::Set_WeaponsActive(_bool b)
{
	for (int i = 0; i < m_Weapons.size(); i++)
	{
		static_pointer_cast<CPlayerWeapon>(m_Weapons[i])->GetCollider()->GetActorDesc().bActive = b;
	}
}

void CPlayer::Set_BigWeaponsActive(_bool b)
{
	for (int i = 0; i < m_Weapons.size(); i++)
	{
		static_pointer_cast<CPlayerWeapon>(m_Weapons[i])->GetBigCollider()->GetActorDesc().bActive = b;
	}
}

bool PlayerCCTFilterCallBack::filter(const PxController& a, const PxController& b)
{
	//ACTORDESC* pDesc1 = (ACTORDESC*)a.getActor()->userData;
	//ACTORDESC* pDesc2 = (ACTORDESC*)b.getActor()->userData;
	//if (nullptr != pDesc1)
	//{
	//	if (pDesc1->strTag == ACTORTAG::PLAYER && pDesc2->strTag == ACTORTAG::MONSTER)
	//	{

	//	}
	//}
	return true;
}
