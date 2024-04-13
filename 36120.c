kadm5_free_principal(krb5_context context, krb5_principal val)
{
    register krb5_int32 i;

    if (!val)
        return;

    if (val->data) {
        i = krb5_princ_size(context, val);
        while(--i >= 0)
            krb5_db_free(context, krb5_princ_component(context, val, i)->data);
        krb5_db_free(context, val->data);
    }
    if (val->realm.data)
        krb5_db_free(context, val->realm.data);
    krb5_db_free(context, val);
}
