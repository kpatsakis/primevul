static int au1200fb_drv_suspend(struct device *dev)
{
	struct au1200fb_platdata *pd = dev_get_drvdata(dev);
	au1200_setpanel(NULL, pd);

	lcd->outmask = 0;
	au_sync();

	return 0;
}
