ProcListFontsWithInfo(ClientPtr client)
{
    REQUEST(xListFontsWithInfoReq);

    REQUEST_FIXED_SIZE(xListFontsWithInfoReq, stuff->nbytes);

    return StartListFontsWithInfo(client, stuff->nbytes,
                                  (unsigned char *) &stuff[1], stuff->maxNames);
}
