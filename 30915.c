ChangeToCursor(DeviceIntPtr pDev, CursorPtr cursor)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;
    ScreenPtr pScreen;

    if (cursor != pSprite->current) {
        if ((pSprite->current->bits->xhot != cursor->bits->xhot) ||
            (pSprite->current->bits->yhot != cursor->bits->yhot))
            CheckPhysLimits(pDev, cursor, FALSE, pSprite->confined,
                            (ScreenPtr) NULL);
#ifdef PANORAMIX
        /* XXX: is this really necessary?? (whot) */
        if (!noPanoramiXExtension)
            pScreen = pSprite->screen;
        else
#endif
            pScreen = pSprite->hotPhys.pScreen;

        (*pScreen->DisplayCursor) (pDev, pScreen, cursor);
        FreeCursor(pSprite->current, (Cursor) 0);
        pSprite->current = RefCursor(cursor);
    }
}
