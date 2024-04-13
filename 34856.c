clients_are_ready(void)
{
    return !xorg_list_is_empty(&ready_clients);
}
