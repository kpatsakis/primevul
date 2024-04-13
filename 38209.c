static int au1100fb_drv_probe(struct platform_device *dev)
{
	struct au1100fb_device *fbdev = NULL;
	struct resource *regs_res;
	unsigned long page;
	u32 sys_clksrc;

	/* Allocate new device private */
	fbdev = devm_kzalloc(&dev->dev, sizeof(struct au1100fb_device),
			     GFP_KERNEL);
	if (!fbdev) {
		print_err("fail to allocate device private record");
		return -ENOMEM;
	}

	if (au1100fb_setup(fbdev))
		goto failed;

	platform_set_drvdata(dev, (void *)fbdev);

	/* Allocate region for our registers and map them */
	regs_res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!regs_res) {
		print_err("fail to retrieve registers resource");
		return -EFAULT;
	}

	au1100fb_fix.mmio_start = regs_res->start;
	au1100fb_fix.mmio_len = resource_size(regs_res);

	if (!devm_request_mem_region(&dev->dev,
				     au1100fb_fix.mmio_start,
				     au1100fb_fix.mmio_len,
				     DRIVER_NAME)) {
		print_err("fail to lock memory region at 0x%08lx",
				au1100fb_fix.mmio_start);
		return -EBUSY;
	}

	fbdev->regs = (struct au1100fb_regs*)KSEG1ADDR(au1100fb_fix.mmio_start);

	print_dbg("Register memory map at %p", fbdev->regs);
	print_dbg("phys=0x%08x, size=%d", fbdev->regs_phys, fbdev->regs_len);

	/* Allocate the framebuffer to the maximum screen size * nbr of video buffers */
	fbdev->fb_len = fbdev->panel->xres * fbdev->panel->yres *
		  	(fbdev->panel->bpp >> 3) * AU1100FB_NBR_VIDEO_BUFFERS;

	fbdev->fb_mem = dmam_alloc_coherent(&dev->dev,
					    PAGE_ALIGN(fbdev->fb_len),
					    &fbdev->fb_phys, GFP_KERNEL);
	if (!fbdev->fb_mem) {
		print_err("fail to allocate frambuffer (size: %dK))",
			  fbdev->fb_len / 1024);
		return -ENOMEM;
	}

	au1100fb_fix.smem_start = fbdev->fb_phys;
	au1100fb_fix.smem_len = fbdev->fb_len;

	/*
	 * Set page reserved so that mmap will work. This is necessary
	 * since we'll be remapping normal memory.
	 */
	for (page = (unsigned long)fbdev->fb_mem;
	     page < PAGE_ALIGN((unsigned long)fbdev->fb_mem + fbdev->fb_len);
	     page += PAGE_SIZE) {
#ifdef CONFIG_DMA_NONCOHERENT
		SetPageReserved(virt_to_page(CAC_ADDR((void *)page)));
#else
		SetPageReserved(virt_to_page(page));
#endif
	}

	print_dbg("Framebuffer memory map at %p", fbdev->fb_mem);
	print_dbg("phys=0x%08x, size=%dK", fbdev->fb_phys, fbdev->fb_len / 1024);

	/* Setup LCD clock to AUX (48 MHz) */
	sys_clksrc = au_readl(SYS_CLKSRC) & ~(SYS_CS_ML_MASK | SYS_CS_DL | SYS_CS_CL);
	au_writel((sys_clksrc | (1 << SYS_CS_ML_BIT)), SYS_CLKSRC);

	/* load the panel info into the var struct */
	au1100fb_var.bits_per_pixel = fbdev->panel->bpp;
	au1100fb_var.xres = fbdev->panel->xres;
	au1100fb_var.xres_virtual = au1100fb_var.xres;
	au1100fb_var.yres = fbdev->panel->yres;
	au1100fb_var.yres_virtual = au1100fb_var.yres;

	fbdev->info.screen_base = fbdev->fb_mem;
	fbdev->info.fbops = &au1100fb_ops;
	fbdev->info.fix = au1100fb_fix;

	fbdev->info.pseudo_palette =
		devm_kzalloc(&dev->dev, sizeof(u32) * 16, GFP_KERNEL);
	if (!fbdev->info.pseudo_palette)
		return -ENOMEM;

	if (fb_alloc_cmap(&fbdev->info.cmap, AU1100_LCD_NBR_PALETTE_ENTRIES, 0) < 0) {
		print_err("Fail to allocate colormap (%d entries)",
			   AU1100_LCD_NBR_PALETTE_ENTRIES);
		return -EFAULT;
	}

	fbdev->info.var = au1100fb_var;

	/* Set h/w registers */
	au1100fb_setmode(fbdev);

	/* Register new framebuffer */
	if (register_framebuffer(&fbdev->info) < 0) {
		print_err("cannot register new framebuffer");
		goto failed;
	}

	return 0;

failed:
	if (fbdev->fb_mem) {
		dma_free_noncoherent(&dev->dev, fbdev->fb_len, fbdev->fb_mem,
				     fbdev->fb_phys);
	}
	if (fbdev->info.cmap.len != 0) {
		fb_dealloc_cmap(&fbdev->info.cmap);
	}

	return -ENODEV;
}
