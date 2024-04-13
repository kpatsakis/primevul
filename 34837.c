DbeStubScreen(DbeScreenPrivPtr pDbeScreenPriv, int *nStubbedScreens)
{
    /* Stub DIX. */
    pDbeScreenPriv->SetupBackgroundPainter = NULL;

    /* Do not unwrap PositionWindow nor DestroyWindow.  If the DDX
     * initialization function failed, we assume that it did not wrap
     * PositionWindow.  Also, DestroyWindow is only wrapped if the DDX
     * initialization function succeeded.
     */

    /* Stub DDX. */
    pDbeScreenPriv->GetVisualInfo = NULL;
    pDbeScreenPriv->AllocBackBufferName = NULL;
    pDbeScreenPriv->SwapBuffers = NULL;
    pDbeScreenPriv->WinPrivDelete = NULL;

    (*nStubbedScreens)++;

}                               /* DbeStubScreen() */
