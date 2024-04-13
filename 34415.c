static __GLXcontext *__glXdirectContextCreate(__GLXscreen *screen,
					      __GLXconfig *modes,
					      __GLXcontext *shareContext)
{
    __GLXcontext *context;

    context = calloc(1, sizeof (__GLXcontext));
    if (context == NULL)
	return NULL;

    context->destroy = __glXdirectContextDestroy;

    return context;
}
