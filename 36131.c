kadm5_setkey_principal(void *server_handle,
                       krb5_principal principal,
                       krb5_keyblock *keyblocks,
                       int n_keys)
{
    return
        kadm5_setkey_principal_3(server_handle, principal,
                                 FALSE, 0, NULL,
                                 keyblocks, n_keys);
}
