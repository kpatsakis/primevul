mark_client_not_ready(ClientPtr client)
{
    xorg_list_del(&client->ready);
}
