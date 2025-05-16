#pragma once

#define SHADER_TIME								"g_fGameTime"
#define SHADOW_ID								"g_fShadowID"

#define SHADER_FLAG								"g_iShaderFlag"
#define SHADER_FLAG_TEXDIFFUSE					0b0000'0000'0000'0001
#define SHADER_FLAG_TEXNORMAL					0b0000'0000'0000'0010
#define SHADER_FLAG_TEXAMBIENT					0b0000'0000'0000'0100
#define SHADER_FLAG_TEXSPECULAR					0b0000'0000'0000'1000
#define SHADER_FLAG_TEXEMISSIVE					0b0000'0000'0001'0000
#define SHADER_FLAG_TRIPLANER_POSITIVE_X		0b0000'0000'0010'0000
#define SHADER_FLAG_TRIPLANER_POSITIVE_Y		0b0000'0000'0100'0000
#define SHADER_FLAG_TRIPLANER_POSITIVE_Z		0b0000'0000'1000'0000
#define SHADER_FLAG_TRIPLANER_NEGATIVE_X		0b0000'0001'0000'0000
#define SHADER_FLAG_TRIPLANER_NEGATIVE_Y		0b0000'0010'0000'0000
#define SHADER_FLAG_TRIPLANER_NEGATIVE_Z		0b0000'0100'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_X			0b0000'1000'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_Y			0b0001'0000'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_Z			0b0010'0000'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_X_Z			0b0100'0000'0000'0000
#define SHADER_FLAG_TRIPLANER_SYNC_XZ			0b1000'0000'0000'0000

#define SHADER_CAMFAR							"g_fCamFar"

#define SHADER_TEXMASK							"g_texMask"
#define SHADER_TEXEXTRAMASK						"g_texExtraMask"
#define SHADER_TEXDIFFUSE						"g_texDiffuse"
#define SHADER_TEXNORMAL						"g_texNormal"
#define SHADER_TEXAMBIENT						"g_texAmbient"
#define SHADER_TEXSPECULAR						"g_texSpecular"
#define SHADER_TEXEMISSION						"g_texEmission"
#define SHADER_TEXDISSOLVE						"g_texDissolve"

#define SHADER_REFLECTION						"g_texReflection"

#define SHADER_TEXHEATMAP						"g_texHeatMap"

#define SHADER_INSTANCEINDEX					"g_iInstanceIndex"

#define SHADER_TILING_DIFFUSE					"g_fTilingDiffuse"
#define SHADER_TILING_NORMAL					"g_fTilingNormal"
#define SHADER_TILING_EMISSIVE					"g_fTilingEmissive"
#define SHADER_TILING2_DIFFUSE					"g_vTilingDiffuse"
#define SHADER_TILING2_NORMAL					"g_vTilingNormal"
#define SHADER_TILING2_SPECULAR					"g_vTilingSpecular"
#define SHADER_TILING2_EMISSIVE					"g_vTilingEmissive"

#define SHADER_UVOFFSET							"g_vUVOffset"
#define SHADER_UVOFFSET1						"g_vUVOffset1"
#define SHADER_UVOFFSET2						"g_vUVOffset2"
#define SHADER_UVOFFSET3						"g_vUVOffset3"

#define SHADER_MATWORLD							"g_mWorld"
#define SHADER_MATVIEW							"g_mView"
#define SHADER_MATVIEWINV						"g_mViewInv"
#define SHADER_MATPROJ							"g_mProj"
#define SHADER_MATPROJINV						"g_mProjInv"
#define SHADER_MATORTHOGRAPHIC					"g_mOrthographic"
#define SHADER_MATQUADWORLD						"g_mQuadWorld"
#define SHADER_MATQUADVIEW						"g_mQuadView"
#define SHADER_MATQUADPROJ						"g_mQuadProj"

#define SHADER_SHADOWCOUNT						"g_iShadowCount"
#define SHADER_MATSHADOWVIEW					"g_mShadowView"
#define SHADER_MATSHADOWPROJ					"g_mShadowProj"
#define SHADER_MATSHADOWVIEWS					"g_mShadowViews"
#define SHADER_MATSHADOWPROJS					"g_mShadowProjs"

#define SHADER_CAMPOS							"g_vCamPosition"

