static int __init ghash_mod_init(void)
{
	return crypto_register_shash(&ghash_alg);
}
