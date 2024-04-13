static void au1200_setmode(struct au1200fb_device *fbdev)
{
	int plane = fbdev->plane;
	/* Window/plane setup */
	lcd->window[plane].winctrl1 = ( 0
		| LCD_WINCTRL1_PRI_N(plane)
		| win->w[plane].mode_winctrl1 /* FRM,CCO,PO,PIPE */
		) ;

	au1200_setlocation(fbdev, plane, win->w[plane].xpos, win->w[plane].ypos);

	lcd->window[plane].winctrl2 = ( 0
		| LCD_WINCTRL2_CKMODE_00
		| LCD_WINCTRL2_DBM
		| LCD_WINCTRL2_BX_N(fbdev->fb_info->fix.line_length)
		| LCD_WINCTRL2_SCX_1
		| LCD_WINCTRL2_SCY_1
		) ;
	lcd->winenable |= win->w[plane].mode_winenable;
	au_sync();
}
