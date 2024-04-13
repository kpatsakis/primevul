AddScreen(Bool (*pfnInit) (ScreenPtr /*pScreen */ ,
                           int /*argc */ ,
                           char **      /*argv */
          ), int argc, char **argv)
{

    int i;
    ScreenPtr pScreen;
    Bool ret;

    i = screenInfo.numScreens;
    if (i == MAXSCREENS)
        return -1;

    pScreen = (ScreenPtr) calloc(1, sizeof(ScreenRec));
    if (!pScreen)
        return -1;

    ret = init_screen(pScreen, i, FALSE);
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
    screenInfo.screens[i] = pScreen;
    screenInfo.numScreens++;
    if (!(*pfnInit) (pScreen, argc, argv)) {
        dixFreeScreenSpecificPrivates(pScreen);
        dixFreePrivates(pScreen->devPrivates, PRIVATE_SCREEN);
        free(pScreen);
        screenInfo.numScreens--;
        return -1;
    }

    update_desktop_dimensions();

    dixRegisterScreenPrivateKey(&cursorScreenDevPriv, pScreen, PRIVATE_CURSOR,
                                0);

    return i;
}
