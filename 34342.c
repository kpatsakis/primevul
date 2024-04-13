SProcRenderCreateLinearGradient (ClientPtr client)
{
    register int n;
    int len;
    REQUEST (xRenderCreateLinearGradientReq);
    REQUEST_AT_LEAST_SIZE (xRenderCreateLinearGradientReq);

    swaps(&stuff->length, n);
    swapl(&stuff->pid, n);
    swapl(&stuff->p1.x, n);
    swapl(&stuff->p1.y, n);
    swapl(&stuff->p2.x, n);
    swapl(&stuff->p2.y, n);
    swapl(&stuff->nStops, n);

    len = (client->req_len << 2) - sizeof(xRenderCreateLinearGradientReq);
    if (stuff->nStops > UINT32_MAX/(sizeof(xFixed) + sizeof(xRenderColor)))
	return BadLength;
    if (len != stuff->nStops*(sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    swapStops(stuff+1, stuff->nStops);

    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
