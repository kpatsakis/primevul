XineramaGetCursorScreen(DeviceIntPtr pDev)
{
    if (!noPanoramiXExtension) {
        return pDev->spriteInfo->sprite->screen->myNum;
    }
    else {
        return 0;
    }
}
