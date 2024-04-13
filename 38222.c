static void au1200_setpanel(struct panel_settings *newpanel,
			    struct au1200fb_platdata *pd)
{
	/*
	 * Perform global setup/init of LCD controller
	 */
	uint32 winenable;

	/* Make sure all windows disabled */
	winenable = lcd->winenable;
	lcd->winenable = 0;
	au_sync();
	/*
	 * Ensure everything is disabled before reconfiguring
	 */
	if (lcd->screen & LCD_SCREEN_SEN) {
		/* Wait for vertical sync period */
		lcd->intstatus = LCD_INT_SS;
		while ((lcd->intstatus & LCD_INT_SS) == 0) {
			au_sync();
		}

		lcd->screen &= ~LCD_SCREEN_SEN;	/*disable the controller*/

		do {
			lcd->intstatus = lcd->intstatus; /*clear interrupts*/
			au_sync();
		/*wait for controller to shut down*/
		} while ((lcd->intstatus & LCD_INT_SD) == 0);

		/* Call shutdown of current panel (if up) */
		/* this must occur last, because if an external clock is driving
		    the controller, the clock cannot be turned off before first
			shutting down the controller.
		 */
		if (pd->panel_shutdown)
			pd->panel_shutdown();
	}

	/* Newpanel == NULL indicates a shutdown operation only */
	if (newpanel == NULL)
		return;

	panel = newpanel;

	printk("Panel(%s), %dx%d\n", panel->name, panel->Xres, panel->Yres);

	/*
	 * Setup clocking if internal LCD clock source (assumes sys_auxpll valid)
	 */
	if (!(panel->mode_clkcontrol & LCD_CLKCONTROL_EXT))
	{
		uint32 sys_clksrc;
		au_writel(panel->mode_auxpll, SYS_AUXPLL);
		sys_clksrc = au_readl(SYS_CLKSRC) & ~0x0000001f;
		sys_clksrc |= panel->mode_toyclksrc;
		au_writel(sys_clksrc, SYS_CLKSRC);
	}

	/*
	 * Configure panel timings
	 */
	lcd->screen = panel->mode_screen;
	lcd->horztiming = panel->mode_horztiming;
	lcd->verttiming = panel->mode_verttiming;
	lcd->clkcontrol = panel->mode_clkcontrol;
	lcd->pwmdiv = panel->mode_pwmdiv;
	lcd->pwmhi = panel->mode_pwmhi;
	lcd->outmask = panel->mode_outmask;
	lcd->fifoctrl = panel->mode_fifoctrl;
	au_sync();

	/* fixme: Check window settings to make sure still valid
	 * for new geometry */
#if 0
	au1200_setlocation(fbdev, 0, win->w[0].xpos, win->w[0].ypos);
	au1200_setlocation(fbdev, 1, win->w[1].xpos, win->w[1].ypos);
	au1200_setlocation(fbdev, 2, win->w[2].xpos, win->w[2].ypos);
	au1200_setlocation(fbdev, 3, win->w[3].xpos, win->w[3].ypos);
#endif
	lcd->winenable = winenable;

	/*
	 * Re-enable screen now that it is configured
	 */
	lcd->screen |= LCD_SCREEN_SEN;
	au_sync();

	/* Call init of panel */
	if (pd->panel_init)
		pd->panel_init();

	/* FIX!!!! not appropriate on panel change!!! Global setup/init */
	lcd->intenable = 0;
	lcd->intstatus = ~0;
	lcd->backcolor = win->mode_backcolor;

	/* Setup Color Key - FIX!!! */
	lcd->colorkey = win->mode_colorkey;
	lcd->colorkeymsk = win->mode_colorkeymsk;

	/* Setup HWCursor - FIX!!! Need to support this eventually */
	lcd->hwc.cursorctrl = 0;
	lcd->hwc.cursorpos = 0;
	lcd->hwc.cursorcolor0 = 0;
	lcd->hwc.cursorcolor1 = 0;
	lcd->hwc.cursorcolor2 = 0;
	lcd->hwc.cursorcolor3 = 0;


#if 0
#define D(X) printk("%25s: %08X\n", #X, X)
	D(lcd->screen);
	D(lcd->horztiming);
	D(lcd->verttiming);
	D(lcd->clkcontrol);
	D(lcd->pwmdiv);
	D(lcd->pwmhi);
	D(lcd->outmask);
	D(lcd->fifoctrl);
	D(lcd->window[0].winctrl0);
	D(lcd->window[0].winctrl1);
	D(lcd->window[0].winctrl2);
	D(lcd->window[0].winbuf0);
	D(lcd->window[0].winbuf1);
	D(lcd->window[0].winbufctrl);
	D(lcd->window[1].winctrl0);
	D(lcd->window[1].winctrl1);
	D(lcd->window[1].winctrl2);
	D(lcd->window[1].winbuf0);
	D(lcd->window[1].winbuf1);
	D(lcd->window[1].winbufctrl);
	D(lcd->window[2].winctrl0);
	D(lcd->window[2].winctrl1);
	D(lcd->window[2].winctrl2);
	D(lcd->window[2].winbuf0);
	D(lcd->window[2].winbuf1);
	D(lcd->window[2].winbufctrl);
	D(lcd->window[3].winctrl0);
	D(lcd->window[3].winctrl1);
	D(lcd->window[3].winctrl2);
	D(lcd->window[3].winbuf0);
	D(lcd->window[3].winbuf1);
	D(lcd->window[3].winbufctrl);
	D(lcd->winenable);
	D(lcd->intenable);
	D(lcd->intstatus);
	D(lcd->backcolor);
	D(lcd->winenable);
	D(lcd->colorkey);
    D(lcd->colorkeymsk);
	D(lcd->hwc.cursorctrl);
	D(lcd->hwc.cursorpos);
	D(lcd->hwc.cursorcolor0);
	D(lcd->hwc.cursorcolor1);
	D(lcd->hwc.cursorcolor2);
	D(lcd->hwc.cursorcolor3);
#endif
}
