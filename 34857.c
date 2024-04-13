init_client_ready(void)
{
    xorg_list_init(&ready_clients);
    xorg_list_init(&saved_ready_clients);
    xorg_list_init(&output_pending_clients);
}
