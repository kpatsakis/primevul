kadm5_rename_principal(void *server_handle,
                       krb5_principal source, krb5_principal target)
{
    krb5_db_entry *kdb;
    osa_princ_ent_rec adb;
    krb5_error_code ret;
    kadm5_server_handle_t handle = server_handle;
    krb5_int16 stype, i;
    krb5_data *salt = NULL;

    CHECK_HANDLE(server_handle);

    krb5_clear_error_message(handle->context);

    if (source == NULL || target == NULL)
        return EINVAL;

    if ((ret = kdb_get_entry(handle, target, &kdb, &adb)) == 0) {
        kdb_free_entry(handle, kdb, &adb);
        return(KADM5_DUP);
    }

    if ((ret = kdb_get_entry(handle, source, &kdb, &adb)))
        return ret;

    /* Transform salts as necessary. */
    for (i = 0; i < kdb->n_key_data; i++) {
        ret = krb5_dbe_compute_salt(handle->context, &kdb->key_data[i],
                                    kdb->princ, &stype, &salt);
        if (ret == KRB5_KDB_BAD_SALTTYPE)
            ret = KADM5_NO_RENAME_SALT;
        if (ret)
            goto done;
        kdb->key_data[i].key_data_type[1] = KRB5_KDB_SALTTYPE_SPECIAL;
        free(kdb->key_data[i].key_data_contents[1]);
        kdb->key_data[i].key_data_contents[1] = (krb5_octet *)salt->data;
        kdb->key_data[i].key_data_length[1] = salt->length;
        kdb->key_data[i].key_data_ver = 2;
        free(salt);
        salt = NULL;
    }

    kadm5_free_principal(handle->context, kdb->princ);
    ret = kadm5_copy_principal(handle->context, target, &kdb->princ);
    if (ret) {
        kdb->princ = NULL; /* so freeing the dbe doesn't lose */
        goto done;
    }

    if ((ret = kdb_put_entry(handle, kdb, &adb)))
        goto done;

    ret = kdb_delete_entry(handle, source);

done:
    krb5_free_data(handle->context, salt);
    kdb_free_entry(handle, kdb, &adb);
    return ret;
}
