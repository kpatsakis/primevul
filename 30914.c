BorderSizeNotEmpty(DeviceIntPtr pDev, WindowPtr pWin)
{
    if (RegionNotEmpty(&pWin->borderSize))
        return TRUE;

#ifdef PANORAMIX
    if (!noPanoramiXExtension && XineramaSetWindowPntrs(pDev, pWin)) {
        int i;

        FOR_NSCREENS_FORWARD_SKIP(i) {
            if (RegionNotEmpty
                (&pDev->spriteInfo->sprite->windows[i]->borderSize))
                return TRUE;
        }
    }
#endif
    return FALSE;
}
