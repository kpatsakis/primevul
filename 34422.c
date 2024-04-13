validGlxVisual(ClientPtr client, __GLXscreen *pGlxScreen, XID id,
	       __GLXconfig **config, int *err)
{
    int i;

    for (i = 0; i < pGlxScreen->numVisuals; i++)
 	if (pGlxScreen->visuals[i]->visualID == id) {
	    *config = pGlxScreen->visuals[i];
	    return TRUE;
	}

    client->errorValue = id;
    *err = BadValue;

    return FALSE;
}
