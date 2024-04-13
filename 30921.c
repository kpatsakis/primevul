CheckPhysLimits(DeviceIntPtr pDev, CursorPtr cursor, Bool generateEvents,
                Bool confineToScreen, /* unused if PanoramiX on */
                ScreenPtr pScreen)    /* unused if PanoramiX on */
{
    HotSpot new;
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    if (!cursor)
        return;
    new = pSprite->hotPhys;
#ifdef PANORAMIX
    if (!noPanoramiXExtension)
        /* I don't care what the DDX has to say about it */
        pSprite->physLimits = pSprite->hotLimits;
    else
#endif
    {
        if (pScreen)
            new.pScreen = pScreen;
        else
            pScreen = new.pScreen;
        (*pScreen->CursorLimits) (pDev, pScreen, cursor, &pSprite->hotLimits,
                                  &pSprite->physLimits);
        pSprite->confined = confineToScreen;
        (*pScreen->ConstrainCursor) (pDev, pScreen, &pSprite->physLimits);
    }

    /* constrain the pointer to those limits */
    if (new.x < pSprite->physLimits.x1)
        new.x = pSprite->physLimits.x1;
    else if (new.x >= pSprite->physLimits.x2)
        new.x = pSprite->physLimits.x2 - 1;
    if (new.y < pSprite->physLimits.y1)
        new.y = pSprite->physLimits.y1;
    else if (new.y >= pSprite->physLimits.y2)
        new.y = pSprite->physLimits.y2 - 1;
    if (pSprite->hotShape)
        ConfineToShape(pDev, pSprite->hotShape, &new.x, &new.y);
    if ((
#ifdef PANORAMIX
            noPanoramiXExtension &&
#endif
            (pScreen != pSprite->hotPhys.pScreen)) ||
        (new.x != pSprite->hotPhys.x) || (new.y != pSprite->hotPhys.y)) {
#ifdef PANORAMIX
        if (!noPanoramiXExtension)
            XineramaSetCursorPosition(pDev, new.x, new.y, generateEvents);
        else
#endif
        {
            if (pScreen != pSprite->hotPhys.pScreen)
                pSprite->hotPhys = new;
            (*pScreen->SetCursorPosition)
                (pDev, pScreen, new.x, new.y, generateEvents);
        }
        if (!generateEvents)
            SyntheticMotion(pDev, new.x, new.y);
    }

#ifdef PANORAMIX
    /* Tell DDX what the limits are */
    if (!noPanoramiXExtension)
        XineramaConstrainCursor(pDev);
#endif
}
