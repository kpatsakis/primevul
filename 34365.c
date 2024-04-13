DoCreateContext(__GLXclientState *cl, GLXContextID gcId,
		GLXContextID shareList, __GLXconfig *config,
		__GLXscreen *pGlxScreen, GLboolean isDirect)
{
    ClientPtr client = cl->client;
    __GLXcontext *glxc, *shareglxc;
    int err;
    
    LEGAL_NEW_RESOURCE(gcId, client);

    /*
    ** Find the display list space that we want to share.  
    **
    ** NOTE: In a multithreaded X server, we would need to keep a reference
    ** count for each display list so that if one client detroyed a list that 
    ** another client was using, the list would not really be freed until it 
    ** was no longer in use.  Since this sample implementation has no support 
    ** for multithreaded servers, we don't do this.  
    */
    if (shareList == None) {
	shareglxc = 0;
    } else {
	if (!validGlxContext(client, shareList, DixReadAccess,
			     &shareglxc, &err))
	    return err;

	if (shareglxc->isDirect) {
	    /*
	    ** NOTE: no support for sharing display lists between direct
	    ** contexts, even if they are in the same address space.
	    */
#if 0
            /* Disabling this code seems to allow shared display lists
             * and texture objects to work.  We'll leave it disabled for now.
             */
	    client->errorValue = shareList;
	    return BadMatch;
#endif
	} else {
	    /*
	    ** Create an indirect context regardless of what the client asked
	    ** for; this way we can share display list space with shareList.
	    */
	    isDirect = GL_FALSE;
	}
    }

    /*
    ** Allocate memory for the new context
    */
    if (!isDirect)
	glxc = pGlxScreen->createContext(pGlxScreen, config, shareglxc);
    else
	glxc = __glXdirectContextCreate(pGlxScreen, config, shareglxc);
    if (!glxc) {
	return BadAlloc;
    }

    /*
    ** Initially, setup the part of the context that could be used by
    ** a GL core that needs windowing information (e.g., Mesa).
    */
    glxc->pGlxScreen = pGlxScreen;
    glxc->config = config;

    /*
    ** Register this context as a resource.
    */
    if (!AddResource(gcId, __glXContextRes, (pointer)glxc)) {
	(*glxc->destroy)(glxc);
	client->errorValue = gcId;
	return BadAlloc;
    }
    
    /*
    ** Finally, now that everything is working, setup the rest of the
    ** context.
    */
    glxc->id = gcId;
    glxc->share_id = shareList;
    glxc->idExists = GL_TRUE;
    glxc->isCurrent = GL_FALSE;
    glxc->isDirect = isDirect;
    glxc->renderMode = GL_RENDER;

    __glXAddToContextList(glxc);

    return Success;
}
