validGlxDrawable(ClientPtr client, XID id, int type, int access_mode,
		 __GLXdrawable **drawable, int *err)
{
    int rc;

    rc = dixLookupResourceByType((pointer *) drawable, id,
				 __glXDrawableRes, client, access_mode);
    if (rc != Success && rc != BadValue) {
	*err = rc;
	client->errorValue = id;
	return FALSE;
    }

    /* If the ID of the glx drawable we looked up doesn't match the id
     * we looked for, it's because we looked it up under the X
     * drawable ID (see DoCreateGLXDrawable). */
    if (rc == BadValue ||
	(*drawable)->drawId != id ||
	(type != GLX_DRAWABLE_ANY && type != (*drawable)->type)) {
	client->errorValue = id;
	switch (type) {
	case GLX_DRAWABLE_WINDOW:
	    *err = __glXError(GLXBadWindow);
	    return FALSE;
	case GLX_DRAWABLE_PIXMAP:
	    *err = __glXError(GLXBadPixmap);
	    return FALSE;
	case GLX_DRAWABLE_PBUFFER:
	    *err = __glXError(GLXBadPbuffer);
	    return FALSE;
	case GLX_DRAWABLE_ANY:
	    *err = __glXError(GLXBadDrawable);
	    return FALSE;
	}
    }

    return TRUE;
}
