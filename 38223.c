static void __exit au1200fb_cleanup(void)
{
	platform_driver_unregister(&au1200fb_driver);
}
