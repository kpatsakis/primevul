int oz_cdev_init(void)
{
	oz_app_enable(OZ_APPID_SERIAL, 1);
	return 0;
}
