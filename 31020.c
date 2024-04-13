XineramaSetWindowPntrs(DeviceIntPtr pDev, WindowPtr pWin)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    if (pWin == screenInfo.screens[0]->root) {
        int i;

        FOR_NSCREENS(i)
            pSprite->windows[i] = screenInfo.screens[i]->root;
    }
    else {
        PanoramiXRes *win;
        int rc, i;

        rc = dixLookupResourceByType((void **) &win, pWin->drawable.id,
                                     XRT_WINDOW, serverClient, DixReadAccess);
        if (rc != Success)
            return FALSE;

        FOR_NSCREENS(i) {
            rc = dixLookupWindow(pSprite->windows + i, win->info[i].id,
                                 serverClient, DixReadAccess);
            if (rc != Success)  /* window is being unmapped */
                return FALSE;
        }
    }
    return TRUE;
}
