EventMaskForClient(WindowPtr pWin, ClientPtr client)
{
    OtherClientsPtr other;

    if (wClient(pWin) == client)
        return pWin->eventMask;
    for (other = wOtherClients(pWin); other; other = other->next) {
        if (SameClient(other, client))
            return other->mask;
    }
    return 0;
}
