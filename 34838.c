ProcDbeAllocateBackBufferName(ClientPtr client)
{
    REQUEST(xDbeAllocateBackBufferNameReq);
    WindowPtr pWin;
    DbeScreenPrivPtr pDbeScreenPriv;
    DbeWindowPrivPtr pDbeWindowPriv;
    XdbeScreenVisualInfo scrVisInfo;
    register int i;
    Bool visualMatched = FALSE;
    xDbeSwapAction swapAction;
    VisualID visual;
    int status;
    int add_index;

    REQUEST_SIZE_MATCH(xDbeAllocateBackBufferNameReq);

    /* The window must be valid. */
    status = dixLookupWindow(&pWin, stuff->window, client, DixManageAccess);
    if (status != Success)
        return status;

    /* The window must be InputOutput. */
    if (pWin->drawable.class != InputOutput) {
        return BadMatch;
    }

    /* The swap action must be valid. */
    swapAction = stuff->swapAction;     /* use local var for performance. */
    if ((swapAction != XdbeUndefined) &&
        (swapAction != XdbeBackground) &&
        (swapAction != XdbeUntouched) && (swapAction != XdbeCopied)) {
        return BadValue;
    }

    /* The id must be in range and not already in use. */
    LEGAL_NEW_RESOURCE(stuff->buffer, client);

    /* The visual of the window must be in the list returned by
     * GetVisualInfo.
     */
    pDbeScreenPriv = DBE_SCREEN_PRIV_FROM_WINDOW(pWin);
    if (!pDbeScreenPriv->GetVisualInfo)
        return BadMatch;        /* screen doesn't support double buffering */

    if (!(*pDbeScreenPriv->GetVisualInfo) (pWin->drawable.pScreen, &scrVisInfo)) {
        /* GetVisualInfo() failed to allocate visual info data. */
        return BadAlloc;
    }

    /* See if the window's visual is on the list. */
    visual = wVisual(pWin);
    for (i = 0; (i < scrVisInfo.count) && !visualMatched; i++) {
        if (scrVisInfo.visinfo[i].visual == visual) {
            visualMatched = TRUE;
        }
    }

    /* Free what was allocated by the GetVisualInfo() call above. */
    free(scrVisInfo.visinfo);

    if (!visualMatched) {
        return BadMatch;
    }

    if ((pDbeWindowPriv = DBE_WINDOW_PRIV(pWin)) == NULL) {
        /* There is no buffer associated with the window.
         * Allocate a window priv.
         */

        pDbeWindowPriv = calloc(1, sizeof(DbeWindowPrivRec));
        if (!pDbeWindowPriv)
            return BadAlloc;

        /* Fill out window priv information. */
        pDbeWindowPriv->pWindow = pWin;
        pDbeWindowPriv->width = pWin->drawable.width;
        pDbeWindowPriv->height = pWin->drawable.height;
        pDbeWindowPriv->x = pWin->drawable.x;
        pDbeWindowPriv->y = pWin->drawable.y;
        pDbeWindowPriv->nBufferIDs = 0;

        /* Set the buffer ID array pointer to the initial (static) array). */
        pDbeWindowPriv->IDs = pDbeWindowPriv->initIDs;

        /* Initialize the buffer ID list. */
        pDbeWindowPriv->maxAvailableIDs = DBE_INIT_MAX_IDS;
        pDbeWindowPriv->IDs[0] = stuff->buffer;

        add_index = 0;
        for (i = 0; i < DBE_INIT_MAX_IDS; i++) {
            pDbeWindowPriv->IDs[i] = DBE_FREE_ID_ELEMENT;
        }

        /* Actually connect the window priv to the window. */
        dixSetPrivate(&pWin->devPrivates, dbeWindowPrivKey, pDbeWindowPriv);

    }                           /* if -- There is no buffer associated with the window. */

    else {
        /* A buffer is already associated with the window.
         * Add the new buffer ID to the array, reallocating the array memory
         * if necessary.
         */

        /* Determine if there is a free element in the ID array. */
        for (i = 0; i < pDbeWindowPriv->maxAvailableIDs; i++) {
            if (pDbeWindowPriv->IDs[i] == DBE_FREE_ID_ELEMENT) {
                /* There is still room in the ID array. */
                break;
            }
        }

        if (i == pDbeWindowPriv->maxAvailableIDs) {
            /* No more room in the ID array -- reallocate another array. */
            XID *pIDs;

            /* Setup an array pointer for the realloc operation below. */
            if (pDbeWindowPriv->maxAvailableIDs == DBE_INIT_MAX_IDS) {
                /* We will malloc a new array. */
                pIDs = NULL;
            }
            else {
                /* We will realloc a new array. */
                pIDs = pDbeWindowPriv->IDs;
            }

            /* malloc/realloc a new array and initialize all elements to 0. */
            pDbeWindowPriv->IDs =
                reallocarray(pIDs,
                             pDbeWindowPriv->maxAvailableIDs + DBE_INCR_MAX_IDS,
                             sizeof(XID));
            if (!pDbeWindowPriv->IDs) {
                return BadAlloc;
            }
            memset(&pDbeWindowPriv->IDs[pDbeWindowPriv->nBufferIDs], 0,
                   (pDbeWindowPriv->maxAvailableIDs + DBE_INCR_MAX_IDS -
                    pDbeWindowPriv->nBufferIDs) * sizeof(XID));

            if (pDbeWindowPriv->maxAvailableIDs == DBE_INIT_MAX_IDS) {
                /* We just went from using the initial (static) array to a
                 * newly allocated array.  Copy the IDs from the initial array
                 * to the new array.
                 */
                memcpy(pDbeWindowPriv->IDs, pDbeWindowPriv->initIDs,
                       DBE_INIT_MAX_IDS * sizeof(XID));
            }

            pDbeWindowPriv->maxAvailableIDs += DBE_INCR_MAX_IDS;
        }

        add_index = i;

    }                           /* else -- A buffer is already associated with the window. */

    /* Call the DDX routine to allocate the back buffer. */
    status = (*pDbeScreenPriv->AllocBackBufferName) (pWin, stuff->buffer,
                                                     stuff->swapAction);

    if (status == Success) {
        pDbeWindowPriv->IDs[add_index] = stuff->buffer;
        if (!AddResource(stuff->buffer, dbeWindowPrivResType,
                         (void *) pDbeWindowPriv)) {
            pDbeWindowPriv->IDs[add_index] = DBE_FREE_ID_ELEMENT;

            if (pDbeWindowPriv->nBufferIDs == 0) {
                status = BadAlloc;
                goto out_free;
            }
        }
    }
    else {
        /* The DDX buffer allocation routine failed for the first buffer of
         * this window.
         */
        if (pDbeWindowPriv->nBufferIDs == 0) {
            goto out_free;
        }
    }

    /* Increment the number of buffers (XIDs) associated with this window. */
    pDbeWindowPriv->nBufferIDs++;

    /* Set swap action on all calls. */
    pDbeWindowPriv->swapAction = stuff->swapAction;

    return status;

 out_free:
    dixSetPrivate(&pWin->devPrivates, dbeWindowPrivKey, NULL);
    free(pDbeWindowPriv);
    return status;

}                               /* ProcDbeAllocateBackBufferName() */