#define SHADER_TEXTARGET_SHADOWDEPTH			"g_texShadowDepthTarget"
#define SHADER_TEXTARGET_MTRL_DIFFUSE			"g_texMtrlDiffuseTarget"
#define SHADER_TEXTARGET_MTRL_AMBIENT			"g_texMtrlAmbientTarget"
#define SHADER_TEXTARGET_MTRL_SPECULAR			"g_texMtrlSpecularTarget"
#define SHADER_TEXTARGET_MTRL_EMISSIVE			"g_texMtrlEmissiveTarget"
#define SHADER_TEXTARGET_NORMAL					"g_texNormalTarget"
#define SHADER_TEXTARGET_DEPTH					"g_texDepthTarget"
#define SHADER_TEXTARGET_PREMASK				"g_texPreMaskTarget"
#define SHADER_TEXTARGET_LENSFLARE				"g_texLensflareTarget"
#define SHADER_TEXTARGET_OUTLINE				"g_texOutLineTarget"
#define SHADER_TEXTARGET_LIGHTDIFFUSE			"g_texLightDiffuseTarget"
#define SHADER_TEXTARGET_LIGHTAMBIENT			"g_texLightAmbientTarget"
#define SHADER_TEXTARGET_LIGHTSPECULAR			"g_texLightSpecularTarget"
#define SHADER_TEXTARGET_SHADOWPROCESS			"g_texShadowProcessTarget"
#define SHADER_TEXTARGET_PREPROCESS				"g_texPreProcessTarget"
#define SHADER_TEXTARGET_POSTPROCESS			"g_texPostProcessTarget"
#define SHADER_TEXTARGET_PROCESSDATA			"g_texProcessDataTarget"
#define SHADER_TEXTARGET_MASK					"g_texMaskTarget"
#define SHADER_TEXTARGET_BLOOM					"g_texBloomTarget"
#define SHADER_TEXTARGET_BLOOMSTRENGTH			"g_texBloomStrengthTarget"
#define SHADER_TEXTARGET_BLOOMMASK				"g_texBloomMaskTarget"
#define SHADER_TEXTARGET_BLUR					"g_texBlurTarget"
#define SHADER_TEXTARGET_SSAO					"g_texSSAOTarget"
#define SHADER_TEXTARGET_UI_3D					"g_texUI_3DTarget"

#define SHADER_LIGHTCNT							"g_iLightCount"
#define SHADER_LIGHTTYPE						"g_iLightType"
#define SHADER_LIGHTPOS							"g_vLightPosition"
#define SHADER_LIGHTDIR							"g_vLightDirection"
#define SHADER_LIGHTDIF							"g_vLightDiffuse"
#define SHADER_LIGHTAMB							"g_vLightAmbient"
#define SHADER_LIGHTSPC							"g_vLightSpecular"
#define SHADER_LIGHTRNG							"g_fLightRange"
#define SHADER_ATT0								"g_fLightAttenuation0"
#define SHADER_ATT1								"g_fLightAttenuation1"
#define SHADER_ATT2								"g_fLightAttenuation2"

#define SHADER_MTRLDIF							"g_vMtrlDiffuse"
#define SHADER_MTRLAMB							"g_vMtrlAmbient"
#define SHADER_MTRLSPC							"g_vMtrlSpecular"
#define SHADER_MTRLEMS							"g_vMtrlEmissive"
#define SHADER_MTRLSHN							"g_fMtrlShininess"

#define SHADER_BONE								"g_mBones"

#define SHADER_TRIPLANER_SHARP					"g_fTPSharpness"

#define SHADER_SIZE								"g_vSize"
#define SHADER_THICKNESS						"g_fThickness"
#define SHADER_MAXINSTANCE						"g_iMaxInstance"

#define SHADER_OUTLINE_SIZE						"g_fOutLineSize"
#define SHADER_OUTLINE_COLOR					"g_vOutlineColor"

#define SHADER_FOG_ENABLE						"g_bFog"
#define SHADER_FOG_COLOR						"g_vFogColor"
#define SHADER_FOG_START						"g_fFogStart"
#define SHADER_FOG_END							"g_fFogEnd"
#define SHADER_FOG_AMBIENT						"g_fFogAmbient"
#define SHADER_FOG_MAX							"g_fFogMax"
#define SHADER_FOG_POWER						"g_fFogPower"

#define SHADER_SPRITESHEET_INDEX				"g_iSpriteSheetIndex"
#define SHADER_SPRITESHEET_SIZE					"g_vSpriteSheetSize"
#define SHADER_FADERATIO						"g_fFadeRatio"
#define SHADER_FADESCOPE						"g_vFadeScope"

#define SHADER_DISTORTION_POWER					"g_fDistortionPower"
#define SHADER_BLOOMSTRENGTH					"g_fBloomStrength"

#define RENDERTARGET_SHADOWDEPTH				TEXT("RenderTarget:ShadowDepth")

#define RENDERTARGET_BLUR_BUFFER_A				TEXT("RenderTarget:Blur:Buffer:A")
#define RENDERTARGET_BLUR_BUFFER_B				TEXT("RenderTarget:Blur:Buffer:B")

#define RENDERTARGET_MATERIAL_DIFFUSE			TEXT("RenderTarget:Material:Diffuse")
#define RENDERTARGET_MATERIAL_AMBIENT			TEXT("RenderTarget:Material:Ambient")
#define RENDERTARGET_MATERIAL_SPECULAR			TEXT("RenderTarget:Material:Specular")
#define RENDERTARGET_MATERIAL_EMISSIVE			TEXT("RenderTarget:Material:Emissive")

