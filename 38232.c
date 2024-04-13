static int au1200fb_init_fbinfo(struct au1200fb_device *fbdev)
{
	struct fb_info *fbi = fbdev->fb_info;
	int bpp;

	fbi->fbops = &au1200fb_fb_ops;

	bpp = winbpp(win->w[fbdev->plane].mode_winctrl1);

	/* Copy monitor specs from panel data */
	/* fixme: we're setting up LCD controller windows, so these dont give a
	damn as to what the monitor specs are (the panel itself does, but that
	isn't done here...so maybe need a generic catchall monitor setting??? */
	memcpy(&fbi->monspecs, &panel->monspecs, sizeof(struct fb_monspecs));

	/* We first try the user mode passed in argument. If that failed,
	 * or if no one has been specified, we default to the first mode of the
	 * panel list. Note that after this call, var data will be set */
	if (!fb_find_mode(&fbi->var,
			  fbi,
			  NULL, /* drv_info.opt_mode, */
			  fbi->monspecs.modedb,
			  fbi->monspecs.modedb_len,
			  fbi->monspecs.modedb,
			  bpp)) {

		print_err("Cannot find valid mode for panel %s", panel->name);
		return -EFAULT;
	}

	fbi->pseudo_palette = kcalloc(16, sizeof(u32), GFP_KERNEL);
	if (!fbi->pseudo_palette) {
		return -ENOMEM;
	}

	if (fb_alloc_cmap(&fbi->cmap, AU1200_LCD_NBR_PALETTE_ENTRIES, 0) < 0) {
		print_err("Fail to allocate colormap (%d entries)",
			   AU1200_LCD_NBR_PALETTE_ENTRIES);
		kfree(fbi->pseudo_palette);
		return -EFAULT;
	}

	strncpy(fbi->fix.id, "AU1200", sizeof(fbi->fix.id));
	fbi->fix.smem_start = fbdev->fb_phys;
	fbi->fix.smem_len = fbdev->fb_len;
	fbi->fix.type = FB_TYPE_PACKED_PIXELS;
	fbi->fix.xpanstep = 0;
	fbi->fix.ypanstep = 0;
	fbi->fix.mmio_start = 0;
	fbi->fix.mmio_len = 0;
	fbi->fix.accel = FB_ACCEL_NONE;

	fbi->screen_base = (char __iomem *) fbdev->fb_mem;

	au1200fb_update_fbinfo(fbi);

	return 0;
}
