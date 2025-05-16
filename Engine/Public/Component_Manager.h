#pragma once
#include "Engine_Define.h"
#pragma region Component
#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"
#include "Mesh.h"
#include "Keyframe.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Sphere.h"
#include "VIBufferInstance_Mesh.h"
#include "VIBufferInstance_Point.h"
#include "VIBufferInstance_Line.h"
#include "VIBufferInstance_Rect.h"
#include "VIBuffer_LineList.h"
#include "ModelRenderer.h"
#pragma endregion
#pragma region Behavior
#include "Physics.h"
#include "Animator.h"
#include "Control.h"
#pragma endregion

BEGIN(Engine)

class CComponent_Manager final : public CSingleton<CComponent_Manager>
{
private:
	explicit CComponent_Manager() DEFAULT;
	virtual ~CComponent_Manager() DEFAULT;

public:
	HRESULT													Reserve_Manager(const SCENE);

public:
	HRESULT													Add_Prototype(const SCENE, const wstring& wstrPrototypeTag, shared_ptr<CComponent> pPrototype);

	shared_ptr<CComponent>									Clone_Component(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);

private:
	shared_ptr<CComponent>									Find_Prototype(const SCENE, const wstring& wstrPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<CComponent>>	ComponentPrototype;
	unique_ptr<ComponentPrototype[]>						m_arrComponentPrototypes;

	SCENE													m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CComponent_Manager>;
};

END
