#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	explicit CModel(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CModel(const CModel&);
	virtual ~CModel() DEFAULT;

public:
	HRESULT														Initialize_Prototype(const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit, _uint iNumInstance = 0);
	HRESULT														Initialize(any pMaterialDesc = g_aNull);

	HRESULT														Render(shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT														Render(_uint iMeshIndex, shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT														Render_ShadowDepth(shared_ptr<class CGameObject>, shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT														Render_Instance(_uint iMeshIndex, shared_ptr<class CShader>, _uint iPassIndex);
	void														Bind_Event(EVENTTYPE EventType, function<void(void)> Function);

	HRESULT														Render_Instance_Model(_uint iDrawCount, shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT														Render_Instance_Model(_uint iDrawCount, shared_ptr<class CShader>, _uint iPassIndex, _uint iMeshIndex);
	void														Initialize_MaterialDesc(MATERIALDESC* pMaterial);

private:
#if ACTIVATE_TOOL
	HRESULT														Initialize_FromAssimp(const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
#endif
	HRESULT														Initialize_FromBinary(const wstring& wstrModelPath);
	HRESULT														Initialize_FromBinary(std::ifstream&);

#if ACTIVATE_TOOL
	HRESULT														Ready_Bones(const aiNode*, _uint iParentBoneIndex);
	HRESULT														Ready_Animations(const aiScene*);
	HRESULT														Ready_Meshes(const aiScene*, _matrixf mPivot);
	HRESULT														Ready_Materials(const aiScene*, const wstring& wstrModelPath);
#endif
public:
	HRESULT														Ready_Instances(_uint iNumInstance);
	HRESULT														BoneChange(vector<shared_ptr<class CBone>> vecBones);
	vector<shared_ptr<class CBone>>								GetBones();

public:
	_bool														Is_AnimationPlaying(_uint iAnimationIndex) const;
	_bool														Is_AnimationFinished(_uint iAnimationIndex) const;

	_uint														Get_BoneIndex(const _char* szBoneName) const;
	_uint														Get_MeshIndex(const _char* szMeshName) const;
	const _char*												Get_MeshName(_uint iMeshIndex) const;
	_float4x4													Get_Pivot() const { return m_mPivot; }
	void														Set_Pivot(_matrix m) { m_mPivot = m; }

	const MODEL													Get_Type() const { return m_eType; }

	const _uint													Get_NumBones() const { return m_iNumBones; }
	const _uint													Get_NumAnimations() const { return m_iNumAnimations; }
	const _uint													Get_NumMeshes() const { return m_iNumMeshes; }
	const _uint													Get_NumMaterials() const { return m_iNumMaterials; }
	const _uint													Get_NumInstances() const { return m_iNumInstances; }

	shared_ptr<class CBone>										Get_Bone(const _char* szBoneName) const { return m_vecBones[Get_BoneIndex(szBoneName)]; }
	shared_ptr<class CBone>										Get_Bone(_uint iIndex) const { return m_vecBones[iIndex]; }
	shared_ptr<class CAnimation>								Get_Animation(_uint iIndex) const { return m_vecAnimations[iIndex]; }
	shared_ptr<class CMesh>										Get_Mesh(const _char* szMeshName) const { return m_vecMeshes[Get_MeshIndex(szMeshName)]; }
	shared_ptr<class CMesh>										Get_Mesh(_uint iIndex) const { return m_vecMeshes[iIndex]; }
#if ACTIVATE_TOOL
	MATERIAL													Get_Material(_uint iIndex) const { return m_vecMaterials[iIndex]; }

	const _uint													Get_MeshIndex(shared_ptr<class CMesh>) const;
	const _uint													Get_AnimationIndex(shared_ptr<class CAnimation>) const;
	const _uint													Get_BoneAnimationIndex(_uint iBoneIndex) const { return m_vecBoneAnimationIndices[iBoneIndex]; }

#endif
	const _float4x4*											Get_BoneMatrices(_uint iMeshIndex);
#if ACTIVATE_TOOL


	void														Set_BoneAnimationIndex(_uint iBoneIndex, _uint iAnimationIndex) { m_vecBoneAnimationIndices[iBoneIndex] = iAnimationIndex; }
#endif
	void														Set_BonePivot(_uint iBoneIndex, _matrixf mPivot) { m_vecBonePivot[iBoneIndex] = mPivot; };
	void														Set_DefaultAnimation(_uint iAnimationIndex);

#if ACTIVATE_TOOL
#endif

public:
	const SHADERDESC											Get_ShaderDesc(_uint iMeshIndex) const { return m_vecShaderDescs[iMeshIndex]; }
	void														Set_ShaderDesc(_uint iMeshIndex, SHADERDESC tShaderDesc) { m_vecShaderDescs[iMeshIndex] = tShaderDesc; }


public:
	void														Set_Animation(_uint iAnimationIndex, _float fPlaySpeed = 1.f, _bool bReverse = false, _float fInterpolationDuration = g_fDefaultInterpolationDuration, _bool bLoop = true);
	void														Fix_Animation(_uint iAnimationIndex, _float fRatio);
	void														Blend_Animation(_uint iAnimationIndex, _float fRatio);

	void														Reset_Animation(_uint iAnimationIndex);
	void														Tick_Animation(_float fTimeDelta);

	void														Interpolate_Animation(_float fTimeDelta);

	void														Iterate_Meshes(function<_bool(shared_ptr<class CMesh>)>);

	HRESULT														Show_MeshFromAnimations(_uint iAnimationIndex, _uint iMeshIndex);
	HRESULT														Hide_MeshFromAnimations(_uint iAnimationIndex, _uint iMeshIndex);

	_bool														Is_MeshHidden(_uint iMeshIndex) const;
	void														Hide_Mesh(_uint iMeshIndex, _bool bHide = true);

	void														Update_Instance(function<void(void*, _uint iNumInstance)>);
	void														Copy_BoneMatrices(_uint iMeshIndex, void* pDest);

	_float														Get_FarestPoint() { return m_fFarestPoint; }

public:
	HRESULT														Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>);
	HRESULT														Bind_BoneMatrices(_uint iMeshIndex, shared_ptr<class CShader>, const _char* szConstantName);
	HRESULT														Bind_ShaderOptions(_uint iMeshIndex, shared_ptr<class CShader>);
private:
	HRESULT														Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, _uint iTextureIndex = 0);
	HRESULT														Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const _char* szConstantName, _uint iTextureIndex = 0);
	HRESULT														Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const _char* szConstantName);

private:
	MODEL														m_eType = MODEL::MAX;
	_float4x4													m_mPivot = g_mUnit;

	_uint														m_iNumBones = 0;
	vector<shared_ptr<class CBone>>								m_vecBones;

	_uint														m_iNumAnimations = 0;
	vector<shared_ptr<class CAnimation>>						m_vecAnimations;

	_uint														m_iNumMeshes = 0;
	vector<shared_ptr<class CMesh>>								m_vecMeshes;



	_uint														m_iNumMaterials = 0;
	vector<MATERIAL>											m_vecMaterials;

	vector<SHADERDESC>											m_vecShaderDescs;

	vector<MATERIALDESC>										m_vecMaterialDescs;

	vector<_uint>												m_vecBoneAnimationIndices;
	vector<_float4x4>											m_vecBonePivot;
	unordered_set<_uint>										m_usetAnimationPlayingIndices;
	unordered_map<_uint, _float>								m_umapAnimationFixRatio;
	_float														m_fAnimationPlaySpeed = 1.f;
	_bool														m_bAnimReverse = false;
	_bool														m_bAnimLoop = true;

	multimap<_uint, _uint>										m_mapHideMeshFromAnimations;
	_uint														m_iCurruntAnimIndex = 0;
	_uint														m_iNextAnimIndex = 0;

	_float4														m_vRootPos;
	_float4														m_vRootAddPos;
	_float4x4													m_mRootMatrix;

	_float														m_fProportion;
	_float														m_fTrackPosition;
	_float														m_fDuration;
	_float														m_fInterpolateTime = 1.f;
	_float														m_fFarestPoint = 0.f;

	vector<class PxTriangleMesh*>								m_vecTriangleMeshes;

	vector<_uint>												m_vecMeshHide;

	_uint														m_iNumInstances = 0;
	vector<shared_ptr<class CVIBufferInstance_Mesh>>			m_vecMeshInstances;
	

	MATERIALDESC*												m_pMaterialDesc	= nullptr;

public:
	static shared_ptr<CModel>									Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit, _uint iNumInstance = 0);
	virtual shared_ptr<CComponent>								Clone(any pMaterialDesc = g_aNull) override;

	static shared_ptr<CModel>									Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, std::ifstream&);
