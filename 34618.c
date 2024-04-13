ProcGetImage(ClientPtr client)
{
    REQUEST(xGetImageReq);

    REQUEST_SIZE_MATCH(xGetImageReq);

    return DoGetImage(client, stuff->format, stuff->drawable,
                      stuff->x, stuff->y,
                      (int) stuff->width, (int) stuff->height,
                      stuff->planeMask);
}
