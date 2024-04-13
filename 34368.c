static int DoDestroyDrawable(__GLXclientState *cl, XID glxdrawable, int type)
{
    __GLXdrawable *pGlxDraw;
    int err;

    if (!validGlxDrawable(cl->client, glxdrawable, type,
			  DixDestroyAccess, &pGlxDraw, &err))
	return err;

    FreeResource(glxdrawable, FALSE);

    return Success;
}
