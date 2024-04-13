static void printmetadata(const mbentry_t *mbentry,
                          const strarray_t *entries,
                          struct getmetadata_options *opts)
{
    annotate_state_t *astate = annotate_state_new();
    strarray_t newa = STRARRAY_INITIALIZER;
    strarray_t newe = STRARRAY_INITIALIZER;
    annotate_state_set_auth(astate,
                            imapd_userisadmin || imapd_userisproxyadmin,
                            imapd_userid, imapd_authstate);
    int r = annotate_state_set_mailbox_mbe(astate, mbentry);
    if (r) goto done;
    r = _metadata_to_annotate(entries, &newa, &newe, NULL, opts->depth);
    if (r) goto done;

    annotate_state_fetch(astate, &newe, &newa, getmetadata_response, opts);
    getmetadata_response(NULL, 0, NULL, NULL, opts);

done:
    annotate_state_abort(&astate);
}
