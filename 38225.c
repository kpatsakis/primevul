static int au1200fb_drv_remove(struct platform_device *dev)
{
	struct au1200fb_platdata *pd = platform_get_drvdata(dev);
	struct au1200fb_device *fbdev;
	struct fb_info *fbi;
	int plane;

	/* Turn off the panel */
	au1200_setpanel(NULL, pd);

	for (plane = 0; plane < device_count; ++plane)	{
		fbi = _au1200fb_infos[plane];
		fbdev = fbi->par;

		/* Clean up all probe data */
		unregister_framebuffer(fbi);
		if (fbi->cmap.len != 0)
			fb_dealloc_cmap(&fbi->cmap);
		kfree(fbi->pseudo_palette);

		framebuffer_release(fbi);
		_au1200fb_infos[plane] = NULL;
	}

	free_irq(platform_get_irq(dev, 0), (void *)dev);

	return 0;
}
