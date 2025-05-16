#include "EnginePCH.h"
#include "ConstantBuffer_Manager.h"
#include "ConstantBuffer.h"

HRESULT CConstantBuffer_Manager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	m_pDevice	= _pDevice;
	m_pContext	= _pContext;
	
	m_vecConstantBuffers.resize(IDX(SHADER_STAGE::MAX));
	for (size_t i = 0; i < IDX(SHADER_STAGE::MAX); ++i)
	{
		m_vecConstantBuffers[i].resize(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT);
	}//

	return S_OK;
}

HRESULT CConstantBuffer_Manager::Create_Buffer(SHADER_STAGE _eShaderStage, _uint _iSlot, const size_t _nLength, _bool _bReleaseBuffer)
{
	if (SHADER_STAGE::MAX == _eShaderStage || _iSlot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		MSG_RETURN(E_FAIL, "CConstantBuffer_Manager::Create_Buffer", "Invalid Slot");
	}

	if (m_vecConstantBuffers[IDX(_eShaderStage)][_iSlot])
	{
		if (_bReleaseBuffer)
		{
			if (FAILED(Release_Buffer(_eShaderStage, _iSlot)))
			{
				MSG_RETURN(E_FAIL, "CConstantBuffer_Manager::Create_Buffer", "Failed to Release_Buffer");
			}
		}
		else
		{
			MSG_RETURN(E_FAIL, "CConstantBuffer_Manager::Create_Buffer", "Slot Already Exists");
		}
	}

	shared_ptr<CConstantBuffer> pInstance = CConstantBuffer::Create(_eShaderStage, m_pDevice, _nLength);
	if (nullptr == pInstance)
	{
		MSG_RETURN(E_FAIL, "CConstantBuffer_Manager::Create_Buffer", "Failed to CConstantBuffer::Create");
	}

	m_vecConstantBuffers[IDX(_eShaderStage)][_iSlot] = pInstance;

	return S_OK;
}

HRESULT CConstantBuffer_Manager::Update_Buffer(SHADER_STAGE _eShaderStage, _uint _iSlot, void* _pData, size_t _nLength, _uint _iOffset)
{
	if (FAILED(m_vecConstantBuffers[IDX(_eShaderStage)][_iSlot]->Update(m_pContext, _pData, _nLength, _iOffset)))
	{
		MSG_RETURN(E_FAIL, "CConstantBuffer_Manager::Bind_Buffer", "Failed to Bind_ConstantBuffer");
	}

	return S_OK;
}

HRESULT CConstantBuffer_Manager::Bind_Buffer(SHADER_STAGE _eShaderStage, _uint _iSlot)
{
	if (FAILED(m_vecConstantBuffers[IDX(_eShaderStage)][_iSlot]->Bind_ConstantBuffer(m_pContext, _iSlot)))
	{
		MSG_RETURN(E_FAIL, "CConstantBuffer_Manager::Bind_Buffer", "Failed to Bind_ConstantBuffer");
	}

	return S_OK;
}

HRESULT CConstantBuffer_Manager::Release_Buffer(SHADER_STAGE _eShaderStage, _uint _iSlot)
{
	if (nullptr == m_vecConstantBuffers[IDX(_eShaderStage)][_iSlot])
	{
		return S_FALSE;
	}

	m_vecConstantBuffers[IDX(_eShaderStage)][_iSlot] = nullptr;

	return S_OK;
}
