mark_client_grab(ClientPtr grab)
{
    ClientPtr   client, tmp;

    xorg_list_for_each_entry_safe(client, tmp, &ready_clients, ready) {
        if (client != grab) {
            xorg_list_del(&client->ready);
            xorg_list_append(&client->ready, &saved_ready_clients);
        }
    }
}
