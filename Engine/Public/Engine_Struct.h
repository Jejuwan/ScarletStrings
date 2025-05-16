#pragma once

namespace Engine
{
#pragma region Window

	typedef struct tagGraphicDesc
	{
		enum WINMODE	{ FULLSCREEN, WINDOW, MAX };
		WINMODE			eWinMode;
		HWND			hWnd;
		_uint			iWinCX, iWinCY;
		
	} GRAPHICDESC;

	typedef struct tagWndProcDesc
	{
		HWND			hWnd;
		UINT			message;
		WPARAM			wParam;
		LPARAM			lParam;
	} WNDPROCDESC;
	
#pragma endregion
#pragma region Shader

	typedef struct tagLightDesc
	{
		LIGHTTYPE		eLightType;
		SHADOWTYPE		eShadowType;
		XMFLOAT2		vShadowProjection;

		XMFLOAT3		vPosition;
		XMFLOAT3		vDirection;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vAmbient;

		_float			fRange;
		_float			fAttenuation0;
		_float			fAttenuation1;
		_float			fAttenuation2;

		_float			fLifeTime			= 0.f;
		_float			fWeight				= 1.f;
	} LIGHTDESC;

	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vEmissive;
		_float			fShininess;

		tagMaterialDesc() :
			vDiffuse	(1.f, 1.f, 1.f, 1.f),
			vAmbient	(0.f, 0.f, 0.f, 0.f),
			vSpecular	(0.f, 0.f, 0.f, 1.f),
			vEmissive	(0.f, 0.f, 0.f, 1.f),
			fShininess	(1.f)
		{}
	} MATERIALDESC;

	typedef struct tagShaderDesc
	{
		_flags			iShaderFlag;
		_float			fDiffuseTiling;
		_float			fNormalTiling;
		MATERIALDESC	tMaterialDesc;

		tagShaderDesc() :
			iShaderFlag		(0),
			fDiffuseTiling	(1.f),
			fNormalTiling	(1.f)
		{}
	} SHADERDESC;

	typedef struct ENGINE_DLL tagInstModelDesc
	{
		wstring					wstrModelTag;
		function<HRESULT(shared_ptr<class CModel>, shared_ptr<class CShader>)>		fpBindResource;
		_uint					iPassIndex;

		tagInstModelDesc() :
			fpBindResource(nullptr),
			iPassIndex(0)
		{}
	} INSTMODELDESC;

	typedef struct ENGINE_DLL tagInstModelMeshDesc
	{
		wstring					wstrModelTag;
		function<HRESULT(shared_ptr<class CModel>, shared_ptr<class CShader>)>		fpBindResource;
		vector<_uint>			vecPassIndex;
		vector<_uint>			vecMeshIndex;

		tagInstModelMeshDesc() :
			fpBindResource(nullptr)
		{}
	} INSTMODELMESHDESC;

	typedef struct tagInstanceRenderDesc
	{
		wstring						wstrModelTag;
		function<HRESULT(shared_ptr<class CModel>, shared_ptr<class CShader>)>		fpBindResource;
		vector<_uint>				vecPassIndex;
		vector<_uint>				vecMeshIndex;

		vector<shared_ptr<class CTransform>>	vecTransform;

		_uint						iPassIndex;

		tagInstanceRenderDesc() 
			: fpBindResource(nullptr)
			, iPassIndex(0)
		{
			vecTransform.reserve(64);
		}
	} INSTANCERENDERDESC;
  
	typedef struct tagShadow
	{
		_float	fShadowShade	= 1.f;
		_float	fBias			= 0.f;
		_float	fMinShadowAvg	= 0.f;
		_float	fTexelScale		= 1.f;
		XMINT2	vGridSize		= XMINT2(4, 4);
	} SHADOWDESC;

