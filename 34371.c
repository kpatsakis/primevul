DoMakeCurrent(__GLXclientState *cl,
	      GLXDrawable drawId, GLXDrawable readId,
	      GLXContextID contextId, GLXContextTag tag)
{
    ClientPtr client = cl->client;
    xGLXMakeCurrentReply reply;
    __GLXcontext *glxc, *prevglxc;
    __GLXdrawable *drawPriv = NULL;
    __GLXdrawable *readPriv = NULL;
    int error;
    GLuint  mask;

    /*
    ** If one is None and the other isn't, it's a bad match.
    */

    mask  = (drawId == None)    ? (1 << 0) : 0;
    mask |= (readId == None)    ? (1 << 1) : 0;
    mask |= (contextId == None) ? (1 << 2) : 0;

    if ( (mask != 0x00) && (mask != 0x07) ) {
	return BadMatch;
    }
    
    /*
    ** Lookup old context.  If we have one, it must be in a usable state.
    */
    if (tag != 0) {
	prevglxc = __glXLookupContextByTag(cl, tag);
	if (!prevglxc) {
	    /*
	    ** Tag for previous context is invalid.
	    */
	    return __glXError(GLXBadContextTag);
	}
	if (prevglxc->renderMode != GL_RENDER) {
	    /* Oops.  Not in render mode render. */
	    client->errorValue = prevglxc->id;
	    return __glXError(GLXBadContextState);
	}
    } else {
	prevglxc = 0;
    }

    /*
    ** Lookup new context.  It must not be current for someone else.
    */
    if (contextId != None) {
	int  status;

	if (!validGlxContext(client, contextId, DixUseAccess, &glxc, &error))
	    return error;
	if ((glxc != prevglxc) && glxc->isCurrent) {
	    /* Context is current to somebody else */
	    return BadAccess;
	}

	assert( drawId != None );
	assert( readId != None );

	drawPriv = __glXGetDrawable(glxc, drawId, client, &status);
	if (drawPriv == NULL)
	    return status;

	readPriv = __glXGetDrawable(glxc, readId, client, &status);
	if (readPriv == NULL)
	    return status;

    } else {
	/* Switching to no context.  Ignore new drawable. */
	glxc = 0;
	drawPriv = 0;
	readPriv = 0;
    }


    if (prevglxc) {
	/*
	** Flush the previous context if needed.
	*/
	if (__GLX_HAS_UNFLUSHED_CMDS(prevglxc)) {
	    if (__glXForceCurrent(cl, tag, (int *)&error)) {
		CALL_Flush( GET_DISPATCH(), () );
		__GLX_NOTE_FLUSHED_CMDS(prevglxc);
	    } else {
		return error;
	    }
	}

	/*
	** Make the previous context not current.
	*/
	if (!(*prevglxc->loseCurrent)(prevglxc)) {
	    return __glXError(GLXBadContext);
	}
	__glXFlushContextCache();
	if (!prevglxc->isDirect) {
	    prevglxc->drawPriv = NULL;
	    prevglxc->readPriv = NULL;
	}
    }
	

    if ((glxc != 0) && !glxc->isDirect) {

	glxc->drawPriv = drawPriv;
	glxc->readPriv = readPriv;

	/* make the context current */
	if (!(*glxc->makeCurrent)(glxc)) {
	    glxc->drawPriv = NULL;
	    glxc->readPriv = NULL;
	    return __glXError(GLXBadContext);
	}

	glxc->isCurrent = GL_TRUE;
    }

    if (prevglxc) {
	ChangeCurrentContext(cl, glxc, tag);
	StopUsingContext(prevglxc);
    } else {
	tag = AddCurrentContext(cl, glxc);
    }

    if (glxc) {
	StartUsingContext(cl, glxc);
	reply.contextTag = tag;
    } else {
	reply.contextTag = 0;
    }

    reply.length = 0;
    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if (client->swapped) {
	__glXSwapMakeCurrentReply(client, &reply);
    } else {
	WriteToClient(client, sz_xGLXMakeCurrentReply, (char *)&reply);
    }
    return Success;
}
