DH *get_dh2048()
{
    DH *dh;

    if ((dh = DH_new()) == NULL)
        return NULL;
    dh->p=BN_bin2bn(dh2048_p, sizeof(dh2048_p), NULL);
    dh->g=BN_bin2bn(dh2048_g, sizeof(dh2048_g), NULL);
    if (dh->p == NULL || dh->g == NULL) {
        DH_free(dh);
        return NULL;
    }
    return dh;
}
