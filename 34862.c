void mark_client_saved_ready(ClientPtr client)
{
    if (xorg_list_is_empty(&client->ready))
        xorg_list_append(&client->ready, &saved_ready_clients);
}