#define RENDERTARGET_NORMAL						TEXT("RenderTarget:Normal")
#define RENDERTARGET_DEPTH						TEXT("RenderTarget:Depth")
#define RENDERTARGET_PREMASK					TEXT("RenderTarget:PreMask")

#define RENDERTARGET_OUTLINE					TEXT("RenderTarget:OutLine")
#define RENDERTARGET_OUTLINE_SCALING			TEXT("RenderTarget:OutLine:Scaling")

#define RENDERTARGET_LIGHT_SSAO					TEXT("RenderTarget:Light:SSAO")

#define RENDERTARGET_LIGHT_DIFFUSE				TEXT("RenderTarget:Light:Diffuse")
#define RENDERTARGET_LIGHT_AMBIENT				TEXT("RenderTarget:Light:Ambient")
#define RENDERTARGET_LIGHT_SPECULAR				TEXT("RenderTarget:Light:Specular")

#define RENDERTARGET_BLOOM						TEXT("RenderTarget:Bloom")
#define RENDERTARGET_BLOOMSTRENGTH				TEXT("RenderTarget:BloomStrength")
#define RENDERTARGET_BLOOMMASK					TEXT("RenderTarget:BloomMask")

#define RENDERTARGET_NEON						TEXT("RenderTarget:Neon")
#define RENDERTARGET_NEON_COMBINE				TEXT("RenderTarget:Neon:Combine")

#define RENDERTARGET_SHADOWPROCESS				TEXT("RenderTarget:ShadowProcess")

#define RENDERTARGET_PREPROCESS					TEXT("RenderTarget:PreProcess")
#define RENDERTARGET_PROCESSDATA				TEXT("RenderTarget:ProcessData")
#define RENDERTARGET_PREMASK					TEXT("RenderTarget:PreMask")
#define RENDERTARGET_MASK						TEXT("RenderTarget:Mask")
#define RENDERTARGET_PREPROCESS_COMBINE			TEXT("RenderTarget:PreProcess:Combine")
#define RENDERTARGET_POSTPROCESS				TEXT("RenderTarget:PostProcess")
#define RENDERTARGET_UI_3D						TEXT("RenderTarget:UI_3D")
#define RENDERTARGET_UI_3D_CHARACTER			TEXT("RenderTarget:UI:3D:Character")

#define RENDERTARGET_CUTSCENE_MAIN				TEXT("RenderTarget:Cutscene:Main")

#define DEPTHSTENCIL_SHADOWDEPTH				TEXT("DepthStencil:ShadowDepth")
#define DEPTHSTENCIL_CUTSCENE					TEXT("DepthStencil:Cutscene")
#define DEPTHSTENCIL_OUTLINE					TEXT("DepthStencil:OutLine")
#define DEPTHSTENCIL_UI_3D						TEXT("DepthStencil:UI_3D")

#define MULTIRENDERTARGET_BLUR_BUFFER_A			TEXT("MultiRenderTarget:Blur:Buffer:A")
#define MULTIRENDERTARGET_BLUR_BUFFER_B			TEXT("MultiRenderTarget:Blur:Buffer:B")
#define MULTIRENDERTARGET_SHADOWDEPTH			TEXT("MultiRenderTarget:ShadowDepth")
#define MULTIRENDERTARGET_NONBLEND				TEXT("MultiRenderTarget:NonBlend")
#define MULTIRENDERTARGET_SSAO					TEXT("MultiRenderTarget:SSAO")
#define MULTIRENDERTARGET_LIGHT					TEXT("MultiRenderTarget:Light")
#define MULTIRENDERTARGET_SHADOWPROCESS			TEXT("MultiRenderTarget:ShadowProcess")
#define MULTIRENDERTARGET_PREPROCESS			TEXT("MultiRenderTarget:PreProcess")
#define MULTIRENDERTARGET_PREPROCESS_COMBINE	TEXT("MultiRenderTarget:PreProcess:Combine")
#define MULTIRENDERTARGET_POSTPROCESS			TEXT("MultiRenderTarget:PostProcess")
#define MULTIRENDERTARGET_OUTLINE				TEXT("MultiRenderTarget:OutLine")
#define MULTIRENDERTARGET_BLOOM					TEXT("MultiRenderTarget:Bloom")
#define MULTIRENDERTARGET_CUTSCENE_MAIN			TEXT("MultiRenderTarget:Cutscene:Main")
#define MULTIRENDERTARGET_UI_3D					TEXT("MultiRenderTarget:UI_3D")
#define MULTIRENDERTARGET_NEON					TEXT("MultiRenderTarget:NEON")