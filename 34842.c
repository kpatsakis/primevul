AddGPUScreen(Bool (*pfnInit) (ScreenPtr /*pScreen */ ,
                              int /*argc */ ,
                              char **      /*argv */
                              ),
             int argc, char **argv)
{
    int i;
    ScreenPtr pScreen;
    Bool ret;

    i = screenInfo.numGPUScreens;
    if (i == MAXGPUSCREENS)
        return -1;

    pScreen = (ScreenPtr) calloc(1, sizeof(ScreenRec));
    if (!pScreen)
        return -1;

    ret = init_screen(pScreen, i, TRUE);
    if (ret != 0) {
        free(pScreen);
        return ret;
    }

    /* This is where screen specific stuff gets initialized.  Load the
       screen structure, call the hardware, whatever.
       This is also where the default colormap should be allocated and
       also pixel values for blackPixel, whitePixel, and the cursor
       Note that InitScreen is NOT allowed to modify argc, argv, or
       any of the strings pointed to by argv.  They may be passed to
       multiple screens.
     */
    screenInfo.gpuscreens[i] = pScreen;
    screenInfo.numGPUScreens++;
    if (!(*pfnInit) (pScreen, argc, argv)) {
        dixFreePrivates(pScreen->devPrivates, PRIVATE_SCREEN);
        free(pScreen);
        screenInfo.numGPUScreens--;
        return -1;
    }

    update_desktop_dimensions();

    /*
     * We cannot register the Screen PRIVATE_CURSOR key if cursors are already
     * created, because dix/privates.c does not have relocation code for
     * PRIVATE_CURSOR. Once this is fixed the if() can be removed and we can
     * register the Screen PRIVATE_CURSOR key unconditionally.
     */
    if (!dixPrivatesCreated(PRIVATE_CURSOR))
        dixRegisterScreenPrivateKey(&cursorScreenDevPriv, pScreen,
                                    PRIVATE_CURSOR, 0);

    return i;
}
