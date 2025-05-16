#include "ClientPCH.h"
#include "Psychokinesis.h"

#include "GameInstance.h"
#include "InteractiveObject.h"

#include "UI_Manager.h"
#include "UI_Indicator_ObjectAttack.h"
#include "UI_Indicator_SpcObjectAttack.h"

#include "Map_Dynamic.h"
#include "Map_Tutorial_Dynamic.h"

#include "Player.h"
#include "PlayerState.h"
#include "PlayerThrowState.h"
#include "Map_Special_Dynamic.h"
#include "Map_Stage1_SpecialDynamic.h"
#include "Scene.h"
#include "Scene_Mizuha.h"
#include "Effect_Manager.h"
#include "Bone.h"

CPsychokinesis::CPsychokinesis(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CPsychokinesis::CPsychokinesis(const CPsychokinesis& _rhs)
	: CGameObject(_rhs)
{
	m_vecKeyframeAnimation = _rhs.m_vecKeyframeAnimation;
}

HRESULT CPsychokinesis::Initialize_Prototype()
{
	//Initialize_Keyframe(TEXT("Bin/Resources/Animation/Psychokinesis/Psychokinesis.mdl"));
	//Initialize_Keyframe(TEXT("Bin/Resources/Animation/Psychokinesis/kinesis.mdl"));
	Initialize_Keyframe(TEXT("Bin/Resources/Model/Player/test5pivot.mdl"));

	return S_OK;
}

HRESULT CPsychokinesis::Initialize(any pPlayerTransform)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pPlayerTransform = any_cast<shared_ptr<CTransform>>(pPlayerTransform); 

	return S_OK;
}

