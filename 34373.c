static void StartUsingContext(__GLXclientState *cl, __GLXcontext *glxc)
{
    glxc->isCurrent = GL_TRUE;
    __glXLastContext = glxc;	
}
