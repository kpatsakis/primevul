SProcRenderSetPictureTransform (ClientPtr client)
{
    register int n;
    REQUEST(xRenderSetPictureTransformReq);
    REQUEST_SIZE_MATCH(xRenderSetPictureTransformReq);

    swaps(&stuff->length, n);
    swapl(&stuff->picture, n);
    swapl(&stuff->transform.matrix11, n);
    swapl(&stuff->transform.matrix12, n);
    swapl(&stuff->transform.matrix13, n);
    swapl(&stuff->transform.matrix21, n);
    swapl(&stuff->transform.matrix22, n);
    swapl(&stuff->transform.matrix23, n);
    swapl(&stuff->transform.matrix31, n);
    swapl(&stuff->transform.matrix32, n);
    swapl(&stuff->transform.matrix33, n);
    return (*ProcRenderVector[stuff->renderReqType]) (client);
}
