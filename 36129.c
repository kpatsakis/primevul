kadm5_set_string(void *server_handle, krb5_principal principal,
                 const char *key, const char *value)
{
    kadm5_server_handle_t handle = server_handle;
    kadm5_ret_t ret;
    krb5_db_entry *kdb;
    osa_princ_ent_rec adb;

    CHECK_HANDLE(server_handle);
    if (principal == NULL || key == NULL)
        return EINVAL;

    ret = kdb_get_entry(handle, principal, &kdb, &adb);
    if (ret)
        return ret;

    ret = krb5_dbe_set_string(handle->context, kdb, key, value);
    if (ret)
        goto done;

    kdb->mask = KADM5_TL_DATA;
    ret = kdb_put_entry(handle, kdb, &adb);

done:
    kdb_free_entry(handle, kdb, &adb);
    return ret;
}
