KillAllClients(void)
{
    int i;

    for (i = 1; i < currentMaxClients; i++)
        if (clients[i]) {
            /* Make sure Retained clients are released. */
            clients[i]->closeDownMode = DestroyAll;
            CloseDownClient(clients[i]);
        }
}