void CPsychokinesis::Tick(_float _fTimeDelta)
{

	__super::Tick(_fTimeDelta);

	auto pGameInstance = CGameInstance::Get_Instance();

	//if (pGameInstance->Key_Down('5'))
	//	m_iFixedIndex--;
	//if (pGameInstance->Key_Down('6'))
	//	m_iFixedIndex++;
	//cout << m_bCapture << endl;
	//if (m_pCapturedObject)
	//	cout << "NOTNULL" << endl;
	//else
	//	cout << "NULL" << endl;
	//cout << m_iFixedIndex << endl;
	//19:naryeojjikgi
#if ACTIVATE_IMGUI
	if (ImGui::BeginMainMenuBar())
	{
	if (ImGui::BeginMenu("Psychokinesis"))
	{
	ImGui::Text("Charging Time: %f", m_fChargingTime);

	ImGui::Text("AnimationIndex: %d", m_iCurrentAnimationIndex);

	ImGui::Text("TrackPosition: %f", m_fTrackPosition[0]);
	ImGui::Text("KeyframeIndex: %d", m_iCurrentKeyframeIndex[0]);

	ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	}
#endif

	//auto pGameInstance = CGameInstance::Get_Instance();
	auto pFocusedObject = pGameInstance->Get_FocusedObject();
	auto pFocusedSpecialObject = pGameInstance->Get_FocusedSpecialObject();

	if (nullptr != pFocusedObject)
	{
		CUI_Manager::Get_Instance()->Set_RenderJW(false);
		auto pUI_Manager = CUI_Manager::Get_Instance();
		auto pIndicator = pUI_Manager->Get_Indicator();
		pIndicator->Ready_PsycoAttack(pFocusedObject, _fTimeDelta);
	}
	else
		CUI_Manager::Get_Instance()->Set_RenderJW(true);

	if (false == m_bCapture && false == m_bThrowing[0])
	{
		auto pUI_Manager = CUI_Manager::Get_Instance();
		auto pIndicator = pUI_Manager->Get_Indicator();
		auto pIndicatorSpc = pUI_Manager->Get_IndicatorSpc();

		if (nullptr != pFocusedSpecialObject)
		{
			pIndicatorSpc->Ready_PsycoAttack(pFocusedSpecialObject, _fTimeDelta);
		}
		else
		{
			pIndicatorSpc->Set_RenderOff();
		}
	}

	Capture(m_iFixedIndex);

	//if (pGameInstance->Key_Up(VK_RBUTTON)/* || pGameInstance->Key_Up('G')*/) // Throw Failed
	//{
	//	m_bCapture = false;
	//}

	if (!pGameInstance->Key_Hold(VK_RBUTTON))
	{
		auto pIndicator = CUI_Manager::Get_Instance()->Get_Indicator();

		pIndicator->Set_Indicator_DefaultState(_fTimeDelta);
	}

	if (m_bCapture && false == m_bThrowing[0])
	{
		if (nullptr == m_pCapturedObject)
		{
			if (nullptr != pFocusedObject && (pGameInstance->Key_Hold(VK_RBUTTON) || (pGameInstance->Key_Hold(VK_LBUTTON) && m_pPlayer->Get_PlayerDesc().bBrainField)))
			{
				//cout << "CAPTUREDOBJECT" << endl;
				m_pCapturedObject = pFocusedObject;
			}
			if (nullptr != pFocusedSpecialObject && pGameInstance->Key_Hold('G'))
				m_pCapturedObject = pFocusedSpecialObject;
		}
		if (m_pCapturedObject.get())
		{
			/*if (static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_ActorDesc().bThrow)
			{
				m_pCapturedObject = nullptr;
			}
			else*/
			{
				static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_ActorDesc().bCapture = true;
			}
		}

		//if (static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_PlayerState()->Get_CurrentState() != PLAYER_STATE::THROW)
		//{
		//	//if(m_pCapturedObject)
		//	//{
		//	//	static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_ActorDesc().bCapture = false;
		//	//	m_pCapturedObject = nullptr;
		//	//}
		//	return;
		//}


		if (dynamic_pointer_cast<CMap_Special_Dynamic>(m_pCapturedObject) &&static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_PlayerState()->Get_CurrentState() != PLAYER_STATE::SPECIALTHROW)
		{

			return;
		}

		if (m_pCapturedObject && pGameInstance->Key_Hold('G'))
		{
			if (nullptr != dynamic_pointer_cast<CMap_Special_Dynamic>(m_pCapturedObject))
			{
				m_pSpecialThrowObject = static_pointer_cast<CMap_Special_Dynamic>(m_pCapturedObject);
				static_pointer_cast<CMap_Special_Dynamic>(m_pCapturedObject)->Set_ThrowStart(true);
			}
		}

		if ((m_pCapturedObject && pGameInstance->Key_Hold(VK_RBUTTON)) || (m_pCapturedObject && pGameInstance->Key_Hold('G')))
		{
			m_EffectTimer += _fTimeDelta;

			if (m_EffectTimer >= 1.f)
			{

				shared_ptr pPlayerTransform = static_pointer_cast<CTransform>(CGameInstance::Get_Instance()->Get_Player()->Get_Component(COMPONENT::TRANSFORM));

				_float3 Pos = pPlayerTransform->Get_State(TRANSFORM::POSITION);
				Pos.y += 2.f;

				const _float4x4* mat = CGameInstance::Get_Instance()->Get_Player()->Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone("Head")->Get_CombinedTransformationPointer();

				_float4x4 mat2 = XMLoadFloat4x4(mat) * XMMatrixScaling(0.012f, 0.012f, 0.012f) * XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTransform->Get_Matrix();

				mat2._42 += 0.25f;

				CEffect_Manager::Get_Instance()->Fetch(VFX::PSY_BRAINITER, mat2);

				m_EffectTimer = 0.f;
			}
			if (nullptr != dynamic_pointer_cast<CMap_Special_Dynamic>(m_pCapturedObject))
			{
				m_bSpecialThrow = true;
			}
			else
			{
				m_bSpecialThrow = false;
			}
		}

		if (m_bSpecialThrow)
			return;

		if (static_cast<CPlayer*>(pGameInstance->Get_Player())->Get_PlayerState()->Get_CurrentState() != PLAYER_STATE::THROW &&
			!m_pPlayer->Get_PlayerDesc().bBrainField)
		{
			return;
		}

	
		if (m_pCapturedObject && pGameInstance->Key_Hold(VK_RBUTTON))
			//if (m_pCapturedObject && m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::THROW)
		{
			m_pCapturedObject->Interactive_PhychoKinesis(true);
			m_fChargingTime += _fTimeDelta * 1.f;


			if (m_bStrong)
			{
				const _float fObjectWeight = m_pCapturedObject->Get_Weight();
				//if (m_fChargingTime >= fObjectWeight) // Throw Ready Finished   1frame~~~ once
				//{
				//	m_bCapture = false;
				//	m_bThrowing[0] = true;
				//	static_pointer_cast<CPlayerThrowState>(m_pPlayer->Get_PlayerState())->SetThrow(true);
				//	m_fChargingTime = 0.f;
				//}
				//else // Throw Ready...(Loop)  object goto anim 1frame
				//{
				static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->SetPhysX(false);
				const pair<ANIMATIONDESC, vector<KEYFRAME>>& AnimKeyframePair = m_vecKeyframeAnimation[m_iCurrentAnimationIndex];
				const KEYFRAME& tKeyframe = AnimKeyframePair.second[0];
				if (Interpolate_Keyframe(tKeyframe, dynamic_pointer_cast<CTransform>(m_pCapturedObject->Get_Component(COMPONENT::TRANSFORM)),
					m_fChargingTime / fObjectWeight, 0.1f))
				{
					m_bThrowing[0] = true;
					m_bCapture = false;
					m_fChargingTime = 0.f;
					m_iFixedIndex = 7;
					if (m_pPlayer->Get_PlayerState()->Get_CurrentSAS() == PLAYER_SAS::REPLICATION)
					{
						static_pointer_cast<CMap_Dynamic>(static_pointer_cast<CMap_Dynamic>(m_pCapturedObject))->Set_Strong(true);
						static_pointer_cast<CMap_Dynamic>(static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Duplicate());
						static_pointer_cast<CMap_Dynamic>(static_pointer_cast<CMap_Dynamic>(m_pCapturedObject))->Set_DuplicatedObjectPos();
					}
					auto pIndicator = CUI_Manager::Get_Instance()->Get_Indicator();

					if (nullptr != m_pCapturedObject)
						pIndicator->Ready_PsycoAttack(m_pCapturedObject, _fTimeDelta);
				}

			}
			else
			{
				dynamic_pointer_cast<CTransform>(m_pCapturedObject->Get_Component(COMPONENT::TRANSFORM))->Translate({ 0,_fTimeDelta * 1.5f,0,0 });
				static_pointer_cast<CMap_Dynamic>(static_pointer_cast<CMap_Dynamic>(m_pCapturedObject))->Set_Strong(false);
				static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->SetPhysX(false);
				if (m_pCapturedObject && m_fChargingTime > 1.f)
				{
					static_pointer_cast<CPlayerThrowState>(m_pPlayer->Get_PlayerState())->SetThrow(true);
				}
				if (m_pCapturedObject && m_fChargingTime > 1.5f)
				{
					static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Throw();
					if (m_pPlayer->Get_PlayerState()->Get_CurrentSAS() == PLAYER_SAS::REPLICATION)
					{
						static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Set_Strong(false);
						static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Duplicate();
						static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Set_DuplicatedObjectPos();
					}
					m_bThrowing[0] = false;
					m_bThrowing[1] = false;
					m_bCapture = false;
					m_fChargingTime = 0.f;
					m_fTrackPosition[0] = 0.f;
					m_fTrackPosition[1] = 0.f;
					m_pCapturedObject = nullptr;
				}

				if (0.f <= m_fChargingTime && 1.5f > m_fChargingTime)
				{
					auto pIndicator = CUI_Manager::Get_Instance()->Get_Indicator();

					pIndicator->Set_Ratio(m_fChargingTime);
				}


				auto pUI_Manager = CUI_Manager::Get_Instance();
				auto pIndicator = pUI_Manager->Get_Indicator();

				if (nullptr != m_pCapturedObject)
					pIndicator->Ready_PsycoAttack(m_pCapturedObject, _fTimeDelta);
			}
		}
	

		if (m_pCapturedObject && m_pPlayer->Get_PlayerDesc().bBrainField && (m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A1 ||
			m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A2 || m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A3))
		{
			m_fChargingTime += _fTimeDelta;
			//static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->SetPhysX(false);
			const pair<ANIMATIONDESC, vector<KEYFRAME>>& AnimKeyframePair = m_vecKeyframeAnimation[m_iCurrentAnimationIndex];
			const KEYFRAME& tKeyframe = AnimKeyframePair.second[0];
			if (Interpolate_Keyframe(tKeyframe, dynamic_pointer_cast<CTransform>(m_pCapturedObject->Get_Component(COMPONENT::TRANSFORM)),
				min(1.f,m_fChargingTime), 0.1f))
			{
				if (m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A1 ||
					m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A2 ||
					m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A3)
				{
					m_iFixedIndex = 18;
					m_bThrowing[0] = true;
					m_bCapture = false;
					m_fChargingTime = 0.f;
					cout << "BFINTEREND" << endl;
				}
			}
		}
	}


	if (m_bStrong)
	{
		m_fStrongTime -= _fTimeDelta;
		if (m_fStrongTime < 0.f)
		{
			m_bStrong = false;
			m_fStrongTime = 1.f;
		}
	}
	if (m_bThrowHit)
	{
		m_fThrowHitTime -= _fTimeDelta;
		if (m_fThrowHitTime < 0.f)
		{
			m_bThrowHit = false;
			m_fThrowHitTime = 1.5f;
			m_iCombo = 0;
		}
	}


	if (m_bThrowing[0]) // 1frame~ ecd
	{
		const pair<ANIMATIONDESC, vector<KEYFRAME>>& AnimKeyframePair = m_vecKeyframeAnimation[m_iCurrentAnimationIndex];
		const ANIMATIONDESC& tAnimDesc = AnimKeyframePair.first;
		const vector<KEYFRAME>& vecKeyframe = AnimKeyframePair.second;

		_float fSpeed;
		if (m_pPlayer->Get_PlayerDesc().bBrainField)
			fSpeed = 4.f;
		else
			fSpeed = 2.f;
		m_fTrackPosition[0] += tAnimDesc.fTicksPerSecond * _fTimeDelta * fSpeed;

		if (m_fTrackPosition[0] > 40.f && !m_pPlayer->Get_PlayerDesc().bBrainField)
			static_pointer_cast<CPlayerThrowState>(m_pPlayer->Get_PlayerState())->SetThrow(true);

		_vector			vScale;
		_vector			vRotation;
		_vector			vPosition;

		if (m_fTrackPosition[0] >= vecKeyframe.back().fTime)
		{
			vScale = XMLoadFloat4(&vecKeyframe.back().vScale);
			vRotation = XMLoadFloat4(&vecKeyframe.back().vRotation);
			vPosition = XMLoadFloat4(&vecKeyframe.back().vTranslation);
		}
		else
		{
			while (m_fTrackPosition[0] > vecKeyframe[m_iCurrentKeyframeIndex[0] + 1].fTime)
				++m_iCurrentKeyframeIndex[0];

			_float		fRatio = (m_fTrackPosition[0] - vecKeyframe[m_iCurrentKeyframeIndex[0]].fTime) /
				(vecKeyframe[m_iCurrentKeyframeIndex[0] + 1].fTime - vecKeyframe[m_iCurrentKeyframeIndex[0]].fTime);

			_vector		vSourScale, vDestScale;
			_vector		vSourRotation, vDestRotation;
			_vector		vSourTranslation, vDestTranslation;

			vSourScale = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[0]].vScale);
			vSourRotation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[0]].vRotation);
			vSourTranslation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[0]].vTranslation);

			vDestScale = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[0] + 1].vScale);
			vDestRotation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[0] + 1].vRotation);
			vDestTranslation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[0] + 1].vTranslation);

			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);
		}

		_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		auto pTransform = dynamic_pointer_cast<CTransform>(m_pCapturedObject->Get_Component(COMPONENT::TRANSFORM));
		
		_float3 vOriginScale = std::get<2>(static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_CopyTuplePivot());
		_matrix mPlayerMatrix = m_pPlayerTransform->Get_Matrix();
		if(m_pPlayer->Get_PlayerDesc().bBrainField && m_pPlayer->Get_PlayerState()->Get_CurrentState() != PLAYER_STATE::SWING_A3)
			m_pPlayerTransform->Translate(m_pPlayerTransform->Get_State(TRANSFORM::LOOK)*3.f);
		else if(m_pPlayer->Get_PlayerDesc().bBrainField && m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A3)
			m_pPlayerTransform->Translate(m_pPlayerTransform->Get_State(TRANSFORM::UP) * -3.f);
		_vector vPlayerLook = mPlayerMatrix.r[2];
		_matrix mPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_ThrowHeight(), 0.f);
		pTransform->Set_Matrix(TransformationMatrix * mPivotMatrix * m_pPlayerTransform->Get_Matrix());
		pTransform->Set_Scale(vOriginScale);
		m_pPlayerTransform->Set_Matrix(mPlayerMatrix);
	}

	if (m_bThrowing[1]) // 1frame~ ecd
	{
		const pair<ANIMATIONDESC, vector<KEYFRAME>>& AnimKeyframePair = m_vecKeyframeAnimation[m_iCurrentAnimationIndex];
		const ANIMATIONDESC& tAnimDesc = AnimKeyframePair.first;
		const vector<KEYFRAME>& vecKeyframe = AnimKeyframePair.second;

		m_fTrackPosition[1] += tAnimDesc.fTicksPerSecond * _fTimeDelta * 2.f;

		if (m_fTrackPosition[1] > 40.f)
			static_pointer_cast<CPlayerThrowState>(m_pPlayer->Get_PlayerState())->SetThrow(true);

		_vector			vScale;
		_vector			vRotation;
		_vector			vPosition;
		if (m_fTrackPosition[1] >= vecKeyframe.back().fTime)
		{
			vScale = XMLoadFloat4(&vecKeyframe.back().vScale);
			vRotation = XMLoadFloat4(&vecKeyframe.back().vRotation);
			vPosition = XMLoadFloat4(&vecKeyframe.back().vTranslation);
		}
		else
		{
			while (m_fTrackPosition[1] > vecKeyframe[m_iCurrentKeyframeIndex[1] + 1].fTime)
				++m_iCurrentKeyframeIndex[1];
			_float		fRatio = (m_fTrackPosition[1] - vecKeyframe[m_iCurrentKeyframeIndex[1]].fTime) /
				(vecKeyframe[m_iCurrentKeyframeIndex[1] + 1].fTime - vecKeyframe[m_iCurrentKeyframeIndex[1]].fTime);
			_vector		vSourScale, vDestScale;
			_vector		vSourRotation, vDestRotation;
			_vector		vSourTranslation, vDestTranslation;
			vSourScale = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[1]].vScale);
			vSourRotation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[1]].vRotation);
			vSourTranslation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[1]].vTranslation);
			vDestScale = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[1] + 1].vScale);
			vDestRotation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[1] + 1].vRotation);
			vDestTranslation = XMLoadFloat4(&vecKeyframe[m_iCurrentKeyframeIndex[1] + 1].vTranslation);
			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);
		}
		_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
		auto pTransform = dynamic_pointer_cast<CTransform>(m_pDuplicatedObject->Get_Component(COMPONENT::TRANSFORM));
		_matrix mPlayerMatrix = m_pPlayerTransform->Get_Matrix();
		_vector vPlayerLook = mPlayerMatrix.r[2];
		_matrix mPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, static_pointer_cast<CMap_Dynamic>(m_pDuplicatedObject)->Get_ThrowHeight(), 0.f);
		//mPivotMatrix.r[3] += (vPlayerLook * 5.f);
		_float3 vOriginScale = pTransform->Get_Scale();
		pTransform->Set_Matrix(TransformationMatrix * mPivotMatrix * mPlayerMatrix);
		pTransform->Set_Scale(vOriginScale);
	}


	if (m_pCapturedObject)
	{
		m_pCapturedObject->Add_RenderObject_Outline();
	}

	if(!m_pPlayer->Get_PlayerDesc().bBrainField)
	{
		if (m_fTrackPosition[0] > 70.f)
		{
			static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Throw();
			m_bThrowing[0] = false;
			m_fTrackPosition[0] = 0.f;
			m_iCurrentKeyframeIndex[0] = 0;
			m_bCapture = false;
			m_fChargingTime = 0.f;
			m_pCapturedObject = nullptr;
		}

		if (m_fTrackPosition[1] > 70.f)
		{
			m_pDuplicatedObject->Throw();
			m_bThrowing[1] = false;
			m_fTrackPosition[1] = 0.f;
			m_iCurrentKeyframeIndex[1] = 0;
			m_bCapture = false;
			m_fChargingTime = 0.f;
		}
	}
	//else
	//{
	//	if (pGameInstance->Key_Up(VK_LBUTTON)/* || pGameInstance->Key_Up('G')*/) // Throw Failed
	//	{
	//		m_bThrowing[0] = false;
	//		//m_bThrowing[1] = false;
	//		m_bCapture = false;
	//		m_fChargingTime = 0.f;
	//		m_fTrackPosition[0] = 0.f;
	//		//m_fTrackPosition[1] = 0.f;
	//		m_iCurrentKeyframeIndex[0] = 0;
	//		//m_iCurrentKeyframeIndex[1] = 0;
	//		m_iCurrentAnimationIndex = m_iFixedIndex;
	//		if (m_pCapturedObject)
	//		{
	//			Reset();
	//		}
	//	}
	//}

	//cout << m_bThrowing[1] << endl;

	if (pGameInstance->Key_Up(VK_RBUTTON)/* || pGameInstance->Key_Up('G')*/) // Throw Failed
	{
		m_bThrowing[0] = false;
		//m_bThrowing[1] = false;
		m_bCapture = false;
		m_fChargingTime = 0.f;
		m_fTrackPosition[0] = 0.f;
		//m_fTrackPosition[1] = 0.f;
		m_iCurrentKeyframeIndex[0] = 0;
		//m_iCurrentKeyframeIndex[1] = 0;
		m_iCurrentAnimationIndex = m_iFixedIndex;
		if (m_pCapturedObject)
		{
			Reset();
		}
	}
}

