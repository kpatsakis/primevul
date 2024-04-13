mark_client_ready(ClientPtr client)
{
    if (xorg_list_is_empty(&client->ready))
        xorg_list_append(&client->ready, &ready_clients);
}
