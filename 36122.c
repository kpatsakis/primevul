kadm5_get_principal_keys(void *server_handle /* IN */,
                         krb5_principal principal /* IN */,
                         krb5_keyblock **keyblocks /* OUT */,
                         int *n_keys /* OUT */)
{
    krb5_db_entry               *kdb;
    osa_princ_ent_rec           adb;
    kadm5_ret_t                 ret;
    kadm5_server_handle_t       handle = server_handle;

    if (keyblocks)
        *keyblocks = NULL;

    CHECK_HANDLE(server_handle);

    if (principal == NULL)
        return EINVAL;

    if ((ret = kdb_get_entry(handle, principal, &kdb, &adb)))
        return(ret);

    if (keyblocks) {
        ret = decrypt_key_data(handle->context,
                               kdb->n_key_data, kdb->key_data,
                               keyblocks, n_keys);
        if (ret)
            goto done;
    }

    ret = KADM5_OK;
done:
    kdb_free_entry(handle, kdb, &adb);

    return ret;
}
