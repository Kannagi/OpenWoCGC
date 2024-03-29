#include "system/gc/__GXInit.h"


GXContext __GXContext;
GXContexts __GXContexts = { &__GXContext, NULL };
void* __piReg  = NULL;
void* __cpReg  = NULL;
void* __peReg  = NULL;
void* __memReg = NULL;

/*
GXFifoObj* GXInit(void* base, u32 size)
{
	static u32 resetFuncRegistered = 0;
	u32 i;

	OSRegisterVersion(__GXVersion);
	gx->inDispList    = GX_FALSE;
	gx->dlSaveContext = GX_TRUE;
	gx->abtWaitPECopy = GX_TRUE;

	gx->tcsManEnab = 0;
	gx->tevTcEnab  = 0;

	GXSetMisc(GX_MT_XF_FLUSH, 0);

	__piReg  = (void*)OSPhysicalToUncached(GX_PI_ADDR);
	__cpReg  = (void*)OSPhysicalToUncached(GX_CP_ADDR);
	__peReg  = (void*)OSPhysicalToUncached(GX_PE_ADDR);
	__memReg = (void*)OSPhysicalToUncached(GX_MEM_ADDR);

	__GXFifoInit();

	GXInitFifoBase(&FifoObj, base, size);
	GXSetCPUFifo(&FifoObj);
	GXSetGPFifo(&FifoObj);

	if (!resetFuncRegistered) {
		OSRegisterResetFunction(&GXResetFuncInfo);
		resetFuncRegistered = 1;
	}

	__GXPEInit();
	EnableWriteGatherPipe();

	gx->genMode = 0;
	FAST_FLAG_SET(gx->genMode, 0, 24, 8);

	gx->bpMask = 255;
	FAST_FLAG_SET(gx->bpMask, 0xF, 24, 8);

	gx->lpSize = 0;
	FAST_FLAG_SET(gx->lpSize, 34, 24, 8);

	for (i = 0; i < GX_MAXTEVSTAGE; i++) {
		gx->tevc[i]     = 0;
		gx->teva[i]     = 0;
		gx->tref[i / 2] = 0;
		gx->texmapId[i] = GX_TEXMAP_NULL;

		FAST_FLAG_SET(gx->tevc[i], 0xC0 + i * 2, 24, 8);
		FAST_FLAG_SET(gx->teva[i], 0xC1 + i * 2, 24, 8);
		FAST_FLAG_SET(gx->tevKsel[i / 2], 0xF6 + i / 2, 24, 8);
		FAST_FLAG_SET(gx->tref[i / 2], 0x28 + i / 2, 24, 8);
	}

	gx->iref = 0;
	FAST_FLAG_SET(gx->iref, 0x27, 24, 8);

	for (i = 0; i < GX_MAX_TEXCOORD; i++) {
		gx->suTs0[i] = 0;
		gx->suTs1[i] = 0;

		FAST_FLAG_SET(gx->suTs0[i], 0x30 + i * 2, 24, 8);
		FAST_FLAG_SET(gx->suTs1[i], 0x31 + i * 2, 24, 8);
	}

	FAST_FLAG_SET(gx->suScis0, 0x20, 24, 8);
	FAST_FLAG_SET(gx->suScis1, 0x21, 24, 8);

	FAST_FLAG_SET(gx->cmode0, 0x41, 24, 8);
	FAST_FLAG_SET(gx->cmode1, 0x42, 24, 8);

	FAST_FLAG_SET(gx->zmode, 0x40, 24, 8);
	FAST_FLAG_SET(gx->peCtrl, 0x43, 24, 8);

	FAST_FLAG_SET(gx->cpTex, 0, 7, 2);

	gx->zScale  = 1.6777216E7f;
	gx->zOffset = 0.0f;

	gx->dirtyState = 0;
	gx->dirtyVAT   = 0;

	{
		u32 val1;
		u32 val2;

		val2 = OS_BUS_CLOCK / 500;

		__GXFlushTextureState();

		val1 = (val2 / 2048) | 0x69000400;

		GX_WRITE_U8(0x61);
		GX_WRITE_U32(val1);

		__GXFlushTextureState();

		val1 = (val2 / 4224) | 0x46000200;
		GX_WRITE_U8(0x61);
		GX_WRITE_U32(val1);
	}

	for (i = 0; i < 8; i++) {
		FAST_FLAG_SET(gx->vatA[i], 1, 30, 33);
		FAST_FLAG_SET(gx->vatB[i], 1, 31, 33);

		GX_WRITE_U8(0x8);
		GX_WRITE_U8(i | 0x80);
		GX_WRITE_U32(gx->vatB[i]);
	}

	{
		u32 reg1 = 0;
		u32 reg2 = 0;

		FAST_FLAG_SET(reg1, 1, 0, 1);
		FAST_FLAG_SET(reg1, 1, 1, 1);
		FAST_FLAG_SET(reg1, 1, 2, 1);
		FAST_FLAG_SET(reg1, 1, 3, 1);
		FAST_FLAG_SET(reg1, 1, 4, 1);
		FAST_FLAG_SET(reg1, 1, 5, 1);
		GX_WRITE_U8(0x10);
		GX_WRITE_U32(0x1000);
		GX_WRITE_U32(reg1);

		FAST_FLAG_SET(reg2, 1, 0, 1);
		GX_WRITE_U8(0x10);
		GX_WRITE_U32(0x1012);
		GX_WRITE_U32(reg2);
	}

	{
		u32 reg = 0;
		FAST_FLAG_SET(reg, 1, 0, 1);
		FAST_FLAG_SET(reg, 1, 1, 1);
		FAST_FLAG_SET(reg, 1, 2, 1);
		FAST_FLAG_SET(reg, 1, 3, 1);
		FAST_FLAG_SET(reg, 0x58, 24, 8);
		GX_WRITE_U8(0x61);
		GX_WRITE_U32(reg);
	}

	for (i = 0; i < GX_MAX_TEXMAP; i++) {
		GXInitTexCacheRegion(&gx->TexRegions0[i], GX_FALSE, GXTexRegionAddrTable[i], GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 8],
		                     GX_TEXCACHE_32K);
		GXInitTexCacheRegion(&gx->TexRegions1[i], GX_FALSE, GXTexRegionAddrTable[i + 16], GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 24],
		                     GX_TEXCACHE_32K);
		GXInitTexCacheRegion(&gx->TexRegions2[i], GX_TRUE, GXTexRegionAddrTable[i + 32], GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 40],
		                     GX_TEXCACHE_32K);
	}

	for (i = 0; i < GX_MAX_TLUT; i++) {
		GXInitTlutRegion(&gx->TlutRegions[i], 0xC0000 + 0x2000 * i, GX_TLUT_256);
	}

	for (i = 0; i < GX_MAX_BIGTLUT; i++) {
		GXInitTlutRegion(&gx->TlutRegions[i + 16], 0xE0000 + 0x8000 * i, GX_TLUT_1K);
	}

	GX_SET_CP_REG(3, 0);

	FAST_FLAG_SET(gx->perfSel, 0, 4, 4);

	GX_WRITE_U8(0x8);
	GX_WRITE_U8(0x20);
	GX_WRITE_U32(gx->perfSel);

	GX_WRITE_U8(0x10);
	GX_WRITE_U32(0x1006);
	GX_WRITE_U32(0);

	GX_WRITE_U8(0x61);
	GX_WRITE_U32(0x23000000);

	GX_WRITE_U8(0x61);
	GX_WRITE_U32(0x24000000);

	GX_WRITE_U8(0x61);
	GX_WRITE_U32(0x67000000);

	__GXSetIndirectMask(0);
	__GXSetTmemConfig(2);
	__GXInitGX();

	return &FifoObj;
}


void __GXInitGX(void)
{
	GXRenderModeObj* renderObj;
	GXTexObj texObj;
	Mtx ident;
	GXColor clearColor = { 64, 64, 64, 255 };
	GXColor ambColor   = { 0, 0, 0, 0 };
	GXColor matColor   = { 255, 255, 255, 255 };
	u32 i;

	switch (VIGetTvFormat()) {
	case VI_NTSC:
		renderObj = &GXNtsc480IntDf;
		break;

	case VI_PAL:
		renderObj = &GXPal528IntDf;
		break;

	case VI_EURGB60:
		renderObj = &GXEurgb60Hz480IntDf;
		break;

	case VI_MPAL:
		renderObj = &GXMpal480IntDf;
		break;

	default:
		renderObj = &GXNtsc480IntDf;
		break;
	}

	GXSetCopyClear(clearColor, 0xFFFFFF);

	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3X4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3X4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX3X4, GX_TG_TEX2, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX3X4, GX_TG_TEX3, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX3X4, GX_TG_TEX4, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD5, GX_TG_MTX3X4, GX_TG_TEX5, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD6, GX_TG_MTX3X4, GX_TG_TEX6, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
	GXSetTexCoordGen2(GX_TEXCOORD7, GX_TG_MTX3X4, GX_TG_TEX7, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);

	GXSetNumTexGens(1);
	GXClearVtxDesc();
	GXInvalidateVtxCache();

	for (i = GX_VA_POS; i <= GX_LIGHT_ARRAY; i++) {
		GXSetArray((GXAttr)i, gx, 0);
	}

	for (i = 0; i < GX_MAX_VTXFMT; i++) {
		GXSetVtxAttrFmtv((GXVtxFmt)i, GXDefaultVATList);
	}

	GXSetLineWidth(6, GX_TO_ZERO);
	GXSetPointSize(6, GX_TO_ZERO);
	GXEnableTexOffsets(GX_TEXCOORD0, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD1, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD2, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD3, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD4, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD5, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD6, GX_FALSE, GX_FALSE);
	GXEnableTexOffsets(GX_TEXCOORD7, GX_FALSE, GX_FALSE);

	ident[0][0] = 1.0f;
	ident[0][1] = 0.0f;
	ident[0][2] = 0.0f;
	ident[0][3] = 0.0f;

	ident[1][0] = 0.0f;
	ident[1][1] = 1.0f;
	ident[1][2] = 0.0f;
	ident[1][3] = 0.0f;

	ident[2][0] = 0.0f;
	ident[2][1] = 0.0f;
	ident[2][2] = 1.0f;
	ident[2][3] = 0.0f;

	GXLoadPosMtxImm(ident, GX_PNMTX0);
	GXLoadNrmMtxImm(ident, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);

	GXLoadTexMtxImm(ident, GX_IDENTITY, GX_MTX3x4);
	GXLoadTexMtxImm(ident, GX_PTIDENTITY, GX_MTX3x4);

	GXSetViewport(0.0f, 0.0f, renderObj->fbWidth, renderObj->xfbHeight, 0.0f, 1.0f);

	GXSetProjectionv(GXDefaultProjData);

	GXSetCoPlanar(GX_FALSE);
	GXSetCullMode(GX_CULL_BACK);
	GXSetClipMode(GX_CLIP_ENABLE);

	GXSetScissor(0, 0, renderObj->fbWidth, renderObj->efbHeight);
	GXSetScissorBoxOffset(0, 0);

	GXSetNumChans(0);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, ambColor);
	GXSetChanMatColor(GX_COLOR0A0, matColor);

	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR1A1, ambColor);
	GXSetChanMatColor(GX_COLOR1A1, matColor);

	GXInvalidateTexAll();
	GXSetTexRegionCallback(__GXDefaultTexRegionCallback);
	GXSetTlutRegionCallback(__GXDefaultTlutRegionCallback);

	GXInitTexObj(&texObj, DefaultTexData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GXLoadTexObj(&texObj, GX_TEXMAP0);
	GXLoadTexObj(&texObj, GX_TEXMAP1);
	GXLoadTexObj(&texObj, GX_TEXMAP2);
	GXLoadTexObj(&texObj, GX_TEXMAP3);
	GXLoadTexObj(&texObj, GX_TEXMAP4);
	GXLoadTexObj(&texObj, GX_TEXMAP5);
	GXLoadTexObj(&texObj, GX_TEXMAP6);
	GXLoadTexObj(&texObj, GX_TEXMAP7);

	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);

	GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);

	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);

	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);

	GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

	for (i = 0; i < GX_MAXTEVSTAGE; i++) {
		GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_4);
		GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1);
		GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
	}

	GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
	GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
	GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
	GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);

	for (i = 0; i < GX_MAXTEVSTAGE; i++) {
		GXSetTevDirect((GXTevStageID)i);
	}

	GXSetNumIndStages(0);
	GXSetIndTexCoordScale(GX_IND_TEX_STAGE_0, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_IND_TEX_STAGE_1, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_IND_TEX_STAGE_2, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_IND_TEX_STAGE_3, GX_ITS_1, GX_ITS_1);

	GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, ambColor);
	GXSetFogRangeAdj(GX_FALSE, 0, nullptr);

	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_TRUE);

	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetZCompLoc(GX_TRUE);

	GXSetDither(GX_TRUE);

	GXSetDstAlpha(GX_FALSE, 0);
	GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

	GXSetFieldMask(GX_TRUE, GX_TRUE);
	GXSetFieldMode((GXBool)renderObj->field_rendering, (GXBool)((renderObj->viHeight == 2 * renderObj->xfbHeight) ? GX_TRUE : GX_FALSE));

	GXSetDispCopySrc(0, 0, renderObj->fbWidth, renderObj->efbHeight);
	GXSetDispCopyDst(renderObj->fbWidth, renderObj->efbHeight);
	GXSetDispCopyYScale((f32)renderObj->xfbHeight / (f32)renderObj->efbHeight);
	GXSetCopyClamp(GX_CLAMP_BOTH);

	GXSetCopyFilter(renderObj->aa, renderObj->sample_pattern, GX_TRUE, renderObj->vfilter);
	GXSetDispCopyGamma(GX_GM_1_0);
	GXSetDispCopyFrame2Field(GX_COPY_PROGRESSIVE);
	GXClearBoundingBox();

	GXPokeColorUpdate(GX_TRUE);
	GXPokeAlphaUpdate(GX_TRUE);
	GXPokeDither(GX_FALSE);
	GXPokeBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ONE, GX_LO_SET);
	GXPokeAlphaMode(GX_ALWAYS, 0);
	GXPokeAlphaRead(GX_READ_FF);
	GXPokeDstAlpha(GX_FALSE, 0);
	GXPokeZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	GXSetGPMetric(GX_PERF0_NONE, GX_PERF1_NONE);
	GXClearGPMetric();
}
*/
