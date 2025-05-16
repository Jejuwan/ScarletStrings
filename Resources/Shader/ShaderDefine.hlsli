#define MAX_TEXTURE						8
#define	MAX_LIGHT						64
#define MAX_INSTANCE					100
#define MAX_BONE						320
#define PI								3.141592654f
#define PI2								6.283185307f
#define DIVPI							0.318309886f
#define DIV2PI							0.159154943f
#define PIDIV2							1.570796327f
#define PIDIV4							0.785398163f

#define CELL_SHADE_FREQUENCY			4.f

#define DIFFUSE_OFFSET					0.f
#define TRIPLANAR_SLOT					1

#define DIRECTIONAL						0
#define POINT							1

#define STATUS_TEXDIFFUSE				0x0001
#define STATUS_TEXNORMAL				0x0002
#define STATUS_TEXAMBIENT				0x0004
#define STATUS_TEXSPECULAR				0x0008
#define STATUS_TEXEMISSIVE				0x0010
#define STATUS_TRIPLANER_POSITIVE_X		0x0020
#define STATUS_TRIPLANER_POSITIVE_Y		0x0040
#define STATUS_TRIPLANER_POSITIVE_Z		0x0080
#define STATUS_TRIPLANER_NEGATIVE_X		0x0100
#define STATUS_TRIPLANER_NEGATIVE_Y		0x0200
#define STATUS_TRIPLANER_NEGATIVE_Z     0x0400
#define STATUS_TRIPLANER_SHARE_X        0x0800
#define STATUS_TRIPLANER_SHARE_Y        0x1000
#define	STATUS_TRIPLANER_SHARE_Z        0x2000
#define	STATUS_TRIPLANER_SHARE_X_Z 		0x4000
#define	STATUS_TRIPLANER_SYNC_XZ		0x8000

// Sampler

sampler LinearSampler = sampler_state
{
	Filter		            = MIN_MAG_MIP_LINEAR;
	AddressU	            = wrap;
	AddressV	            = wrap;
};

sampler ClampLinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

sampler LinearUClampVLinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = clamp;
};

sampler PointSampler = sampler_state
{
	Filter		            = MIN_MAG_MIP_POINT;
	AddressU	            = wrap;
	AddressV	            = wrap;
};

sampler TargetSampler = sampler_state
{
	Filter		            = MIN_MAG_MIP_POINT;
	AddressU	            = clamp;
	AddressV	            = clamp;
};

sampler LinearMirrorSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = mirror;
    AddressV = mirror;
};

// RasterizerState

RasterizerState RS_Default
{
	FillMode                = solid;
	CullMode                = back;
	FrontCounterClockwise   = false;

//	DepthBias				= 1000;
//	SlopeScaledDepthBias	= 1.f;
};

RasterizerState RS_InverseCull
{
	FillMode                = solid;
	CullMode                = front;
	FrontCounterClockwise   = false;
};

RasterizerState RS_NoneCull
{
	FillMode                = solid;
	CullMode                = none;
	FrontCounterClockwise   = false;
};

RasterizerState RS_WireFrame
{
	FillMode				= wireframe;
	CullMode				= back;
	FrontCounterClockwise	= false;
};

// BlendState

BlendState BS_Default
{
	BlendEnable[0]          = false;
};

BlendState BS_Accumulate
{
	BlendEnable[0]			= true;
	BlendEnable[1]			= true;

	SrcBlend				= One;
	DestBlend				= One;
	BlendOp					= Add;
};

BlendState BS_OpaqueBlend
{
    BlendEnable[0] = true;
    SrcBlend[0] = One;
    DestBlend[0] = Zero;
    BlendOp[0] = ADD;
    
    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = Add;
    
    RenderTargetWriteMask[0] = 15; //Ox0F
};

BlendState BS_AlphaBlend
{
	BlendEnable[0]          = true;
	SrcBlend                = Src_Alpha;
	DestBlend               = Inv_Src_Alpha;
	BlendOp                 = Add;
};

BlendState BS_AdditiveBlend_Particle
{
    AlphaToCoverageEnable = false;
    
    BlendEnable[0]			= true;
    SrcBlend[0]				= Src_Alpha;
    DestBlend[0]			= One;
    BlendOp[0]				= Add;
    
    SrcBlendAlpha[0]		= One;
    DestBlendAlpha[0]		= Zero;
    BlendOpAlpha[0]			= Add;
    
    RenderTargetWriteMask[0] = 15; //Ox0F
};

// DepthStencilState

DepthStencilState DSS_Default
{
	DepthEnable             = true;
	DepthWriteMask          = all;
	DepthFunc               = less_equal;

	StencilEnable			= false;
	FrontFaceStencilFunc	= less;
	FrontFaceStencilPass	= replace;
};

DepthStencilState DSS_IgnoreDepth
{
	DepthEnable             = false;
	DepthWriteMask          = zero;
	DepthFunc               = less_equal;

	StencilEnable			= false;
	FrontFaceStencilFunc	= less;
	FrontFaceStencilPass	= replace;
};

DepthStencilState DSS_ShadowDepth
{
	DepthEnable             = true;
	DepthWriteMask          = all;
	DepthFunc               = less;

	StencilEnable			= false;
	FrontFaceStencilFunc	= less;
	FrontFaceStencilPass	= replace;
};

DepthStencilState DSS_DepthRead_Particle
{
    DepthEnable = true;
    DepthFunc = Less_Equal;
    DepthWriteMask = 0;
};

DepthStencilState DSS_DepthRead_Neon
{
    DepthEnable = true;
    DepthFunc = Less_Equal;
    DepthWriteMask = 0;

    StencilEnable = false;
    FrontFaceStencilFunc = less;
    FrontFaceStencilPass = replace;
};

