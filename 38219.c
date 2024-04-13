static void __exit au1100fb_unload(void)
{
	platform_driver_unregister(&au1100fb_driver);
}
