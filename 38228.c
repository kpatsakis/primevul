static int au1200fb_fb_check_var(struct fb_var_screeninfo *var,
	struct fb_info *fbi)
{
	struct au1200fb_device *fbdev = fbi->par;
	u32 pixclock;
	int screen_size, plane;

	plane = fbdev->plane;

	/* Make sure that the mode respect all LCD controller and
	 * panel restrictions. */
	var->xres = win->w[plane].xres;
	var->yres = win->w[plane].yres;

	/* No need for virtual resolution support */
	var->xres_virtual = var->xres;
	var->yres_virtual = var->yres;

	var->bits_per_pixel = winbpp(win->w[plane].mode_winctrl1);

	screen_size = var->xres_virtual * var->yres_virtual;
	if (var->bits_per_pixel > 8) screen_size *= (var->bits_per_pixel / 8);
	else screen_size /= (8/var->bits_per_pixel);

	if (fbdev->fb_len < screen_size)
		return -EINVAL; /* Virtual screen is to big, abort */

	/* FIX!!!! what are the implicaitons of ignoring this for windows ??? */
	/* The max LCD clock is fixed to 48MHz (value of AUX_CLK). The pixel
	 * clock can only be obtain by dividing this value by an even integer.
	 * Fallback to a slower pixel clock if necessary. */
	pixclock = max((u32)(PICOS2KHZ(var->pixclock) * 1000), fbi->monspecs.dclkmin);
	pixclock = min3(pixclock, fbi->monspecs.dclkmax, (u32)AU1200_LCD_MAX_CLK/2);

	if (AU1200_LCD_MAX_CLK % pixclock) {
		int diff = AU1200_LCD_MAX_CLK % pixclock;
		pixclock -= diff;
	}

	var->pixclock = KHZ2PICOS(pixclock/1000);
#if 0
	if (!panel_is_active(panel)) {
		int pcd = AU1200_LCD_MAX_CLK / (pixclock * 2) - 1;

		if (!panel_is_color(panel)
			&& (panel->control_base & LCD_CONTROL_MPI) && (pcd < 3)) {
			/* STN 8bit mono panel support is up to 6MHz pixclock */
			var->pixclock = KHZ2PICOS(6000);
		} else if (!pcd) {
			/* Other STN panel support is up to 12MHz  */
			var->pixclock = KHZ2PICOS(12000);
		}
	}
#endif
	/* Set bitfield accordingly */
	switch (var->bits_per_pixel) {
		case 16:
		{
			/* 16bpp True color.
			 * These must be set to MATCH WINCTRL[FORM] */
			int idx;
			idx = (win->w[0].mode_winctrl1 & LCD_WINCTRL1_FRM) >> 25;
			var->red    = rgb_bitfields[idx][0];
			var->green  = rgb_bitfields[idx][1];
			var->blue   = rgb_bitfields[idx][2];
			var->transp = rgb_bitfields[idx][3];
			break;
		}

		case 32:
		{
			/* 32bpp True color.
			 * These must be set to MATCH WINCTRL[FORM] */
			int idx;
			idx = (win->w[0].mode_winctrl1 & LCD_WINCTRL1_FRM) >> 25;
			var->red    = rgb_bitfields[idx][0];
			var->green  = rgb_bitfields[idx][1];
			var->blue   = rgb_bitfields[idx][2];
			var->transp = rgb_bitfields[idx][3];
			break;
		}
		default:
			print_dbg("Unsupported depth %dbpp", var->bits_per_pixel);
			return -EINVAL;
	}

	return 0;
}
