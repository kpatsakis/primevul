int __glXDisp_CopyContext(__GLXclientState *cl, GLbyte *pc)
{
    ClientPtr client = cl->client;
    xGLXCopyContextReq *req = (xGLXCopyContextReq *) pc;
    GLXContextID source = req->source;
    GLXContextID dest = req->dest;
    GLXContextTag tag = req->contextTag;
    unsigned long mask = req->mask;
    __GLXcontext *src, *dst;
    int error;

    if (!validGlxContext(cl->client, source, DixReadAccess, &src, &error))
	return error;
    if (!validGlxContext(cl->client, dest, DixWriteAccess, &dst, &error))
	return error;

    /*
    ** They must be in the same address space, and same screen.
    ** NOTE: no support for direct rendering contexts here.
    */
    if (src->isDirect || dst->isDirect ||
	(src->pGlxScreen != dst->pGlxScreen)) {
	client->errorValue = source;
	return BadMatch;
    }

    /*
    ** The destination context must not be current for any client.
    */
    if (dst->isCurrent) {
	client->errorValue = dest;
	return BadAccess;
    }

    if (tag) {
	__GLXcontext *tagcx = __glXLookupContextByTag(cl, tag);
	
	if (!tagcx) {
	    return __glXError(GLXBadContextTag);
	}
	if (tagcx != src) {
	    /*
	    ** This would be caused by a faulty implementation of the client
	    ** library.
	    */
	    return BadMatch;
	}
	/*
	** In this case, glXCopyContext is in both GL and X streams, in terms
	** of sequentiality.
	*/
	if (__glXForceCurrent(cl, tag, &error)) {
	    /*
	    ** Do whatever is needed to make sure that all preceding requests
	    ** in both streams are completed before the copy is executed.
	    */
	    CALL_Finish( GET_DISPATCH(), () );
	    __GLX_NOTE_FLUSHED_CMDS(tagcx);
	} else {
	    return error;
	}
    }
    /*
    ** Issue copy.  The only reason for failure is a bad mask.
    */
    if (!(*dst->copy)(dst, src, mask)) {
	client->errorValue = mask;
	return BadValue;
    }
    return Success;
}
