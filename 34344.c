SProcRenderCreateRadialGradient (ClientPtr client)
{
    register int n;
    int len;
    REQUEST (xRenderCreateRadialGradientReq);
    REQUEST_AT_LEAST_SIZE (xRenderCreateRadialGradientReq);

    swaps(&stuff->length, n);
    swapl(&stuff->pid, n);
    swapl(&stuff->inner.x, n);
    swapl(&stuff->inner.y, n);
    swapl(&stuff->outer.x, n);
    swapl(&stuff->outer.y, n);
    swapl(&stuff->inner_radius, n);
    swapl(&stuff->outer_radius, n);
    swapl(&stuff->nStops, n);

    len = (client->req_len << 2) - sizeof(xRenderCreateRadialGradientReq);
    if (stuff->nStops > UINT32_MAX/(sizeof(xFixed) + sizeof(xRenderColor)))
	return BadLength;
    if (len != stuff->nStops*(sizeof(xFixed) + sizeof(xRenderColor)))
        return BadLength;

    swapStops(stuff+1, stuff->nStops);

    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
