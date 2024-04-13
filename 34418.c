validGlxContext(ClientPtr client, XID id, int access_mode,
		__GLXcontext **context, int *err)
{
    *err = dixLookupResourceByType((pointer *) context, id,
				   __glXContextRes, client, access_mode);
    if (*err != Success) {
	client->errorValue = id;
	if (*err == BadValue)
	    *err = __glXError(GLXBadContext);
	return FALSE;
    }

    return TRUE;
}
