XineramaConstrainCursor(DeviceIntPtr pDev)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;
    ScreenPtr pScreen;
    BoxRec newBox;

    pScreen = pSprite->screen;
    newBox = pSprite->physLimits;

    /* Translate the constraining box to the screen
       the sprite is actually on */
    newBox.x1 += screenInfo.screens[0]->x - pScreen->x;
    newBox.x2 += screenInfo.screens[0]->x - pScreen->x;
    newBox.y1 += screenInfo.screens[0]->y - pScreen->y;
    newBox.y2 += screenInfo.screens[0]->y - pScreen->y;

    (*pScreen->ConstrainCursor) (pDev, pScreen, &newBox);
}
