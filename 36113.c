kadm5_chpass_principal(void *server_handle,
                       krb5_principal principal, char *password)
{
    return
        kadm5_chpass_principal_3(server_handle, principal, FALSE,
                                 0, NULL, password);
}