void CPsychokinesis::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CPsychokinesis::Render()
{
	return S_OK;
}

void CPsychokinesis::Capture(_uint iAnimIndex)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	if (nullptr != pGameInstance->Get_FocusedSpecialObject())
	{
		if (CGameInstance::Get_Instance()->Key_Hold('G'))
		{
			m_bCapture = true;
			m_iCurrentAnimationIndex = m_iFixedIndex;

			auto pIndicatorSpc = CUI_Manager::Get_Instance()->Get_IndicatorSpc();
			pIndicatorSpc->Set_RenderOff();
		}
	}
	else
	{    
		if (CGameInstance::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			m_bCapture = true;
			m_iCurrentAnimationIndex = m_iFixedIndex;
		}
		if (CGameInstance::Get_Instance()->Key_Down(VK_LBUTTON) && m_pPlayer->Get_PlayerDesc().bBrainField)
		{
			m_bCapture = true;
			if(m_pCapturedObject)
				static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_ActorDesc().bCapture = true;
		}
	}
}

HRESULT CPsychokinesis::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTestSphere::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

HRESULT CPsychokinesis::Initialize_Keyframe(const wstring& _wstrModelPath)
{
	std::ifstream inFile(_wstrModelPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Open File");
	}

	_uint iNumAnimation = 0;
	inFile.read(reinterpret_cast<_byte*>(&iNumAnimation), sizeof(_uint));
	m_vecKeyframeAnimation.resize(iNumAnimation);


	for (_uint i = 0; i < iNumAnimation; ++i)
	{
		ANIMATIONDESC tAnimDesc;
		inFile.read(reinterpret_cast<_byte*>(&tAnimDesc.fDuration), sizeof(_float));
		inFile.read(reinterpret_cast<_byte*>(&tAnimDesc.fTicksPerSecond), sizeof(_float));

		_uint iNumKeyframe;
		inFile.read(reinterpret_cast<_byte*>(&iNumKeyframe), sizeof(_uint));
		m_vecKeyframeAnimation.reserve(iNumKeyframe);
		for (_uint j = 0; j < iNumKeyframe; ++j)
		{
			KEYFRAME tKeyframe;
			inFile.read(reinterpret_cast<_byte*>(&tKeyframe), sizeof(KEYFRAME));

			auto& tAnimKeyframePair = m_vecKeyframeAnimation[i];
			tAnimKeyframePair.first = tAnimDesc;
			tAnimKeyframePair.second.push_back(tKeyframe);
		}
		//inFile.read(reinterpret_cast<_byte*>(m_vecKeyframeAnimation[i].data()), sizeof(KEYFRAME) * iNumKeyframe);
	}

	return S_OK;
}

