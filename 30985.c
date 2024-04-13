PostNewCursor(DeviceIntPtr pDev)
{
    WindowPtr win;
    GrabPtr grab = pDev->deviceGrab.grab;
    SpritePtr pSprite = pDev->spriteInfo->sprite;
    CursorPtr pCursor;

    if (syncEvents.playingEvents)
        return;
    if (grab) {
        if (grab->cursor) {
            ChangeToCursor(pDev, grab->cursor);
            return;
        }
        if (IsParent(grab->window, pSprite->win))
            win = pSprite->win;
        else
            win = grab->window;
    }
    else
        win = pSprite->win;
    for (; win; win = win->parent) {
        if (win->optional) {
            pCursor = WindowGetDeviceCursor(win, pDev);
            if (!pCursor && win->optional->cursor != NullCursor)
                pCursor = win->optional->cursor;
            if (pCursor) {
                ChangeToCursor(pDev, pCursor);
                return;
            }
        }
    }
}
