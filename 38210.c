int au1100fb_drv_remove(struct platform_device *dev)
{
	struct au1100fb_device *fbdev = NULL;

	if (!dev)
		return -ENODEV;

	fbdev = (struct au1100fb_device *) platform_get_drvdata(dev);

#if !defined(CONFIG_FRAMEBUFFER_CONSOLE) && defined(CONFIG_LOGO)
	au1100fb_fb_blank(VESA_POWERDOWN, &fbdev->info);
#endif
	fbdev->regs->lcd_control &= ~LCD_CONTROL_GO;

	/* Clean up all probe data */
	unregister_framebuffer(&fbdev->info);

	fb_dealloc_cmap(&fbdev->info.cmap);

	return 0;
}
