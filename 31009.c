UpdateSpriteForScreen(DeviceIntPtr pDev, ScreenPtr pScreen)
{
    SpritePtr pSprite = NULL;
    WindowPtr win = NULL;
    CursorPtr pCursor;

    if (!pScreen)
        return;

    if (!pDev->spriteInfo->sprite)
        return;

    pSprite = pDev->spriteInfo->sprite;

    win = pScreen->root;

    pSprite->hotPhys.pScreen = pScreen;
    pSprite->hot = pSprite->hotPhys;
    pSprite->hotLimits.x2 = pScreen->width;
    pSprite->hotLimits.y2 = pScreen->height;
    pSprite->win = win;
    pCursor = RefCursor(wCursor(win));
    if (pSprite->current)
        FreeCursor(pSprite->current, 0);
    pSprite->current = pCursor;
    pSprite->spriteTraceGood = 1;
    pSprite->spriteTrace[0] = win;
    (*pScreen->CursorLimits) (pDev,
                              pScreen,
                              pSprite->current,
                              &pSprite->hotLimits, &pSprite->physLimits);
    pSprite->confined = FALSE;
    (*pScreen->ConstrainCursor) (pDev, pScreen, &pSprite->physLimits);
    (*pScreen->DisplayCursor) (pDev, pScreen, pSprite->current);

#ifdef PANORAMIX
    if (!noPanoramiXExtension) {
        pSprite->hotLimits.x1 = -screenInfo.screens[0]->x;
        pSprite->hotLimits.y1 = -screenInfo.screens[0]->y;
        pSprite->hotLimits.x2 = PanoramiXPixWidth - screenInfo.screens[0]->x;
        pSprite->hotLimits.y2 = PanoramiXPixHeight - screenInfo.screens[0]->y;
        pSprite->physLimits = pSprite->hotLimits;
        pSprite->screen = pScreen;
    }
#endif
}
