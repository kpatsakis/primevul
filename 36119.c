kadm5_delete_principal(void *server_handle, krb5_principal principal)
{
    unsigned int                ret;
    kadm5_policy_ent_rec        polent;
    krb5_db_entry               *kdb;
    osa_princ_ent_rec           adb;
    kadm5_server_handle_t handle = server_handle;

    CHECK_HANDLE(server_handle);

    krb5_clear_error_message(handle->context);

    if (principal == NULL)
        return EINVAL;

    if ((ret = kdb_get_entry(handle, principal, &kdb, &adb)))
        return(ret);
    ret = k5_kadm5_hook_remove(handle->context, handle->hook_handles,
                               KADM5_HOOK_STAGE_PRECOMMIT, principal);
    if (ret) {
        kdb_free_entry(handle, kdb, &adb);
        return ret;
    }

    if ((adb.aux_attributes & KADM5_POLICY)) {
        if ((ret = kadm5_get_policy(handle->lhandle,
                                    adb.policy, &polent))
            == KADM5_OK) {
            polent.policy_refcnt--;
            if ((ret = kadm5_modify_policy_internal(handle->lhandle, &polent,
                                                    KADM5_REF_COUNT))
                != KADM5_OK) {
                (void) kadm5_free_policy_ent(handle->lhandle, &polent);
                kdb_free_entry(handle, kdb, &adb);
                return(ret);
            }
        }
        if ((ret = kadm5_free_policy_ent(handle->lhandle, &polent))) {
            kdb_free_entry(handle, kdb, &adb);
            return ret;
        }
    }

    ret = kdb_delete_entry(handle, principal);

    kdb_free_entry(handle, kdb, &adb);

    if (ret == 0)
        (void) k5_kadm5_hook_remove(handle->context,
                                    handle->hook_handles,
                                    KADM5_HOOK_STAGE_POSTCOMMIT, principal);

    return ret;
}
