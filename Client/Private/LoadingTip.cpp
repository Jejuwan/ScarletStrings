#include "ClientPCH.h"
#include "LoadingTip.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"

std::pair<std::wstring, std::wstring> CLoadingTip::m_tLoadingTipPair[IDX(SCENE::MAX)] = {
	{ TEXT("Title"), TEXT("Description") }, // STATIC
	{ TEXT("Title"), TEXT("Description") }, // LOADING
	{ TEXT("���������"), TEXT("�ʳ��ɷ��ڸ����� ������ �� ���� ���� ���� ����.\n��Ī �����. ���̸� ���� �� �ִ� ���� ��������̴�.") }, // TEST
	{ TEXT("Title"), TEXT("Description") }, // REALTEST
	{ TEXT("�ֹ�� ����� ��"), TEXT("���̰� Ȱ���ϰ� �ִ� �� �ֺ����� ���� �� �� �ִ� �⹦�� ��.\n��Ը� ������ �ִ� ��ҿ��� Ư�� ���� �Ǿ��ִٴ� ���� �ִ�.") }, // MIZUHA
	{ TEXT("���: ��Ŀ�� Ǯ"), TEXT("ũ�Ⱑ �۰� ź�� ��⸸ �ϴ� ���� �������� ���� ���縦 ��ġä��\n�Ÿ��� �η� �ϱ� ������ ã�Ƴ��� ��ƴ�. �� ���� �� Ȱ���ϸ� �߰��� �� �ִ�.") }, // SUOH
	{ TEXT("��ų: �÷����� Ȯ�� ���� �߰�"), TEXT("��� ȭ�鿡�� ������ �� �ִ� �÷����� ������ �þ��.") }, // SUOH_BATTLE
	{ TEXT("���: ���� ����"), TEXT("�׻� ��Ʋ��Ʋ �����̰� �ֱ� ������ ���� �����ؿ��� �˱� ��ƴ�.\n�״��� �������� �����Ƿ� �� �����߷��� �� ���̴�.") }, // MINDROOM
	{ TEXT("���� �ܸ�"), TEXT("������ī�� �������� �ݵ�� 1�뾿�� ������ �ִ� ���� �޴�ܸ�.\n���ɷ��� ����� �����ϴ� ���� ��� �ܸ��� ���̳ݿ� ����Ǿ� �ִ�.") }, // HIDEOUT
	{ TEXT(""), TEXT("") }, // TOOL
	{ TEXT(""), TEXT("") }, // MAPTOOL
	{ TEXT(""), TEXT("") }, // GAMEPLAY
	{ TEXT("���: �ٽ� ����"), TEXT("������ �Ѹ��� ������ ������ ���ؼ� �� �� ���� ������ �� �ִ�.\n��, ������ �ֵθ��� ������ �ڷ� ������ ������ �����ϴ�. ���Ҹ��� �Ǻ��� �� �ִ�.") }, // CUTSCENE_MIZUHA
};

CLoadingTip::CLoadingTip(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CLoadingTip::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingTip::Initialize(any eLoadingScene)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::VIBUFFER_RECT) | BIT(COMPONENT::TRANSFORM);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER] = make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX_LOADING, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT] = make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Initialize", "Failed to CGameObject::Initialize");
	}

	m_pTransform->Set_Scale({ 1564.f, 216.f, 1.f });
	m_pTransform->Set_State(TRANSFORM::POSITION, { -18.f, -331.f, 0.9f, 1.f });
	m_pTransform->Rotate(TRANSFORM::LOOK, XMConvertToRadians(-3.f));
	m_fTilingFactor = 10.f;

	m_tMaterialDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);

	m_vDescFontPosition = { 185.f, 748.f };
	m_fDescFontRotation = 0.05f;

	m_vTitleFontPosition = { 185.f, 680.f };

	m_eLoadingScene = any_cast<SCENE>(eLoadingScene);

	return S_OK;
}

void CLoadingTip::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

