InitClient(ClientPtr client, int i, void *ospriv)
{
    client->index = i;
    client->clientAsMask = ((Mask) i) << CLIENTOFFSET;
    client->closeDownMode = i ? DestroyAll : RetainPermanent;
    client->requestVector = InitialVector;
    client->osPrivate = ospriv;
    QueryMinMaxKeyCodes(&client->minKC, &client->maxKC);
    client->smart_start_tick = SmartScheduleTime;
    client->smart_stop_tick = SmartScheduleTime;
    client->clientIds = NULL;
}
