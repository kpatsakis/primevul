DoGetFBConfigs(__GLXclientState *cl, unsigned screen)
{
    ClientPtr client = cl->client;
    xGLXGetFBConfigsReply reply;
    __GLXscreen *pGlxScreen;
    CARD32 buf[__GLX_FBCONFIG_ATTRIBS_LENGTH];
    int p, err;
    __GLXconfig *modes;
    __GLX_DECLARE_SWAP_VARIABLES;
    __GLX_DECLARE_SWAP_ARRAY_VARIABLES;

    if (!validGlxScreen(cl->client, screen, &pGlxScreen, &err))
	return err;

    reply.numFBConfigs = pGlxScreen->numFBConfigs;
    reply.numAttribs = __GLX_TOTAL_FBCONFIG_ATTRIBS;
    reply.length = (__GLX_FBCONFIG_ATTRIBS_LENGTH * reply.numFBConfigs);
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if (client->swapped) {
	__GLX_SWAP_SHORT(&reply.sequenceNumber);
	__GLX_SWAP_INT(&reply.length);
	__GLX_SWAP_INT(&reply.numFBConfigs);
	__GLX_SWAP_INT(&reply.numAttribs);
    }

    WriteToClient(client, sz_xGLXGetFBConfigsReply, (char *)&reply);

    for (modes = pGlxScreen->fbconfigs; modes != NULL; modes = modes->next) {
	p = 0;

#define WRITE_PAIR(tag,value) \
    do { buf[p++] = tag ; buf[p++] = value ; } while( 0 )

	WRITE_PAIR( GLX_VISUAL_ID,        modes->visualID );
	WRITE_PAIR( GLX_FBCONFIG_ID,      modes->fbconfigID );
	WRITE_PAIR( GLX_X_RENDERABLE,     GL_TRUE );

	WRITE_PAIR( GLX_RGBA,
		    (modes->renderType & GLX_RGBA_BIT) ? GL_TRUE : GL_FALSE );
	WRITE_PAIR( GLX_RENDER_TYPE,      modes->renderType );
	WRITE_PAIR( GLX_DOUBLEBUFFER,     modes->doubleBufferMode );
	WRITE_PAIR( GLX_STEREO,           modes->stereoMode );

	WRITE_PAIR( GLX_BUFFER_SIZE,      modes->rgbBits );
	WRITE_PAIR( GLX_LEVEL,            modes->level );
	WRITE_PAIR( GLX_AUX_BUFFERS,      modes->numAuxBuffers );
	WRITE_PAIR( GLX_RED_SIZE,         modes->redBits );
	WRITE_PAIR( GLX_GREEN_SIZE,       modes->greenBits );
	WRITE_PAIR( GLX_BLUE_SIZE,        modes->blueBits );
	WRITE_PAIR( GLX_ALPHA_SIZE,       modes->alphaBits );
	WRITE_PAIR( GLX_ACCUM_RED_SIZE,   modes->accumRedBits );
	WRITE_PAIR( GLX_ACCUM_GREEN_SIZE, modes->accumGreenBits );
	WRITE_PAIR( GLX_ACCUM_BLUE_SIZE,  modes->accumBlueBits );
	WRITE_PAIR( GLX_ACCUM_ALPHA_SIZE, modes->accumAlphaBits );
	WRITE_PAIR( GLX_DEPTH_SIZE,       modes->depthBits );
	WRITE_PAIR( GLX_STENCIL_SIZE,     modes->stencilBits );
	WRITE_PAIR( GLX_X_VISUAL_TYPE,    modes->visualType );
	WRITE_PAIR( GLX_CONFIG_CAVEAT, modes->visualRating );
	WRITE_PAIR( GLX_TRANSPARENT_TYPE, modes->transparentPixel );
	WRITE_PAIR( GLX_TRANSPARENT_RED_VALUE, modes->transparentRed );
	WRITE_PAIR( GLX_TRANSPARENT_GREEN_VALUE, modes->transparentGreen );
	WRITE_PAIR( GLX_TRANSPARENT_BLUE_VALUE, modes->transparentBlue );
	WRITE_PAIR( GLX_TRANSPARENT_ALPHA_VALUE, modes->transparentAlpha );
	WRITE_PAIR( GLX_TRANSPARENT_INDEX_VALUE, modes->transparentIndex );
	WRITE_PAIR( GLX_SWAP_METHOD_OML, modes->swapMethod );
	WRITE_PAIR( GLX_SAMPLES_SGIS, modes->samples );
	WRITE_PAIR( GLX_SAMPLE_BUFFERS_SGIS, modes->sampleBuffers );
	/* GLX_VISUAL_SELECT_GROUP_SGIX ? */
	WRITE_PAIR( GLX_DRAWABLE_TYPE, modes->drawableType );
	WRITE_PAIR( GLX_BIND_TO_TEXTURE_RGB_EXT, modes->bindToTextureRgb );
	WRITE_PAIR( GLX_BIND_TO_TEXTURE_RGBA_EXT, modes->bindToTextureRgba );
	WRITE_PAIR( GLX_BIND_TO_MIPMAP_TEXTURE_EXT, modes->bindToMipmapTexture );
	WRITE_PAIR( GLX_BIND_TO_TEXTURE_TARGETS_EXT, modes->bindToTextureTargets );

	if (client->swapped) {
	    __GLX_SWAP_INT_ARRAY(buf, __GLX_FBCONFIG_ATTRIBS_LENGTH);
	}
	WriteToClient(client, __GLX_SIZE_CARD32 * __GLX_FBCONFIG_ATTRIBS_LENGTH,
		      (char *)buf);
    }
    return Success;
}