//#ifdef _DEBUG
//	if (CImGui_Manager::Get_Instance()->Is_Enable())
//	{
//		ImGui::Begin("Font Settings");
//
//		ImGui::SliderFloat2("Font Position", &m_vTitleFontPosition.x, -2000.f, 2000.f);
//		ImGui::SliderFloat("Font Rotation", &m_fDescFontRotation, -3.14f, 3.14f);
//
//		ImGui::End();
//
//		ImGui::Begin("Loading Tip");
//
//		_float4x4 matBackGround = m_pTransform->Get_WorldMatrix();
//
//		_bool bTransformDirtyFlag = false;
//		_vector vScale, vQuaternion, vTranslation;
//
//		_float3 vNewPosition;
//		_float3 vNewRotation;
//		_float3 vNewScale;
//		if (XMMatrixDecompose(&vScale, &vQuaternion, &vTranslation, matBackGround))
//		{
//			ImGui::SeparatorText("Tip Transform");
//			vNewPosition = vTranslation;
//			if (ImGui::DragFloat3("Tip Position", &vNewPosition.x, 1.f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewRotation = Function::QuaternionToEuler(vQuaternion, false);
//			if (ImGui::DragFloat3("Tip Rotation", &vNewRotation.x, 0.5f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//			vNewScale = vScale;
//			if (ImGui::DragFloat3("BackGround Scale", &vNewScale.x, 1.0f, -FLT_MAX, +FLT_MAX))
//				bTransformDirtyFlag = true;
//		}
//
//		if (bTransformDirtyFlag)
//		{
//			_float4x4 mNewWorld = XMMatrixAffineTransformation(vNewScale, XMVectorZero(), XMQuaternionRotationRollPitchYawFromVector(
//				_float3(XMConvertToRadians(vNewRotation.x), XMConvertToRadians(vNewRotation.y), XMConvertToRadians(vNewRotation.z))), vNewPosition);
//
//			m_pTransform->Set_Matrix(mNewWorld);
//		}
//
//		ImGui::SliderFloat("Tiling Factor", &m_fTilingFactor, 0.f, 100.f);
//
//		ImGui::End();
//	}
//#endif
}

void CLoadingTip::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Add_RenderObject(UI_LAYER::LAYER_2);
}

HRESULT CLoadingTip::Render()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_OnShader");
	}

	if (FAILED(m_pShader->Bind_RawValue(SHADER_MTRLDIF, &m_tMaterialDesc.vDiffuse, sizeof(_float4))))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to Bind Material");
	}

	if (FAILED(m_pTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}
	if (FAILED(m_pLineTexture->Bind_ShaderResourceView(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXMASK)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CTexture::Bind_ShaderResourceView");
	}

	if (FAILED(m_pShader->Bind_Float("g_fTilingFactor", m_fTilingFactor)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to Bind_Float");
	}
	
	if (FAILED(__super::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Render", "Failed to CGameObject::Render");
	}

	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_tLoadingTipPair[IDX(m_eLoadingScene)].first,
		m_vTitleFontPosition, XMVectorSet(0.5f, 0.5f, 0.5f, 1.f), 0.6f, m_fDescFontRotation);
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_ScarletStrings"), m_tLoadingTipPair[IDX(m_eLoadingScene)].second,
		m_vDescFontPosition, XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.5f, m_fDescFontRotation);
	
	return S_OK;
}

HRESULT CLoadingTip::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	m_pTransform->Set_Matrix(g_mUnit);

	m_pVIBuffer_Rect = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer_Rect)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::Ready_Components", "Failed to Get_Component: CVIBuffer_Rect");
	}
	
	auto pGameInstance = CGameInstance::Get_Instance();
	m_pTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/UI/Dummy/T_ui_Dummy.dds"));
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::CTexture::Create", "Failed to CTexture::Create");
	}
	m_pLineTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Noise/T_ui_noise_Line_3.dds"));
	if (nullptr == m_pLineTexture)
	{
		MSG_RETURN(E_FAIL, "CLoadingFrame::CTexture::Create", "Failed to CTexture::Create");
	}

	return S_OK;
}

shared_ptr<CLoadingTip> CLoadingTip::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any eLoadingScene)
{
	shared_ptr<CLoadingTip> pInstance = make_private_shared(CLoadingTip, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(eLoadingScene)))
	{
		MSG_RETURN(nullptr, "CLoadingTip::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CLoadingTip::Clone(any)
{
	return shared_ptr<CGameObject>();
}
