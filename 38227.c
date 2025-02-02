static int au1200fb_fb_blank(int blank_mode, struct fb_info *fbi)
{
	struct au1200fb_device *fbdev = fbi->par;

	/* Short-circuit screen blanking */
	if (noblanking)
		return 0;

	switch (blank_mode) {

	case FB_BLANK_UNBLANK:
	case FB_BLANK_NORMAL:
		/* printk("turn on panel\n"); */
		au1200_setpanel(panel, fbdev->pd);
		break;
	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	case FB_BLANK_POWERDOWN:
		/* printk("turn off panel\n"); */
		au1200_setpanel(NULL, fbdev->pd);
		break;
	default:
		break;

	}

	/* FB_BLANK_NORMAL is a soft blank */
	return (blank_mode == FB_BLANK_NORMAL) ? -EINVAL : 0;
}