#pragma endregion
#pragma region Vertex

	typedef struct ENGINE_DLL tagVertexPosition
	{
		XMFLOAT3		vPosition;

		const static _uint						iNumElement = 1;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOS;

	typedef struct ENGINE_DLL tagVertexPositionSize
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vSize;

		const static _uint						iNumElement = 2;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSSIZE;

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexCoord;

		const static _uint						iNumElement = 2;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexCubePositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexCoord;

		const static _uint						iNumElement = 2;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXCUBEPOSTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexCoord;

		const static _uint						iNumElement = 3;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexCoord;

		const static _uint						iNumElement = 4;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEX, VTXMESH;

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoordBones
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexCoord;
		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		const static _uint						iNumElement = 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEXBONE, VTXMESHANIM;

#pragma region Instance

	typedef struct ENGINE_DLL tagVertexPositionSizeInstanceTransform
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		const static _uint						iNumElement = VTXPOSSIZE::iNumElement + 4;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSSIZEINSTTRANS;

	typedef struct ENGINE_DLL tagVertexPositionSizeInstanceTransformColor
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		XMFLOAT4			vColor;

		const static _uint						iNumElement = VTXPOSSIZE::iNumElement + 5;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSSIZEINSTTRANSCOLOR;

	typedef struct ENGINE_DLL tagVertexPositionSizeInstanceTransformColorArgument
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		XMFLOAT4			vColor;
		XMFLOAT4			vArgument;

		const static _uint						iNumElement = VTXPOSSIZE::iNumElement + 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSSIZEINSTTRANSCOLORARG;

	typedef struct ENGINE_DLL tagVertexPositionTexcordInstanceTransformColorArgument2
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		XMFLOAT4			vArgument0;
		XMFLOAT4			vArgument1;

		const static _uint						iNumElement = VTXPOSTEX::iNumElement + 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSTEXINSTTRANSARG2;

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoordInstanceTransformColorArgument
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		XMFLOAT4			vColor;
		XMFLOAT4			vArgument;

		const static _uint						iNumElement = VTXMESH::iNumElement + 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEXINSTTRANSCOLORARG, VTXMESHINSTTRANSCOLORARG;

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoordInstanceTransform
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		const static _uint						iNumElement = VTXMESH::iNumElement + 4;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEXINSTTRANSFORM, VTXMESHINSTTRANSFORM;

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoordBonesInstanceTransformColorArgument
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;

		XMFLOAT4			vColor;
		XMFLOAT4			vArgument;

		const static _uint						iNumElement = VTXMESHANIM::iNumElement + 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEXBONEINSTTRANSCOLORARG, VTXMESHANIMINSTTRANSCOLORARG;

#pragma endregion
#pragma endregion
#pragma region Model
	
	typedef struct tagMaterial
	{
		shared_ptr<class CTexture>	pTexture[AI_TEXTURE_TYPE_MAX];
	} MATERIAL;

#pragma endregion
#pragma region Animation

	typedef struct tagKeyFrame
	{
		XMFLOAT4		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT4		vTranslation;
		_float			fTime;
	} KEYFRAME;

	typedef struct tagAnimationDesc
	{
		_float			fDuration;
		_float			fTicksPerSecond;
	} ANIMATIONDESC;

#pragma endregion
#pragma region Navigation

	typedef struct tagNavigationDesc
	{
		_uint			iInitialIndex;
		XMFLOAT4X4		mWorld;
	} NAVIGATIONDESC;

#pragma endregion
#pragma region Collider

	typedef struct tagColliderDesc
	{
		COLLIDER		eType;
		COLLISION_GROUP	eGroup;

		XMFLOAT3		vPosition;
		XMFLOAT3		vSize;
		XMFLOAT3		vRotation;
		_float 			fRadius;
	} COLLIDERDESC;

#pragma endregion
#pragma region Entity

	typedef struct tagEntityDesc
	{
		_float			fMaxHP;
		_float			fHP;
		_float			fAtk;

		_float			fForwardSpeed;
		_float			fLeftSpeed;
		_float			fBackwardSpeed;
		_float			fRightSpeed;
		_float			fSpritPower;
		_float			fJumpPower;

		_float			fKnuckBackGauge;

		XMFLOAT3		vMaxSpeed;
		XMFLOAT3		vResist;
	} ENTITYDESC;

	typedef struct tagSkillDesc
	{
		_uint			iStock						= 0u;
		_float			fCurrentCoolTime			= 0.f;
		_float			fCurrentImmediateCoolTime	= 0.f;
		_float			fCoefficient				= 0.f;

		_uint			iMaxStock					= 0u;
		_float			fCoolTime					= 0.f;
		_float			fImmediateCoolTime			= 0.f;

		_bool			bCall						= false;

		_bool			bAvailable					= false;
	} SKILLDESC;

#pragma endregion

#pragma region Sprite_UI

	typedef struct tagSpriteUIDesc
	{
		_uint			m_iIndex;	
		_uint			m_iIndexMaxX;
		_uint			m_iIndexMaxY;

		_float			m_fTime;
		_float			m_fSizeX;
		_float			m_fSizeY;
		_float 			m_fFramePerSec;

	} SPRITEUIDESC;
#pragma endregion


#pragma region Effect
#pragma endregion

#pragma region Etc

	typedef struct tagXMFLOAT3Hash
	{
		size_t operator()(const DirectX::XMFLOAT3& k) const
		{
			return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
		}
	} XMFLOAT3HASH, FLOAT3HASH;

	typedef struct tagXMFLOAT3Equal
	{
		bool operator()(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs) const
		{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
		}
	} XMFLOAT3EQUAL, FLOAT3EQUAL;

#pragma endregion

#pragma region TEST

	typedef struct tagTestObjDesc
	{
		enum TYPE { STATIC, DYNAMIC };
		TYPE eType;
		XMFLOAT3 vSpawnPos;
		XMFLOAT3 vSpawnLookDir;
	}TESTOBJDESC;

#pragma endregion

	typedef struct tagAttackData
	{
		PLAYER_HURTTYPE eHurtType = PLAYER_HURTTYPE::NONE;
		_int iDamage;
		_uint iType;
	}ATTACKDESC;

	typedef struct tagHitData
	{
		ACTORTAG strAttackTag;
		PLAYER_HURTTYPE eHurtType = PLAYER_HURTTYPE::NONE;
		_bool* AttackColliderActive = nullptr;
		_int iDamage;
		_float fDownGage = 0;
		_float fPowerGuard = 0;
		_uint iType = 0;
		XMFLOAT3 vAttackPose;
		XMMATRIX mEffectMat;
	}HITDESC;

	typedef struct tagPhysxActorData
	{
		ACTORTAG strTag = ACTORTAG::ACTORTAG_END;
		_bool bActive = true;
		_bool bCapture = false;
		_bool bHit = false;
		_bool bBigHit = false;
		_bool bThrowHit = false;
		_bool bThrow = false;
		_bool bAirBone = false;
		XMMATRIX mOwnerMat = XMMatrixIdentity();
		XMMATRIX mEffectMat;
		XMFLOAT3 vOwnerPose;
		XMFLOAT3 vTargetPos = { 0,0,0 };
		ATTACKDESC stAttackDesc;
		HITDESC stHitDesc;
		PxControllerCollisionFlags eCollisionFlag;
	}ACTORDESC;


}
