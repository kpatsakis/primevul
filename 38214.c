void au1100fb_fb_rotate(struct fb_info *fbi, int angle)
{
	struct au1100fb_device *fbdev = to_au1100fb_device(fbi);

	print_dbg("fb_rotate %p %d", fbi, angle);

	if (fbdev && (angle > 0) && !(angle % 90)) {

		fbdev->regs->lcd_control &= ~LCD_CONTROL_GO;

		fbdev->regs->lcd_control &= ~(LCD_CONTROL_SM_MASK);
		fbdev->regs->lcd_control |= ((angle/90) << LCD_CONTROL_SM_BIT);

		fbdev->regs->lcd_control |= LCD_CONTROL_GO;
	}
}
