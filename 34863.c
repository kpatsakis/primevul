mark_client_ungrab(void)
{
    ClientPtr   client, tmp;

    xorg_list_for_each_entry_safe(client, tmp, &saved_ready_clients, ready) {
        xorg_list_del(&client->ready);
        xorg_list_append(&client->ready, &ready_clients);
    }
}
