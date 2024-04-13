ScreenRestructured(ScreenPtr pScreen)
{
    GrabPtr grab;
    DeviceIntPtr pDev;

    for (pDev = inputInfo.devices; pDev; pDev = pDev->next) {
        if (!IsFloating(pDev) && !DevHasCursor(pDev))
            continue;

        /* GrabDevice doesn't have a confineTo field, so we don't need to
         * worry about it. */
        if ((grab = pDev->deviceGrab.grab) && grab->confineTo) {
            if (grab->confineTo->drawable.pScreen
                != pDev->spriteInfo->sprite->hotPhys.pScreen)
                pDev->spriteInfo->sprite->hotPhys.x =
                    pDev->spriteInfo->sprite->hotPhys.y = 0;
            ConfineCursorToWindow(pDev, grab->confineTo, TRUE, TRUE);
        }
        else
            ConfineCursorToWindow(pDev,
                                  pDev->spriteInfo->sprite->hotPhys.pScreen->
                                  root, TRUE, FALSE);
    }
}
