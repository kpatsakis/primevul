int DH_generate_key(DH *dh)
{
#ifdef OPENSSL_FIPS
    if (FIPS_mode() && !(dh->meth->flags & DH_FLAG_FIPS_METHOD)
        && !(dh->flags & DH_FLAG_NON_FIPS_ALLOW)) {
        DHerr(DH_F_DH_GENERATE_KEY, DH_R_NON_FIPS_METHOD);
        return 0;
    }
#endif
    return dh->meth->generate_key(dh);
}