_bool CPsychokinesis::Interpolate_Keyframe(const KEYFRAME& tKeyframe, const shared_ptr<CTransform> pTransform,
	_float fInterpolatedTime, _float fDistanceThreshold)
{
	_matrix mPlayerMatrix = m_pPlayerTransform->Get_Matrix();
	_vector vPlayerLook = mPlayerMatrix.r[2];
	_matrix mPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixTranslation(0.f, static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_ThrowHeight(), 0.f);

	_matrix	mDestMatrix = (XMMatrixAffineTransformation(XMLoadFloat4(&tKeyframe.vScale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&tKeyframe.vRotation),
		XMLoadFloat4(&tKeyframe.vTranslation))) * mPivotMatrix * mPlayerMatrix;

	_vector		vSourScale, vDestScale;
	_vector		vSourRotation, vDestRotation;
	_vector		vSourTranslation, vDestTranslation;

	XMMatrixDecompose(&vDestScale, &vDestRotation, &vDestTranslation, mDestMatrix);

	_matrix mSrcMatrix = pTransform->Get_Matrix();
	XMMatrixDecompose(&vSourScale, &vSourRotation, &vSourTranslation, mSrcMatrix);

	_vector vScale = XMVectorLerp(vSourScale, vDestScale, fInterpolatedTime);
	_vector vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fInterpolatedTime);
	_vector vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fInterpolatedTime);

	_matrix mNextMatrix = XMMatrixAffineTransformation(vScale,
		XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	_float3 vOriginScale = std::get<2>(static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Get_CopyTuplePivot());
	pTransform->Set_Matrix(mNextMatrix);
	pTransform->Set_Scale(vOriginScale);
	if (abs(XMVectorGetX(XMVector3Length(mDestMatrix.r[3] - mNextMatrix.r[3]))) <= fDistanceThreshold)
	{
		return true;
	}

	return false;
}

