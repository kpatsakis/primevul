__GLXcontext *__glXLookupContextByTag(__GLXclientState *cl, GLXContextTag tag)
{
    int num = cl->numCurrentContexts;

    if (tag < 1 || tag > num) {
	return 0;
    } else {
	return cl->currentContexts[tag-1];
    }
}
