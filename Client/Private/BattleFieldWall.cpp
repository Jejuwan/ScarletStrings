#include "ClientPCH.h"
#include "BattleFieldWall.h"
#include "GameInstance.h"
#include "PhysXEngine.h"

CBattleFieldWall::CBattleFieldWall(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CBattleFieldWall::CBattleFieldWall(const CBattleFieldWall& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBattleFieldWall::Initialize_Prototype()
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::MODEL_RENDERER);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL_RENDERER] = make_pair(PROTOTYPE_COMPONENT_MODEL_RENDERER, g_aNull);

	return S_OK;
}

HRESULT CBattleFieldWall::Initialize(any matWorld)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTestCube::Initialize", "Failed to CGameObject::Initialize");
	}

	m_tInstBattleFieldAsciiDesc.wstrModelTag = PROTOTYPE_COMPONENT_MODEL_BATTLEFIELD;
	m_tInstBattleFieldAsciiDesc.iPassIndex = 17;
	m_tInstBattleFieldAsciiDesc.fpBindResource = [&](shared_ptr<CModel>, shared_ptr<CShader> pShader)
		{
			const _float4x4 matView = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW);
			_float4 vCamPos = *(_float4*)matView.m[3];
			if (FAILED(pShader->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4))))
			{
				MSG_RETURN(E_FAIL, "CBattleField::RenderInstance", "Failed to Bind_RawValue");
			}
			
			if (FAILED(pShader->Bind_Float("g_fAccTime", m_fAccTime)))
			{
				MSG_RETURN(E_FAIL, "CBattleField::RenderInstance", "Failed to Bind_Float:g_fAccTime");
			}

			if (FAILED(pShader->Bind_ShaderResourceView(SHADER_TEXDISSOLVE, m_pNoiseTexture->Get_ShaderResourceView())))
			{
				MSG_RETURN(E_FAIL, "CBattleField::RenderInstance", "Failed to Bind_ShaderResourceView:SHADER_TEXDISSOLVE");
			}

			return S_OK;
		};

	return S_OK;
}

void CBattleFieldWall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fAccTime += fTimeDelta * 0.1f;

#ifdef _DEBUG
#ifdef ACTIVATE_IMGUI
	//ImGui::Begin("Battle Field");

	//_float3 vScale = m_pTransform->Get_Scale();
	//if (ImGui::SliderFloat3("Scale", &vScale.x, 0.f, 5.f))
	//{
	//	m_pTransform->Set_Scale(vScale);
	//}

	//ImGui::End();
#endif
#endif
}

void CBattleFieldWall::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	m_pModelRenderer->Add_RenderObject(RENDER_GROUP::BLEND, m_tInstBattleFieldAsciiDesc, m_pTransform);
}

HRESULT CBattleFieldWall::Render()
{
	return S_OK;
}

void CBattleFieldWall::Enable_Physicx(_bool bEnable)
{
	for (size_t i = 0; i < m_vecActor.size(); ++i)
	{
		if (bEnable)
		{
			m_bOnScene = true;
			CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->addActor(*m_vecActor[i]);
		}
		else if (m_bOnScene)
			CGameInstance::Get_Instance()->Get_PhysXEngine()->GetScene()->removeActor(*m_vecActor[i]);
	}
}

void CBattleFieldWall::Bake_Physics()
{
	m_vecActor = m_pModelRenderer->Create_Actor(PROTOTYPE_COMPONENT_MODEL_BATTLEFIELD, m_pTransform, true);
	
	m_ActorDesc.strTag = ACTORTAG::NONE;
	for (size_t i = 0; i < m_vecActor.size(); ++i)
	{
		m_vecActor[i]->userData = &m_ActorDesc;
	}
}

HRESULT CBattleFieldWall::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBattleField::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pModelRenderer = Get_Component<CModelRenderer>(COMPONENT::MODEL_RENDERER);
	if (nullptr == m_pModelRenderer)
	{
		MSG_RETURN(E_FAIL, "CBattleField::Ready_Components", "Failed to Get_Component: MODEL_RENDERER");
	}
	
	m_pNoiseTexture = dynamic_pointer_cast<CTexture>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_TEXTURE_NOISE_CSL_050));
	if (nullptr == m_pNoiseTexture)
	{
		MSG_RETURN(E_FAIL, "CBattleField::Ready_Components", "Failed to Clone_Component: Texture");
	}

	m_pTransform = m_pModelRenderer->Add_Instance(PROTOTYPE_COMPONENT_MODEL_BATTLEFIELD);
	Add_Component(COMPONENT::TRANSFORM, m_pTransform);

	return S_OK;
}

shared_ptr<CBattleFieldWall> CBattleFieldWall::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CBattleFieldWall> pInstance = make_private_shared(CBattleFieldWall, pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBattleField::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBattleFieldWall::Clone(any matWorld)
{
	shared_ptr<CBattleFieldWall> pInstance = make_private_shared_copy(CBattleFieldWall, *this);

	if (FAILED(pInstance->Initialize(matWorld)))
	{
		MSG_RETURN(nullptr, "CBattleField::Clone", "Failed to Initialize");
	}

	return pInstance;
}
