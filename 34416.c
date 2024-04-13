static void __glXdirectContextDestroy(__GLXcontext *context)
{
    __glXContextDestroy(context);
    free(context);
}
