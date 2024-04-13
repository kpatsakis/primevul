SProcRenderCreateConicalGradient (ClientPtr client)
{
    register int n;
    int len;
    REQUEST (xRenderCreateConicalGradientReq);
    REQUEST_AT_LEAST_SIZE (xRenderCreateConicalGradientReq);

    swaps(&stuff->length, n);
    swapl(&stuff->pid, n);
    swapl(&stuff->center.x, n);
    swapl(&stuff->center.y, n);
    swapl(&stuff->angle, n);
    swapl(&stuff->nStops, n);

    len = (client->req_len << 2) - sizeof(xRenderCreateConicalGradientReq);
    if (stuff->nStops > UINT32_MAX/(sizeof(xFixed) + sizeof(xRenderColor)))
	return BadLength;
    if (len != stuff->nStops*(sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    swapStops(stuff+1, stuff->nStops);

    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
