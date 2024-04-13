RemoveGPUScreen(ScreenPtr pScreen)
{
    int idx, j;
    if (!pScreen->isGPU)
        return;

    idx = pScreen->myNum - GPU_SCREEN_OFFSET;
    for (j = idx; j < screenInfo.numGPUScreens - 1; j++) {
        screenInfo.gpuscreens[j] = screenInfo.gpuscreens[j + 1];
        screenInfo.gpuscreens[j]->myNum = j + GPU_SCREEN_OFFSET;
    }
    screenInfo.numGPUScreens--;

    /* this gets freed later in the resource list, but without
     * the screen existing it causes crashes - so remove it here */
    if (pScreen->defColormap)
        FreeResource(pScreen->defColormap, RT_COLORMAP);
    free(pScreen);

}
