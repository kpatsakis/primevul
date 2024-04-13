XineramaTryClientEventsResult(ClientPtr client,
                              GrabPtr grab, Mask mask, Mask filter)
{
    if ((client) && (client != serverClient) && (!client->clientGone) &&
        ((filter == CantBeFiltered) || (mask & filter))) {
        if (grab && !SameClient(grab, client))
            return -1;
        else
            return 1;
    }
    return 0;
}
