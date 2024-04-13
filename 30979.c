OtherClientGone(void *value, XID id)
{
    OtherClientsPtr other, prev;
    WindowPtr pWin = (WindowPtr) value;

    prev = 0;
    for (other = wOtherClients(pWin); other; other = other->next) {
        if (other->resource == id) {
            if (prev)
                prev->next = other->next;
            else {
                if (!(pWin->optional->otherClients = other->next))
                    CheckWindowOptionalNeed(pWin);
            }
            free(other);
            RecalculateDeliverableEvents(pWin);
            return Success;
        }
        prev = other;
    }
    FatalError("client not on event list");
}
