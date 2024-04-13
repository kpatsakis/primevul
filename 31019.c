XineramaSetCursorPosition(DeviceIntPtr pDev, int x, int y, Bool generateEvent)
{
    ScreenPtr pScreen;
    int i;
    SpritePtr pSprite = pDev->spriteInfo->sprite;

    /* x,y are in Screen 0 coordinates.  We need to decide what Screen
       to send the message too and what the coordinates relative to
       that screen are. */

    pScreen = pSprite->screen;
    x += screenInfo.screens[0]->x;
    y += screenInfo.screens[0]->y;

    if (!point_on_screen(pScreen, x, y)) {
        FOR_NSCREENS(i) {
            if (i == pScreen->myNum)
                continue;
            if (point_on_screen(screenInfo.screens[i], x, y)) {
                pScreen = screenInfo.screens[i];
                break;
            }
        }
    }

    pSprite->screen = pScreen;
    pSprite->hotPhys.x = x - screenInfo.screens[0]->x;
    pSprite->hotPhys.y = y - screenInfo.screens[0]->y;
    x -= pScreen->x;
    y -= pScreen->y;

    return (*pScreen->SetCursorPosition) (pDev, pScreen, x, y, generateEvent);
}