void CPsychokinesis::Reset()
{
	m_bThrowing[0] = false;
	//m_bThrowing[1] = false;
	m_bCapture = false;
	m_fChargingTime = 0.f;
	m_fTrackPosition[0] = 0.f;
	//m_fTrackPosition[1] = 0.f;
	m_iCurrentKeyframeIndex[0] = 0;
	//m_iCurrentKeyframeIndex[1] = 0;
	m_iCurrentAnimationIndex = m_iFixedIndex;
	if (m_pCapturedObject)
	{
		static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Cancel();

		m_pCapturedObject->Interactive_PhychoKinesis(false);
		m_pCapturedObject = nullptr;
	}

	m_pSpecialThrowObject = nullptr;

	cout << "RESET" << endl;
}

void CPsychokinesis::DuplicateReset()
{
	m_bThrowing[1] = false;
	m_fTrackPosition[1] = 0.f;
	m_iCurrentKeyframeIndex[1] = 0;
	m_pDuplicatedObject = nullptr;
}

void CPsychokinesis::DestroyObject()
{
	if (m_pCapturedObject)
	{
		static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->Destroy();
	}
	if (m_pRecentThrowObject)
	{
		dynamic_pointer_cast<CMap_Dynamic>(m_pRecentThrowObject)->Destroy();
	}
}

