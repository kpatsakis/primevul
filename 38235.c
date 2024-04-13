static void au1200fb_update_fbinfo(struct fb_info *fbi)
{
	/* FIX!!!! This also needs to take the window pixel format into account!!! */

	/* Update var-dependent FB info */
	if (panel_is_color(panel)) {
		if (fbi->var.bits_per_pixel <= 8) {
			/* palettized */
			fbi->fix.visual = FB_VISUAL_PSEUDOCOLOR;
			fbi->fix.line_length = fbi->var.xres_virtual /
				(8/fbi->var.bits_per_pixel);
		} else {
			/* non-palettized */
			fbi->fix.visual = FB_VISUAL_TRUECOLOR;
			fbi->fix.line_length = fbi->var.xres_virtual * (fbi->var.bits_per_pixel / 8);
		}
	} else {
		/* mono FIX!!! mono 8 and 4 bits */
		fbi->fix.visual = FB_VISUAL_MONO10;
		fbi->fix.line_length = fbi->var.xres_virtual / 8;
	}

	fbi->screen_size = fbi->fix.line_length * fbi->var.yres_virtual;
	print_dbg("line length: %d\n", fbi->fix.line_length);
	print_dbg("bits_per_pixel: %d\n", fbi->var.bits_per_pixel);
}
