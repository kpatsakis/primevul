kadm5_modify_principal(void *server_handle,
                       kadm5_principal_ent_t entry, long mask)
{
    int                     ret, ret2, i;
    kadm5_policy_ent_rec    npol, opol;
    int                     have_npol = 0, have_opol = 0;
    krb5_db_entry           *kdb;
    krb5_tl_data            *tl_data_orig;
    osa_princ_ent_rec       adb;
    kadm5_server_handle_t handle = server_handle;

    CHECK_HANDLE(server_handle);

    krb5_clear_error_message(handle->context);

    if((mask & KADM5_PRINCIPAL) || (mask & KADM5_LAST_PWD_CHANGE) ||
       (mask & KADM5_MOD_TIME) || (mask & KADM5_MOD_NAME) ||
       (mask & KADM5_MKVNO) || (mask & KADM5_AUX_ATTRIBUTES) ||
       (mask & KADM5_KEY_DATA) || (mask & KADM5_LAST_SUCCESS) ||
       (mask & KADM5_LAST_FAILED))
        return KADM5_BAD_MASK;
    if((mask & ~ALL_PRINC_MASK))
        return KADM5_BAD_MASK;
    if((mask & KADM5_POLICY) && (mask & KADM5_POLICY_CLR))
        return KADM5_BAD_MASK;
    if(entry == (kadm5_principal_ent_t) NULL)
        return EINVAL;
    if (mask & KADM5_TL_DATA) {
        tl_data_orig = entry->tl_data;
        while (tl_data_orig) {
            if (tl_data_orig->tl_data_type < 256)
                return KADM5_BAD_TL_TYPE;
            tl_data_orig = tl_data_orig->tl_data_next;
        }
    }

    ret = kdb_get_entry(handle, entry->principal, &kdb, &adb);
    if (ret)
        return(ret);

    /*
     * This is pretty much the same as create ...
     */

    if ((mask & KADM5_POLICY)) {
        /* get the new policy */
        ret = kadm5_get_policy(handle->lhandle, entry->policy, &npol);
        if (ret) {
            switch (ret) {
            case EINVAL:
                ret = KADM5_BAD_POLICY;
                break;
            case KADM5_UNK_POLICY:
            case KADM5_BAD_POLICY:
                ret =  KADM5_UNK_POLICY;
                break;
            }
            goto done;
        }
        have_npol = 1;

        /* if we already have a policy, get it to decrement the refcnt */
        if(adb.aux_attributes & KADM5_POLICY) {
            /* ... but not if the old and new are the same */
            if(strcmp(adb.policy, entry->policy)) {
                ret = kadm5_get_policy(handle->lhandle,
                                       adb.policy, &opol);
                switch(ret) {
                case EINVAL:
                case KADM5_BAD_POLICY:
                case KADM5_UNK_POLICY:
                    break;
                case KADM5_OK:
                    have_opol = 1;
                    opol.policy_refcnt--;
                    break;
                default:
                    goto done;
                    break;
                }
                npol.policy_refcnt++;
            }
        } else npol.policy_refcnt++;

        /* set us up to use the new policy */
        adb.aux_attributes |= KADM5_POLICY;
        if (adb.policy)
            free(adb.policy);
        adb.policy = strdup(entry->policy);

        /* set pw_max_life based on new policy */
        if (npol.pw_max_life) {
            ret = krb5_dbe_lookup_last_pwd_change(handle->context, kdb,
                                                  &(kdb->pw_expiration));
            if (ret)
                goto done;
            kdb->pw_expiration += npol.pw_max_life;
        } else {
            kdb->pw_expiration = 0;
        }
    }

    if ((mask & KADM5_POLICY_CLR) &&
        (adb.aux_attributes & KADM5_POLICY)) {
        ret = kadm5_get_policy(handle->lhandle, adb.policy, &opol);
        switch(ret) {
        case EINVAL:
        case KADM5_BAD_POLICY:
        case KADM5_UNK_POLICY:
            ret = KADM5_BAD_DB;
            goto done;
            break;
        case KADM5_OK:
            have_opol = 1;
            if (adb.policy)
                free(adb.policy);
            adb.policy = NULL;
            adb.aux_attributes &= ~KADM5_POLICY;
            kdb->pw_expiration = 0;
            opol.policy_refcnt--;
            break;
        default:
            goto done;
            break;
        }
    }

    if (((mask & KADM5_POLICY) || (mask & KADM5_POLICY_CLR)) &&
        (((have_opol) &&
          (ret =
           kadm5_modify_policy_internal(handle->lhandle, &opol,
                                        KADM5_REF_COUNT))) ||
         ((have_npol) &&
          (ret =
           kadm5_modify_policy_internal(handle->lhandle, &npol,
                                        KADM5_REF_COUNT)))))
        goto done;

    if ((mask & KADM5_ATTRIBUTES))
        kdb->attributes = entry->attributes;
    if ((mask & KADM5_MAX_LIFE))
        kdb->max_life = entry->max_life;
    if ((mask & KADM5_PRINC_EXPIRE_TIME))
        kdb->expiration = entry->princ_expire_time;
    if (mask & KADM5_PW_EXPIRATION)
        kdb->pw_expiration = entry->pw_expiration;
    if (mask & KADM5_MAX_RLIFE)
        kdb->max_renewable_life = entry->max_renewable_life;

    if((mask & KADM5_KVNO)) {
        for (i = 0; i < kdb->n_key_data; i++)
            kdb->key_data[i].key_data_kvno = entry->kvno;
    }

    if (mask & KADM5_TL_DATA) {
        krb5_tl_data *tl;

        /* may have to change the version number of the API. Updates the list with the given tl_data rather than over-writting */

        for (tl = entry->tl_data; tl;
             tl = tl->tl_data_next)
        {
            ret = krb5_dbe_update_tl_data(handle->context, kdb, tl);
            if( ret )
            {
                goto done;
            }
        }
    }

    /*
     * Setting entry->fail_auth_count to 0 can be used to manually unlock
     * an account. It is not possible to set fail_auth_count to any other
     * value using kadmin.
     */
    if (mask & KADM5_FAIL_AUTH_COUNT) {
        if (entry->fail_auth_count != 0) {
            ret = KADM5_BAD_SERVER_PARAMS;
            goto done;
        }

        kdb->fail_auth_count = 0;
    }

    /* let the mask propagate to the database provider */
    kdb->mask = mask;

    ret = k5_kadm5_hook_modify(handle->context, handle->hook_handles,
                               KADM5_HOOK_STAGE_PRECOMMIT, entry, mask);
    if (ret)
        goto done;

    ret = kdb_put_entry(handle, kdb, &adb);
    if (ret) goto done;
    (void) k5_kadm5_hook_modify(handle->context, handle->hook_handles,
                                KADM5_HOOK_STAGE_POSTCOMMIT, entry, mask);

    ret = KADM5_OK;
done:
    if (have_opol) {
        ret2 = kadm5_free_policy_ent(handle->lhandle, &opol);
        ret = ret ? ret : ret2;
    }
    if (have_npol) {
        ret2 = kadm5_free_policy_ent(handle->lhandle, &npol);
        ret = ret ? ret : ret2;
    }
    kdb_free_entry(handle, kdb, &adb);
    return ret;
}