shared_ptr<CPsychokinesis> CPsychokinesis::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CPsychokinesis> pInstance = make_private_shared(CPsychokinesis, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CPsychokinesis::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CPsychokinesis::Clone(any pPlayerTransform)
{
	shared_ptr<CPsychokinesis> pInstance = make_private_shared_copy(CPsychokinesis, *this);

	if (FAILED(pInstance->Initialize(pPlayerTransform)))
	{
		MSG_RETURN(nullptr, "CPsychokinesis::Clone", "Failed to Initialize");
	}

	return pInstance;
}

void CPsychokinesis::Change_BF_Attack(_uint idx, _float fTimeDelta)
{
	m_bThrowing[0] = false;
	m_fTrackPosition[0] = 0.f;
	m_iCurrentKeyframeIndex[0] = 0;
	m_fChargingTime = 0.f;

	if (idx == 2)
	{
		m_iCurrentAnimationIndex = 19;
	}
	if (idx == 3)
	{
		m_iCurrentAnimationIndex = 20;
	}

	if (m_pCapturedObject && m_pPlayer->Get_PlayerDesc().bBrainField)
	{
		m_fChargingTime += fTimeDelta;
		//cout << m_fChargingTime << endl;
		//static_pointer_cast<CMap_Dynamic>(m_pCapturedObject)->SetPhysX(false);
		const pair<ANIMATIONDESC, vector<KEYFRAME>>& AnimKeyframePair = m_vecKeyframeAnimation[m_iCurrentAnimationIndex];
		const KEYFRAME& tKeyframe = AnimKeyframePair.second[0];
		if (Interpolate_Keyframe(tKeyframe, dynamic_pointer_cast<CTransform>(m_pCapturedObject->Get_Component(COMPONENT::TRANSFORM)),
			min(1.f, m_fChargingTime ), 0.1f))
		{
			if(m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A1 ||
				m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A2||
				m_pPlayer->Get_PlayerState()->Get_CurrentState() == PLAYER_STATE::SWING_A3)
			{
				m_bThrowing[0] = true;
				m_bCapture = false;
			}
			//m_fChargingTime = 0.f;
		}
	}
}
