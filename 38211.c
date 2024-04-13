int au1100fb_drv_resume(struct platform_device *dev)
{
	struct au1100fb_device *fbdev = platform_get_drvdata(dev);

	if (!fbdev)
		return 0;

	memcpy(fbdev->regs, &fbregs, sizeof(struct au1100fb_regs));

	/* Restart LCD clocking */
	au_writel(sys_clksrc, SYS_CLKSRC);

	/* Unblank the LCD */
	au1100fb_fb_blank(VESA_NO_BLANKING, &fbdev->info);

	return 0;
}
