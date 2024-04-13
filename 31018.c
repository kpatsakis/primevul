XineramaPointInWindowIsVisible(WindowPtr pWin, int x, int y)
{
    BoxRec box;
    int i, xoff, yoff;

    if (!pWin->realized)
        return FALSE;

    if (RegionContainsPoint(&pWin->borderClip, x, y, &box))
        return TRUE;

    if (!XineramaSetWindowPntrs(inputInfo.pointer, pWin))
         return FALSE;

    xoff = x + screenInfo.screens[0]->x;
    yoff = y + screenInfo.screens[0]->y;

    FOR_NSCREENS_FORWARD_SKIP(i) {
        pWin = inputInfo.pointer->spriteInfo->sprite->windows[i];

        x = xoff - screenInfo.screens[i]->x;
        y = yoff - screenInfo.screens[i]->y;

        if (RegionContainsPoint(&pWin->borderClip, x, y, &box)
            && (!wInputShape(pWin) ||
                RegionContainsPoint(wInputShape(pWin),
                                    x - pWin->drawable.x,
                                    y - pWin->drawable.y, &box)))
            return TRUE;

    }

    return FALSE;
}
