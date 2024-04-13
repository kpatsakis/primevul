XineramaConfineCursorToWindow(DeviceIntPtr pDev,
                              WindowPtr pWin, Bool generateEvents)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    int x, y, off_x, off_y, i;

    if (!XineramaSetWindowPntrs(pDev, pWin))
        return;

    i = PanoramiXNumScreens - 1;

    RegionCopy(&pSprite->Reg1, &pSprite->windows[i]->borderSize);
    off_x = screenInfo.screens[i]->x;
    off_y = screenInfo.screens[i]->y;

    while (i--) {
        x = off_x - screenInfo.screens[i]->x;
        y = off_y - screenInfo.screens[i]->y;

        if (x || y)
            RegionTranslate(&pSprite->Reg1, x, y);

        RegionUnion(&pSprite->Reg1, &pSprite->Reg1,
                    &pSprite->windows[i]->borderSize);

        off_x = screenInfo.screens[i]->x;
        off_y = screenInfo.screens[i]->y;
    }

    pSprite->hotLimits = *RegionExtents(&pSprite->Reg1);

    if (RegionNumRects(&pSprite->Reg1) > 1)
        pSprite->hotShape = &pSprite->Reg1;
    else
        pSprite->hotShape = NullRegion;

    pSprite->confined = FALSE;
    pSprite->confineWin =
        (pWin == screenInfo.screens[0]->root) ? NullWindow : pWin;

    CheckPhysLimits(pDev, pSprite->current, generateEvents, FALSE, NULL);
}
