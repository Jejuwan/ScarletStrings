#pragma once
#include "Engine_Define.h"
#include "BlackBoard.h"
#include "System.h"

BEGIN(Engine)

class CRenderer;
class CTransform;
class CShader;
class CTexture;
class CCollider;
class CModel;
class CVIBuffer;
class CMesh;
class CVIBuffer_Rect;
class CVIBuffer_LineList;
class CVIBuffer_Cube;
class CVIBuffer_Sphere;
class CVIBufferInstance_Mesh;
class CVIBufferInstance_Model;
class CVIBufferInstance_Point;
class CVIBufferInstance_Line;
class CVIBufferInstance_Rect;
class CKeyframe;
class CBehaviorTree;
class CPhysics;
class CAnimator;
class CControl;

class ENGINE_DLL CNode abstract : public ISystem
{
protected:
	explicit CNode(NODE = NODE::LEAF);
	explicit CNode(const CNode&) DEFAULT;
	virtual ~CNode() DEFAULT;

public:
	virtual HRESULT					Initialize(shared_ptr<class CBlackBoard>);

public:
	virtual void					Activate();
	virtual STATUS					Invoke(_float fTimeDelta)		PURE;
	virtual void					Terminate();

public:
	_bool							Is_Running() const				{ return m_eStatus == STATUS::RUNNING; }
	_bool							Is_Terminated() const			{ return m_eStatus == STATUS::MAX; }

public:
#ifdef _DEBUG
	void							Set_RootNode()					{ m_bRootNode = true; }
	void							Set_Depth(const _uint iDepth)	{ m_iNodeDepth = iDepth; }

	void							Register_DebugNode(shared_ptr<CNode>);
	virtual void					Refresh_Node(shared_ptr<CNode> pRootNode, shared_ptr<CNode> pParentNode = nullptr);\

	virtual void					Debug(any = g_aNull);
#endif

protected:
	void							Begin_Invoke(_float fTimeDelta);
	STATUS							Return_Invoke();

protected:
	const NODE						m_eNodeType		= NODE::MAX;
	STATUS							m_eStatus		= STATUS::MAX;

	shared_ptr<class CBlackBoard>	m_pBlackBoard;

	_float							m_fTimeAcc		= 0.f;

protected:
#ifdef _DEBUG
	list<shared_ptr<CNode>>			m_lstNodeDebug;
	
	_bool							m_bRootNode		= false;
	_uint							m_iNodeDepth	= 0u;
#endif

protected:
	shared_ptr<CNode>				shared_from_node() { return static_pointer_cast<CNode>(shared_from_this()); }
};

END
