CheckCursorConfinement(WindowPtr pWin)
{
    GrabPtr grab;
    WindowPtr confineTo;
    DeviceIntPtr pDev;

#ifdef PANORAMIX
    if (!noPanoramiXExtension && pWin->drawable.pScreen->myNum)
        return;
#endif

    for (pDev = inputInfo.devices; pDev; pDev = pDev->next) {
        if (DevHasCursor(pDev)) {
            grab = pDev->deviceGrab.grab;
            if (grab && (confineTo = grab->confineTo)) {
                if (!BorderSizeNotEmpty(pDev, confineTo))
                    (*pDev->deviceGrab.DeactivateGrab) (pDev);
                else if ((pWin == confineTo) || IsParent(pWin, confineTo))
                    ConfineCursorToWindow(pDev, confineTo, TRUE, TRUE);
            }
        }
    }
}
