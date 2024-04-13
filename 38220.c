static int au1200_setlocation (struct au1200fb_device *fbdev, int plane,
	int xpos, int ypos)
{
	uint32 winctrl0, winctrl1, winenable, fb_offset = 0;
	int xsz, ysz;

	/* FIX!!! NOT CHECKING FOR COMPLETE OFFSCREEN YET */

	winctrl0 = lcd->window[plane].winctrl0;
	winctrl1 = lcd->window[plane].winctrl1;
	winctrl0 &= (LCD_WINCTRL0_A | LCD_WINCTRL0_AEN);
	winctrl1 &= ~(LCD_WINCTRL1_SZX | LCD_WINCTRL1_SZY);

	/* Check for off-screen adjustments */
	xsz = win->w[plane].xres;
	ysz = win->w[plane].yres;
	if ((xpos + win->w[plane].xres) > panel->Xres) {
		/* Off-screen to the right */
		xsz = panel->Xres - xpos; /* off by 1 ??? */
		/*printk("off screen right\n");*/
	}

	if ((ypos + win->w[plane].yres) > panel->Yres) {
		/* Off-screen to the bottom */
		ysz = panel->Yres - ypos; /* off by 1 ??? */
		/*printk("off screen bottom\n");*/
	}

	if (xpos < 0) {
		/* Off-screen to the left */
		xsz = win->w[plane].xres + xpos;
		fb_offset += (((0 - xpos) * winbpp(lcd->window[plane].winctrl1))/8);
		xpos = 0;
		/*printk("off screen left\n");*/
	}

	if (ypos < 0) {
		/* Off-screen to the top */
		ysz = win->w[plane].yres + ypos;
		/* fixme: fb_offset += ((0-ypos)*fb_pars[plane].line_length); */
		ypos = 0;
		/*printk("off screen top\n");*/
	}

	/* record settings */
	win->w[plane].xpos = xpos;
	win->w[plane].ypos = ypos;

	xsz -= 1;
	ysz -= 1;
	winctrl0 |= (xpos << 21);
	winctrl0 |= (ypos << 10);
	winctrl1 |= (xsz << 11);
	winctrl1 |= (ysz << 0);

	/* Disable the window while making changes, then restore WINEN */
	winenable = lcd->winenable & (1 << plane);
	au_sync();
	lcd->winenable &= ~(1 << plane);
	lcd->window[plane].winctrl0 = winctrl0;
	lcd->window[plane].winctrl1 = winctrl1;
	lcd->window[plane].winbuf0 =
	lcd->window[plane].winbuf1 = fbdev->fb_phys;
	lcd->window[plane].winbufctrl = 0; /* select winbuf0 */
	lcd->winenable |= winenable;
	au_sync();

	return 0;
}
