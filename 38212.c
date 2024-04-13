static int au1100fb_fb_blank(int blank_mode, struct fb_info *fbi)
{
	struct au1100fb_device *fbdev = to_au1100fb_device(fbi);

	print_dbg("fb_blank %d %p", blank_mode, fbi);

	switch (blank_mode) {

	case VESA_NO_BLANKING:
		/* Turn on panel */
		fbdev->regs->lcd_control |= LCD_CONTROL_GO;
		au_sync();
		break;

	case VESA_VSYNC_SUSPEND:
	case VESA_HSYNC_SUSPEND:
	case VESA_POWERDOWN:
		/* Turn off panel */
		fbdev->regs->lcd_control &= ~LCD_CONTROL_GO;
		au_sync();
		break;
	default:
		break;

	}
	return 0;
}
