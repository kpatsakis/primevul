WindowsRestructured(void)
{
    DeviceIntPtr pDev = inputInfo.devices;

    while (pDev) {
        if (IsMaster(pDev) || IsFloating(pDev))
            CheckMotion(NULL, pDev);
        pDev = pDev->next;
    }
}
