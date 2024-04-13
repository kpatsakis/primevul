static void __exit ghash_mod_exit(void)
{
	crypto_unregister_shash(&ghash_alg);
}
