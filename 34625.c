ProcInternAtom(ClientPtr client)
{
    Atom atom;
    char *tchar;

    REQUEST(xInternAtomReq);

    REQUEST_FIXED_SIZE(xInternAtomReq, stuff->nbytes);
    if ((stuff->onlyIfExists != xTrue) && (stuff->onlyIfExists != xFalse)) {
        client->errorValue = stuff->onlyIfExists;
        return BadValue;
    }
    tchar = (char *) &stuff[1];
    atom = MakeAtom(tchar, stuff->nbytes, !stuff->onlyIfExists);
    if (atom != BAD_RESOURCE) {
        xInternAtomReply reply = {
            .type = X_Reply,
            .sequenceNumber = client->sequence,
            .length = 0,
            .atom = atom
        };
        WriteReplyToClient(client, sizeof(xInternAtomReply), &reply);
        return Success;
    }
    else
        return BadAlloc;
}
