findVisual (ScreenPtr pScreen, VisualID vid)
{
    VisualPtr	pVisual;
    int		v;

    for (v = 0; v < pScreen->numVisuals; v++)
    {
	pVisual = pScreen->visuals + v;
	if (pVisual->vid == vid)
	    return pVisual;
    }
    return 0;
}
