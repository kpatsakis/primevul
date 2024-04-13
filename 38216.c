static int __init au1100fb_load(void)
{
	return platform_driver_register(&au1100fb_driver);
}
