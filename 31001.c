ReinitializeRootWindow(WindowPtr win, int xoff, int yoff)
{
    GrabPtr grab;
    DeviceIntPtr pDev;
    SpritePtr pSprite;

    if (noPanoramiXExtension)
        return;

    pDev = inputInfo.devices;
    while (pDev) {
        if (DevHasCursor(pDev)) {
            pSprite = pDev->spriteInfo->sprite;
            pSprite->hot.x -= xoff;
            pSprite->hot.y -= yoff;

            pSprite->hotPhys.x -= xoff;
            pSprite->hotPhys.y -= yoff;

            pSprite->hotLimits.x1 -= xoff;
            pSprite->hotLimits.y1 -= yoff;
            pSprite->hotLimits.x2 -= xoff;
            pSprite->hotLimits.y2 -= yoff;

            if (RegionNotEmpty(&pSprite->Reg1))
                RegionTranslate(&pSprite->Reg1, xoff, yoff);
            if (RegionNotEmpty(&pSprite->Reg2))
                RegionTranslate(&pSprite->Reg2, xoff, yoff);

            /* FIXME: if we call ConfineCursorToWindow, must we do anything else? */
            if ((grab = pDev->deviceGrab.grab) && grab->confineTo) {
                if (grab->confineTo->drawable.pScreen
                    != pSprite->hotPhys.pScreen)
                    pSprite->hotPhys.x = pSprite->hotPhys.y = 0;
                ConfineCursorToWindow(pDev, grab->confineTo, TRUE, TRUE);
            }
            else
                ConfineCursorToWindow(pDev,
                                      pSprite->hotPhys.pScreen->root,
                                      TRUE, FALSE);

        }
        pDev = pDev->next;
    }
}
