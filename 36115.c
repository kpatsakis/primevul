kadm5_copy_principal(krb5_context context, krb5_const_principal inprinc, krb5_principal *outprinc)
{
    register krb5_principal tempprinc;
    register int i, nelems;

    tempprinc = (krb5_principal)krb5_db_alloc(context, NULL, sizeof(krb5_principal_data));

    if (tempprinc == 0)
        return ENOMEM;

    VALGRIND_CHECK_DEFINED(*inprinc);
    *tempprinc = *inprinc;

    nelems = (int) krb5_princ_size(context, inprinc);
    tempprinc->data = krb5_db_alloc(context, NULL, nelems * sizeof(krb5_data));
    if (tempprinc->data == 0) {
        krb5_db_free(context, (char *)tempprinc);
        return ENOMEM;
    }

    for (i = 0; i < nelems; i++) {
        unsigned int len = krb5_princ_component(context, inprinc, i)->length;
        krb5_princ_component(context, tempprinc, i)->length = len;
        if (((krb5_princ_component(context, tempprinc, i)->data =
              krb5_db_alloc(context, NULL, len)) == 0) && len) {
            while (--i >= 0)
                krb5_db_free(context, krb5_princ_component(context, tempprinc, i)->data);
            krb5_db_free (context, tempprinc->data);
            krb5_db_free (context, tempprinc);
            return ENOMEM;
        }
        if (len)
            memcpy(krb5_princ_component(context, tempprinc, i)->data,
                   krb5_princ_component(context, inprinc, i)->data, len);
        krb5_princ_component(context, tempprinc, i)->magic = KV5M_DATA;
    }

    tempprinc->realm.data =
        krb5_db_alloc(context, NULL, tempprinc->realm.length = inprinc->realm.length);
    if (!tempprinc->realm.data && tempprinc->realm.length) {
        for (i = 0; i < nelems; i++)
            krb5_db_free(context, krb5_princ_component(context, tempprinc, i)->data);
        krb5_db_free(context, tempprinc->data);
        krb5_db_free(context, tempprinc);
        return ENOMEM;
    }
    if (tempprinc->realm.length)
        memcpy(tempprinc->realm.data, inprinc->realm.data,
               inprinc->realm.length);

    *outprinc = tempprinc;
    return 0;
}
