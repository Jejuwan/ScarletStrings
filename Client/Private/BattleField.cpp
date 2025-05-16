#include "ClientPCH.h"
#include "BattleField.h"

#include "GameInstance.h"

#include "BattleFieldWall.h"

CBattleField::CBattleField(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CGameObject(pDevice, pContext)
{
}

CBattleField::CBattleField(const CBattleField& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBattleField::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattleField::Initialize(any wstrFilePath)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBattleField::Initialize", "Failed to CGameObject::Initialize");
	}

	Import(any_cast<wstring>(wstrFilePath));

	return S_OK;
}

void CBattleField::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto pBattleField : m_vecBattleField)
	{
		pBattleField->Tick(fTimeDelta);
	}
}

void CBattleField::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	for (auto pBattleField : m_vecBattleField)
	{
		pBattleField->Late_Tick(fTimeDelta);
	}
}

HRESULT CBattleField::Render()
{
	return S_OK;
}

void CBattleField::Set_Enable(_bool bEnable)
{
	for (auto pBattleField : m_vecBattleField)
	{
		pBattleField->Enable_Physicx(bEnable);
	}

	__super::Set_Enable(bEnable);
}

HRESULT CBattleField::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBattleFieldGenerator::Ready_Components", "Failed to CGameObject::Ready_Components");
	}

	return S_OK;
}

HRESULT CBattleField::Import(const wstring& wstrFilePath)
{
	std::ifstream inFile(wstrFilePath, std::ios::binary);
	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CBattleField::Import", "Failed to Open File");
	}

	_uint iNumBattleField = 0;
	inFile.read(reinterpret_cast<char*>(&iNumBattleField), sizeof(_uint));

	m_vecBattleField.reserve(iNumBattleField);

	for (_uint i = 0; i < iNumBattleField; ++i)
	{
		_float4x4 matWorld;
		inFile.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

		auto pObject = CGameInstance::Get_Instance()->Clone_GameObject(CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_BATTLEFIELD_WALL);

		auto pBattleField = dynamic_pointer_cast<CBattleFieldWall>(pObject);
		m_vecBattleField.emplace_back(pBattleField);

		auto pBattleFieldTransform = dynamic_pointer_cast<CTransform>(pObject->Get_Component(COMPONENT::TRANSFORM));
		pBattleFieldTransform->Set_Matrix(matWorld);
		pBattleField->Bake_Physics();
	}
	return S_OK;
}

shared_ptr<CBattleField> CBattleField::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
	shared_ptr<CBattleField> pInstance = make_private_shared(CBattleField, pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBattleField::Create", "Failed to Initialze_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBattleField::Clone(any wstrFilePath)
{
	shared_ptr<CBattleField> pInstance = make_private_shared_copy(CBattleField, *this);

	if (FAILED(pInstance->Initialize(wstrFilePath)))
	{
		MSG_RETURN(nullptr, "CBattleField::Clone", "Failed to Initialze");
	}

	return pInstance;
}
