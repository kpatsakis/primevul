check_pw_reuse(krb5_context context,
               krb5_keyblock *hist_keyblocks,
               int n_new_key_data, krb5_key_data *new_key_data,
               unsigned int n_pw_hist_data, osa_pw_hist_ent *pw_hist_data)
{
    unsigned int x, y, z;
    krb5_keyblock newkey, histkey, *kb;
    krb5_key_data *key_data;
    krb5_error_code ret;

    assert (n_new_key_data >= 0);
    for (x = 0; x < (unsigned) n_new_key_data; x++) {
        /* Check only entries with the most recent kvno. */
        if (new_key_data[x].key_data_kvno != new_key_data[0].key_data_kvno)
            break;
        ret = krb5_dbe_decrypt_key_data(context, NULL, &(new_key_data[x]),
                                        &newkey, NULL);
        if (ret)
            return(ret);
        for (y = 0; y < n_pw_hist_data; y++) {
            for (z = 0; z < (unsigned int) pw_hist_data[y].n_key_data; z++) {
                for (kb = hist_keyblocks; kb->enctype != 0; kb++) {
                    key_data = &pw_hist_data[y].key_data[z];
                    ret = krb5_dbe_decrypt_key_data(context, kb, key_data,
                                                    &histkey, NULL);
                    if (ret)
                        continue;
                    if (newkey.length == histkey.length &&
                        newkey.enctype == histkey.enctype &&
                        memcmp(newkey.contents, histkey.contents,
                               histkey.length) == 0) {
                        krb5_free_keyblock_contents(context, &histkey);
                        krb5_free_keyblock_contents(context, &newkey);
                        return KADM5_PASS_REUSE;
                    }
                    krb5_free_keyblock_contents(context, &histkey);
                }
            }
        }
        krb5_free_keyblock_contents(context, &newkey);
    }

    return(0);
}
