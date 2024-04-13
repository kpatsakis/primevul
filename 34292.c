PanoramiXRenderReset (void)
{
    int	    i;
    for (i = 0; i < RenderNumberRequests; i++)
	ProcRenderVector[i] = PanoramiXSaveRenderVector[i];
    RenderErrBase = 0;
}
