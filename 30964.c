InitializeSprite(DeviceIntPtr pDev, WindowPtr pWin)
{
    SpritePtr pSprite;
    ScreenPtr pScreen;
    CursorPtr pCursor;

    if (!pDev->spriteInfo->sprite) {
        DeviceIntPtr it;

        pDev->spriteInfo->sprite = (SpritePtr) calloc(1, sizeof(SpriteRec));
        if (!pDev->spriteInfo->sprite)
            FatalError("InitializeSprite: failed to allocate sprite struct");

        /* We may have paired another device with this device before our
         * device had a actual sprite. We need to check for this and reset the
         * sprite field for all paired devices.
         *
         * The VCK is always paired with the VCP before the VCP has a sprite.
         */
        for (it = inputInfo.devices; it; it = it->next) {
            if (it->spriteInfo->paired == pDev)
                it->spriteInfo->sprite = pDev->spriteInfo->sprite;
        }
        if (inputInfo.keyboard->spriteInfo->paired == pDev)
            inputInfo.keyboard->spriteInfo->sprite = pDev->spriteInfo->sprite;
    }

    pSprite = pDev->spriteInfo->sprite;
    pDev->spriteInfo->spriteOwner = TRUE;

    pScreen = (pWin) ? pWin->drawable.pScreen : (ScreenPtr) NULL;
    pSprite->hot.pScreen = pScreen;
    pSprite->hotPhys.pScreen = pScreen;
    if (pScreen) {
        pSprite->hotPhys.x = pScreen->width / 2;
        pSprite->hotPhys.y = pScreen->height / 2;
        pSprite->hotLimits.x2 = pScreen->width;
        pSprite->hotLimits.y2 = pScreen->height;
    }

    pSprite->hot = pSprite->hotPhys;
    pSprite->win = pWin;

    if (pWin) {
        pCursor = wCursor(pWin);
        pSprite->spriteTrace = (WindowPtr *) calloc(1, 32 * sizeof(WindowPtr));
        if (!pSprite->spriteTrace)
            FatalError("Failed to allocate spriteTrace");
        pSprite->spriteTraceSize = 32;

        RootWindow(pDev->spriteInfo->sprite) = pWin;
        pSprite->spriteTraceGood = 1;

        pSprite->pEnqueueScreen = pScreen;
        pSprite->pDequeueScreen = pSprite->pEnqueueScreen;

    }
    else {
        pCursor = NullCursor;
        pSprite->spriteTrace = NULL;
        pSprite->spriteTraceSize = 0;
        pSprite->spriteTraceGood = 0;
        pSprite->pEnqueueScreen = screenInfo.screens[0];
        pSprite->pDequeueScreen = pSprite->pEnqueueScreen;
    }
    pCursor = RefCursor(pCursor);
    if (pSprite->current)
        FreeCursor(pSprite->current, None);
    pSprite->current = pCursor;

    if (pScreen) {
        (*pScreen->RealizeCursor) (pDev, pScreen, pSprite->current);
        (*pScreen->CursorLimits) (pDev, pScreen, pSprite->current,
                                  &pSprite->hotLimits, &pSprite->physLimits);
        pSprite->confined = FALSE;

        (*pScreen->ConstrainCursor) (pDev, pScreen, &pSprite->physLimits);
        (*pScreen->SetCursorPosition) (pDev, pScreen, pSprite->hot.x,
                                       pSprite->hot.y, FALSE);
        (*pScreen->DisplayCursor) (pDev, pScreen, pSprite->current);
    }
#ifdef PANORAMIX
    if (!noPanoramiXExtension) {
        pSprite->hotLimits.x1 = -screenInfo.screens[0]->x;
        pSprite->hotLimits.y1 = -screenInfo.screens[0]->y;
        pSprite->hotLimits.x2 = PanoramiXPixWidth - screenInfo.screens[0]->x;
        pSprite->hotLimits.y2 = PanoramiXPixHeight - screenInfo.screens[0]->y;
        pSprite->physLimits = pSprite->hotLimits;
        pSprite->confineWin = NullWindow;
        pSprite->hotShape = NullRegion;
        pSprite->screen = pScreen;
        /* gotta UNINIT these someplace */
        RegionNull(&pSprite->Reg1);
        RegionNull(&pSprite->Reg2);
    }
#endif
}