#if ACTIVATE_TOOL
	HRESULT														Export(const wstring& wstrPath);
	HRESULT														Export(std::ofstream&);

	HRESULT														Export_Animation(const wstring& wstrPath);

#ifdef _DEBUG
	HRESULT														Export_ShaderMaterial();
	wstring														m_wstrModelPath;
#endif
#endif

	_float4														Get_RootPos() {
		return m_vRootPos;
	}
	_float4														Get_RootAddPos() {
		return m_vRootAddPos;
	}
	_float4x4													Get_RootMatrix() { return m_mRootMatrix; }
	HRESULT														SetRootBoneIndex(_uint Reference, _uint RootBone);
	HRESULT														SetRootBoneAddIndex(_uint RootAdd);

	_float														Get_Proportion() { return m_fProportion; }
	_float														Get_TrackPosition() {return m_fTrackPosition;}
	_float														Get_Duration() { return m_fDuration; }
	void														Set_TrackPosition(_float f);
	vector<PxTriangleMesh*>& Get_TriangleMeshes() { return m_vecTriangleMeshes; }
	_uint														Get_NextAnimIndex() { return m_iNextAnimIndex; }
	void														Set_NextAnimIndex(_uint i) { m_iNextAnimIndex = i; }
	shared_ptr<class CAnimation>								Get_CurrentAnim() { return m_vecAnimations[m_iCurruntAnimIndex]; }
	void														Set_InterpolateTime(_float f) { m_fInterpolateTime = f; }
	_uint														Get_CurrentAnimIndex() { return m_iCurruntAnimIndex; }
	_bool														Get_InterpolateEnd();
	void														Set_InterpolateEnd(_bool b);
};
END