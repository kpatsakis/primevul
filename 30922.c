CheckVirtualMotion(DeviceIntPtr pDev, QdEventPtr qe, WindowPtr pWin)
{
    SpritePtr pSprite = pDev->spriteInfo->sprite;
    RegionPtr reg = NULL;
    DeviceEvent *ev = NULL;

    if (qe) {
        ev = &qe->event->device_event;
        switch (ev->type) {
        case ET_Motion:
        case ET_ButtonPress:
        case ET_ButtonRelease:
        case ET_KeyPress:
        case ET_KeyRelease:
        case ET_ProximityIn:
        case ET_ProximityOut:
            pSprite->hot.pScreen = qe->pScreen;
            pSprite->hot.x = ev->root_x;
            pSprite->hot.y = ev->root_y;
            pWin =
                pDev->deviceGrab.grab ? pDev->deviceGrab.grab->
                confineTo : NullWindow;
            break;
        default:
            break;
        }
    }
    if (pWin) {
        BoxRec lims;

#ifdef PANORAMIX
        if (!noPanoramiXExtension) {
            int x, y, off_x, off_y, i;

            if (!XineramaSetWindowPntrs(pDev, pWin))
                return;

            i = PanoramiXNumScreens - 1;

            RegionCopy(&pSprite->Reg2, &pSprite->windows[i]->borderSize);
            off_x = screenInfo.screens[i]->x;
            off_y = screenInfo.screens[i]->y;

            while (i--) {
                x = off_x - screenInfo.screens[i]->x;
                y = off_y - screenInfo.screens[i]->y;

                if (x || y)
                    RegionTranslate(&pSprite->Reg2, x, y);

                RegionUnion(&pSprite->Reg2, &pSprite->Reg2,
                            &pSprite->windows[i]->borderSize);

                off_x = screenInfo.screens[i]->x;
                off_y = screenInfo.screens[i]->y;
            }
        }
        else
#endif
        {
            if (pSprite->hot.pScreen != pWin->drawable.pScreen) {
                pSprite->hot.pScreen = pWin->drawable.pScreen;
                pSprite->hot.x = pSprite->hot.y = 0;
            }
        }

        lims = *RegionExtents(&pWin->borderSize);
        if (pSprite->hot.x < lims.x1)
            pSprite->hot.x = lims.x1;
        else if (pSprite->hot.x >= lims.x2)
            pSprite->hot.x = lims.x2 - 1;
        if (pSprite->hot.y < lims.y1)
            pSprite->hot.y = lims.y1;
        else if (pSprite->hot.y >= lims.y2)
            pSprite->hot.y = lims.y2 - 1;

#ifdef PANORAMIX
        if (!noPanoramiXExtension) {
            if (RegionNumRects(&pSprite->Reg2) > 1)
                reg = &pSprite->Reg2;

        }
        else
#endif
        {
            if (wBoundingShape(pWin))
                reg = &pWin->borderSize;
        }

        if (reg)
            ConfineToShape(pDev, reg, &pSprite->hot.x, &pSprite->hot.y);

        if (qe && ev) {
            qe->pScreen = pSprite->hot.pScreen;
            ev->root_x = pSprite->hot.x;
            ev->root_y = pSprite->hot.y;
        }
    }
#ifdef PANORAMIX
    if (noPanoramiXExtension)   /* No typo. Only set the root win if disabled */
#endif
        RootWindow(pDev->spriteInfo->sprite) = pSprite->hot.pScreen->root;
}
