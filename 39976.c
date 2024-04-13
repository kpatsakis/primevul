static void __exit caif_sktexit_module(void)
{
	sock_unregister(PF_CAIF);
}
