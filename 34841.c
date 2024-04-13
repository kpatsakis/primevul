ProcDbeSwapBuffers(ClientPtr client)
{
    REQUEST(xDbeSwapBuffersReq);
    WindowPtr pWin;
    DbeScreenPrivPtr pDbeScreenPriv;
    DbeSwapInfoPtr swapInfo;
    xDbeSwapInfo *dbeSwapInfo;
    int error;
    unsigned int i, j;
    unsigned int nStuff;
    int nStuff_i;       /* DDX API requires int for nStuff */

    REQUEST_AT_LEAST_SIZE(xDbeSwapBuffersReq);
    nStuff = stuff->n;          /* use local variable for performance. */

    if (nStuff == 0) {
        REQUEST_SIZE_MATCH(xDbeSwapBuffersReq);
        return Success;
    }

    if (nStuff > UINT32_MAX / sizeof(DbeSwapInfoRec))
        return BadAlloc;
    REQUEST_FIXED_SIZE(xDbeSwapBuffersReq, nStuff * sizeof(xDbeSwapInfo));

    /* Get to the swap info appended to the end of the request. */
    dbeSwapInfo = (xDbeSwapInfo *) &stuff[1];

    /* Allocate array to record swap information. */
    swapInfo = xallocarray(nStuff, sizeof(DbeSwapInfoRec));
    if (swapInfo == NULL) {
        return BadAlloc;
    }

    for (i = 0; i < nStuff; i++) {
        /* Check all windows to swap. */

        /* Each window must be a valid window - BadWindow. */
        error = dixLookupWindow(&pWin, dbeSwapInfo[i].window, client,
                                DixWriteAccess);
        if (error != Success) {
            free(swapInfo);
            return error;
        }

        /* Each window must be double-buffered - BadMatch. */
        if (DBE_WINDOW_PRIV(pWin) == NULL) {
            free(swapInfo);
            return BadMatch;
        }

        /* Each window must only be specified once - BadMatch. */
        for (j = i + 1; j < nStuff; j++) {
            if (dbeSwapInfo[i].window == dbeSwapInfo[j].window) {
                free(swapInfo);
                return BadMatch;
            }
        }

        /* Each swap action must be valid - BadValue. */
        if ((dbeSwapInfo[i].swapAction != XdbeUndefined) &&
            (dbeSwapInfo[i].swapAction != XdbeBackground) &&
            (dbeSwapInfo[i].swapAction != XdbeUntouched) &&
            (dbeSwapInfo[i].swapAction != XdbeCopied)) {
            free(swapInfo);
            return BadValue;
        }

        /* Everything checks out OK.  Fill in the swap info array. */
        swapInfo[i].pWindow = pWin;
        swapInfo[i].swapAction = dbeSwapInfo[i].swapAction;

    }                           /* for (i = 0; i < nStuff; i++) */

    /* Call the DDX routine to perform the swap(s).  The DDX routine should
     * scan the swap list (swap info), swap any buffers that it knows how to
     * handle, delete them from the list, and update nStuff to indicate how
     * many windows it did not handle.
     *
     * This scheme allows a range of sophistication in the DDX SwapBuffers()
     * implementation.  Naive implementations could just swap the first buffer
     * in the list, move the last buffer to the front, decrement nStuff, and
     * return.  The next level of sophistication could be to scan the whole
     * list for windows on the same screen.  Up another level, the DDX routine
     * could deal with cross-screen synchronization.
     */

    nStuff_i = nStuff;
    while (nStuff_i > 0) {
        pDbeScreenPriv = DBE_SCREEN_PRIV_FROM_WINDOW(swapInfo[0].pWindow);
        error = (*pDbeScreenPriv->SwapBuffers) (client, &nStuff_i, swapInfo);
        if (error != Success) {
            free(swapInfo);
            return error;
        }
    }

    free(swapInfo);
    return Success;

}                               /* ProcDbeSwapBuffers() */
