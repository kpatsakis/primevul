ProcChangeHosts(ClientPtr client)
{
    REQUEST(xChangeHostsReq);

    REQUEST_FIXED_SIZE(xChangeHostsReq, stuff->hostLength);

    if (stuff->mode == HostInsert)
        return AddHost(client, (int) stuff->hostFamily,
                       stuff->hostLength, (void *) &stuff[1]);
    if (stuff->mode == HostDelete)
        return RemoveHost(client, (int) stuff->hostFamily,
                          stuff->hostLength, (void *) &stuff[1]);
    client->errorValue = stuff->mode;
    return BadValue;
}
