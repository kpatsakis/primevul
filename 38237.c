static void get_global(u_int cmd, struct au1200_lcd_global_regs_t *pdata)
{
	unsigned int hi1, divider;

	pdata->xsize = ((lcd->screen & LCD_SCREEN_SX) >> 19) + 1;
	pdata->ysize = ((lcd->screen & LCD_SCREEN_SY) >> 8) + 1;

	pdata->backcolor = lcd->backcolor;
	pdata->colorkey = lcd->colorkey;
	pdata->mask = lcd->colorkeymsk;

	hi1 = (lcd->pwmhi >> 16) + 1;
	divider = (lcd->pwmdiv & 0x3FFFF) + 1;
	pdata->brightness = ((hi1 << 8) / divider) - 1;
	au_sync();
}
