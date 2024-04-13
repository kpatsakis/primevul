PointInBorderSize(WindowPtr pWin, int x, int y)
{
    BoxRec box;

    if (RegionContainsPoint(&pWin->borderSize, x, y, &box))
        return TRUE;

#ifdef PANORAMIX
    if (!noPanoramiXExtension &&
        XineramaSetWindowPntrs(inputInfo.pointer, pWin)) {
        SpritePtr pSprite = inputInfo.pointer->spriteInfo->sprite;
        int i;

        FOR_NSCREENS_FORWARD_SKIP(i) {
            if (RegionContainsPoint(&pSprite->windows[i]->borderSize,
                                    x + screenInfo.screens[0]->x -
                                    screenInfo.screens[i]->x,
                                    y + screenInfo.screens[0]->y -
                                    screenInfo.screens[i]->y, &box))
                return TRUE;
        }
    }
#endif
    return FALSE;
}
