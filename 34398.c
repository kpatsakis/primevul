int __glXDisp_GetVisualConfigs(__GLXclientState *cl, GLbyte *pc)
{
    xGLXGetVisualConfigsReq *req = (xGLXGetVisualConfigsReq *) pc;
    ClientPtr client = cl->client;
    xGLXGetVisualConfigsReply reply;
    __GLXscreen *pGlxScreen;
    __GLXconfig *modes;
    CARD32 buf[GLX_VIS_CONFIG_TOTAL];
    int p, i, err;
    __GLX_DECLARE_SWAP_VARIABLES;
    __GLX_DECLARE_SWAP_ARRAY_VARIABLES;

    if (!validGlxScreen(cl->client, req->screen, &pGlxScreen, &err))
	return err;

    reply.numVisuals = pGlxScreen->numVisuals;
    reply.numProps = GLX_VIS_CONFIG_TOTAL;
    reply.length = (reply.numVisuals * __GLX_SIZE_CARD32 * GLX_VIS_CONFIG_TOTAL) >> 2;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if (client->swapped) {
	__GLX_SWAP_SHORT(&reply.sequenceNumber);
	__GLX_SWAP_INT(&reply.length);
	__GLX_SWAP_INT(&reply.numVisuals);
	__GLX_SWAP_INT(&reply.numProps);
    }

    WriteToClient(client, sz_xGLXGetVisualConfigsReply, (char *)&reply);

    for (i = 0; i < pGlxScreen->numVisuals; i++) {
	modes = pGlxScreen->visuals[i];

	p = 0;
	buf[p++] = modes->visualID;
	buf[p++] = glxConvertToXVisualType( modes->visualType );
	buf[p++] = (modes->renderType & GLX_RGBA_BIT) ? GL_TRUE : GL_FALSE;

	buf[p++] = modes->redBits;
	buf[p++] = modes->greenBits;
	buf[p++] = modes->blueBits;
	buf[p++] = modes->alphaBits;
	buf[p++] = modes->accumRedBits;
	buf[p++] = modes->accumGreenBits;
	buf[p++] = modes->accumBlueBits;
	buf[p++] = modes->accumAlphaBits;

	buf[p++] = modes->doubleBufferMode;
	buf[p++] = modes->stereoMode;

	buf[p++] = modes->rgbBits;
	buf[p++] = modes->depthBits;
	buf[p++] = modes->stencilBits;
	buf[p++] = modes->numAuxBuffers;
	buf[p++] = modes->level;

	assert(p == GLX_VIS_CONFIG_UNPAIRED);
	/* 
	** Add token/value pairs for extensions.
	*/
	buf[p++] = GLX_VISUAL_CAVEAT_EXT;
	buf[p++] = modes->visualRating;
	buf[p++] = GLX_TRANSPARENT_TYPE;
	buf[p++] = modes->transparentPixel;
	buf[p++] = GLX_TRANSPARENT_RED_VALUE;
	buf[p++] = modes->transparentRed;
	buf[p++] = GLX_TRANSPARENT_GREEN_VALUE;
	buf[p++] = modes->transparentGreen;
	buf[p++] = GLX_TRANSPARENT_BLUE_VALUE;
	buf[p++] = modes->transparentBlue;
	buf[p++] = GLX_TRANSPARENT_ALPHA_VALUE;
	buf[p++] = modes->transparentAlpha;
	buf[p++] = GLX_TRANSPARENT_INDEX_VALUE;
	buf[p++] = modes->transparentIndex;
	buf[p++] = GLX_SAMPLES_SGIS;
	buf[p++] = modes->samples;
	buf[p++] = GLX_SAMPLE_BUFFERS_SGIS;
	buf[p++] = modes->sampleBuffers;
	buf[p++] = 0; /* copy over visualSelectGroup (GLX_VISUAL_SELECT_GROUP_SGIX)? */
	buf[p++] = 0;

	assert(p == GLX_VIS_CONFIG_TOTAL);
	if (client->swapped) {
	    __GLX_SWAP_INT_ARRAY(buf, p);
	}
	WriteToClient(client, __GLX_SIZE_CARD32 * p, (char *)buf);
    }
    return Success;
}
