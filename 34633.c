ProcOpenFont(ClientPtr client)
{
    int err;

    REQUEST(xOpenFontReq);

    REQUEST_FIXED_SIZE(xOpenFontReq, stuff->nbytes);
    client->errorValue = stuff->fid;
    LEGAL_NEW_RESOURCE(stuff->fid, client);
    err = OpenFont(client, stuff->fid, (Mask) 0,
                   stuff->nbytes, (char *) &stuff[1]);
    if (err == Success) {
        return Success;
    }
    else
        return err;
}
