void EC_ec_pre_comp_free(EC_PRE_COMP *pre)
{
    int i;

    if (pre == NULL)
        return;

    CRYPTO_DOWN_REF(&pre->references, &i, pre->lock);
    REF_PRINT_COUNT("EC_ec", pre);
    if (i > 0)
        return;
    REF_ASSERT_ISNT(i < 0);

    if (pre->points != NULL) {
        EC_POINT **pts;

        for (pts = pre->points; *pts != NULL; pts++)
            EC_POINT_free(*pts);
        OPENSSL_free(pre->points);
    }
    CRYPTO_THREAD_lock_free(pre->lock);
    OPENSSL_free(pre);
}
