static int au1200fb_drv_probe(struct platform_device *dev)
{
	struct au1200fb_device *fbdev;
	struct au1200fb_platdata *pd;
	struct fb_info *fbi = NULL;
	unsigned long page;
	int bpp, plane, ret, irq;

	print_info("" DRIVER_DESC "");

	pd = dev->dev.platform_data;
	if (!pd)
		return -ENODEV;

	/* Setup driver with options */
	if (au1200fb_setup(pd))
		return -ENODEV;

	/* Point to the panel selected */
	panel = &known_lcd_panels[panel_index];
	win = &windows[window_index];

	printk(DRIVER_NAME ": Panel %d %s\n", panel_index, panel->name);
	printk(DRIVER_NAME ": Win %d %s\n", window_index, win->name);

	/* shut gcc up */
	ret = 0;
	fbdev = NULL;

	for (plane = 0; plane < device_count; ++plane) {
		bpp = winbpp(win->w[plane].mode_winctrl1);
		if (win->w[plane].xres == 0)
			win->w[plane].xres = panel->Xres;
		if (win->w[plane].yres == 0)
			win->w[plane].yres = panel->Yres;

		fbi = framebuffer_alloc(sizeof(struct au1200fb_device),
					&dev->dev);
		if (!fbi)
			goto failed;

		_au1200fb_infos[plane] = fbi;
		fbdev = fbi->par;
		fbdev->fb_info = fbi;
		fbdev->pd = pd;

		fbdev->plane = plane;

		/* Allocate the framebuffer to the maximum screen size */
		fbdev->fb_len = (win->w[plane].xres * win->w[plane].yres * bpp) / 8;

		fbdev->fb_mem = dmam_alloc_noncoherent(&dev->dev,
				PAGE_ALIGN(fbdev->fb_len),
				&fbdev->fb_phys, GFP_KERNEL);
		if (!fbdev->fb_mem) {
			print_err("fail to allocate frambuffer (size: %dK))",
				  fbdev->fb_len / 1024);
			return -ENOMEM;
		}

		/*
		 * Set page reserved so that mmap will work. This is necessary
		 * since we'll be remapping normal memory.
		 */
		for (page = (unsigned long)fbdev->fb_phys;
		     page < PAGE_ALIGN((unsigned long)fbdev->fb_phys +
			     fbdev->fb_len);
		     page += PAGE_SIZE) {
			SetPageReserved(pfn_to_page(page >> PAGE_SHIFT)); /* LCD DMA is NOT coherent on Au1200 */
		}
		print_dbg("Framebuffer memory map at %p", fbdev->fb_mem);
		print_dbg("phys=0x%08x, size=%dK", fbdev->fb_phys, fbdev->fb_len / 1024);

		/* Init FB data */
		if ((ret = au1200fb_init_fbinfo(fbdev)) < 0)
			goto failed;

		/* Register new framebuffer */
		ret = register_framebuffer(fbi);
		if (ret < 0) {
			print_err("cannot register new framebuffer");
			goto failed;
		}

		au1200fb_fb_set_par(fbi);

#if !defined(CONFIG_FRAMEBUFFER_CONSOLE) && defined(CONFIG_LOGO)
		if (plane == 0)
			if (fb_prepare_logo(fbi, FB_ROTATE_UR)) {
				/* Start display and show logo on boot */
				fb_set_cmap(&fbi->cmap, fbi);
				fb_show_logo(fbi, FB_ROTATE_UR);
			}
#endif
	}

	/* Now hook interrupt too */
	irq = platform_get_irq(dev, 0);
	ret = request_irq(irq, au1200fb_handle_irq,
			  IRQF_SHARED, "lcd", (void *)dev);
	if (ret) {
		print_err("fail to request interrupt line %d (err: %d)",
			  irq, ret);
		goto failed;
	}

	platform_set_drvdata(dev, pd);

	/* Kickstart the panel */
	au1200_setpanel(panel, pd);

	return 0;

failed:
	/* NOTE: This only does the current plane/window that failed; others are still active */
	if (fbi) {
		if (fbi->cmap.len != 0)
			fb_dealloc_cmap(&fbi->cmap);
		kfree(fbi->pseudo_palette);
	}
	if (plane == 0)
		free_irq(AU1200_LCD_INT, (void*)dev);
	return ret;
}
