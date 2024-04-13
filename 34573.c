CloseDownRetainedResources(void)
{
    int i;
    ClientPtr client;

    for (i = 1; i < currentMaxClients; i++) {
        client = clients[i];
        if (client && (client->closeDownMode == RetainTemporary)
            && (client->clientGone))
            CloseDownClient(client);
    }
}
