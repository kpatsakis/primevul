int test_exp(BIO *bp, BN_CTX *ctx)
{
    BIGNUM *a, *b, *d, *e, *one;
    int i;

    a = BN_new();
    b = BN_new();
    d = BN_new();
    e = BN_new();
    one = BN_new();
    BN_one(one);

    for (i = 0; i < num2; i++) {
        BN_bntest_rand(a, 20 + i * 5, 0, 0);
        BN_bntest_rand(b, 2 + i, 0, 0);

        if (BN_exp(d, a, b, ctx) <= 0)
            return (0);

        if (bp != NULL) {
            if (!results) {
                BN_print(bp, a);
                BIO_puts(bp, " ^ ");
                BN_print(bp, b);
                BIO_puts(bp, " - ");
            }
            BN_print(bp, d);
            BIO_puts(bp, "\n");
        }
        BN_one(e);
        for (; !BN_is_zero(b); BN_sub(b, b, one))
            BN_mul(e, e, a, ctx);
        BN_sub(e, e, d);
        if (!BN_is_zero(e)) {
            fprintf(stderr, "Exponentiation test failed!\n");
            return 0;
        }
    }
    BN_free(a);
    BN_free(b);
    BN_free(d);
    BN_free(e);
    BN_free(one);
    return (1);
}
