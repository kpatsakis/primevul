ConfineCursorToWindow(DeviceIntPtr pDev, WindowPtr pWin, Bool generateEvents,
                      Bool confineToScreen)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    if (syncEvents.playingEvents) {
        CheckVirtualMotion(pDev, (QdEventPtr) NULL, pWin);
        SyntheticMotion(pDev, pSprite->hot.x, pSprite->hot.y);
    }
    else {
        ScreenPtr pScreen = pWin->drawable.pScreen;

#ifdef PANORAMIX
        if (!noPanoramiXExtension) {
            XineramaConfineCursorToWindow(pDev, pWin, generateEvents);
            return;
        }
#endif
        pSprite->hotLimits = *RegionExtents(&pWin->borderSize);
        pSprite->hotShape = wBoundingShape(pWin) ? &pWin->borderSize
            : NullRegion;
        CheckPhysLimits(pDev, pSprite->current, generateEvents,
                        confineToScreen, pWin->drawable.pScreen);

        if (*pScreen->CursorConfinedTo)
            (*pScreen->CursorConfinedTo) (pDev, pScreen, pWin);
    }
}
