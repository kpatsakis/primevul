kadm5_get_strings(void *server_handle, krb5_principal principal,
                  krb5_string_attr **strings_out, int *count_out)
{
    kadm5_server_handle_t handle = server_handle;
    kadm5_ret_t ret;
    krb5_db_entry *kdb = NULL;

    *strings_out = NULL;
    *count_out = 0;
    CHECK_HANDLE(server_handle);
    if (principal == NULL)
        return EINVAL;

    ret = kdb_get_entry(handle, principal, &kdb, NULL);
    if (ret)
        return ret;

    ret = krb5_dbe_get_strings(handle->context, kdb, strings_out, count_out);
    kdb_free_entry(handle, kdb, NULL);
    return ret;
}
