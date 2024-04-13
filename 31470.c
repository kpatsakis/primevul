EC_KEY *o2i_ECPublicKey(EC_KEY **a, const unsigned char **in, long len)
{
    EC_KEY *ret = NULL;

    if (a == NULL || (*a) == NULL || (*a)->group == NULL) {
        /*
         * sorry, but a EC_GROUP-structur is necessary to set the public key
         */
        ECerr(EC_F_O2I_ECPUBLICKEY, ERR_R_PASSED_NULL_PARAMETER);
        return 0;
    }
    ret = *a;
    if (ret->pub_key == NULL &&
        (ret->pub_key = EC_POINT_new(ret->group)) == NULL) {
        ECerr(EC_F_O2I_ECPUBLICKEY, ERR_R_MALLOC_FAILURE);
        return 0;
    }
    if (!EC_POINT_oct2point(ret->group, ret->pub_key, *in, len, NULL)) {
        ECerr(EC_F_O2I_ECPUBLICKEY, ERR_R_EC_LIB);
        return 0;
    }
    /* save the point conversion form */
    ret->conv_form = (point_conversion_form_t) (*in[0] & ~0x01);
    *in += len;
    return ret;
}
