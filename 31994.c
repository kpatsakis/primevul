XFixesFetchRegion (Display *dpy, XserverRegion region, int *nrectanglesRet)
{
    XRectangle	bounds;

    return XFixesFetchRegionAndBounds (dpy, region, nrectanglesRet, &bounds);
}
