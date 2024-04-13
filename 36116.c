kadm5_create_principal(void *server_handle,
                       kadm5_principal_ent_t entry, long mask,
                       char *password)
{
    return
        kadm5_create_principal_3(server_handle, entry, mask,
                                 0, NULL, password);
}
