static void ChangeCurrentContext(__GLXclientState *cl, __GLXcontext *glxc,
				GLXContextTag tag)
{
    __GLXcontext **table = cl->currentContexts;
    table[tag-1] = glxc;
}
