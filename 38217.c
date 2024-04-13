int au1100fb_setmode(struct au1100fb_device *fbdev)
{
	struct fb_info *info = &fbdev->info;
	u32 words;
	int index;

	if (!fbdev)
		return -EINVAL;

	/* Update var-dependent FB info */
	if (panel_is_active(fbdev->panel) || panel_is_color(fbdev->panel)) {
		if (info->var.bits_per_pixel <= 8) {
			/* palettized */
			info->var.red.offset    = 0;
			info->var.red.length    = info->var.bits_per_pixel;
			info->var.red.msb_right = 0;

			info->var.green.offset  = 0;
			info->var.green.length  = info->var.bits_per_pixel;
			info->var.green.msb_right = 0;

			info->var.blue.offset   = 0;
			info->var.blue.length   = info->var.bits_per_pixel;
			info->var.blue.msb_right = 0;

			info->var.transp.offset = 0;
			info->var.transp.length = 0;
			info->var.transp.msb_right = 0;

			info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
			info->fix.line_length = info->var.xres_virtual /
							(8/info->var.bits_per_pixel);
		} else {
			/* non-palettized */
			index = (fbdev->panel->control_base & LCD_CONTROL_SBPPF_MASK) >> LCD_CONTROL_SBPPF_BIT;
			info->var.red = rgb_bitfields[index][0];
			info->var.green = rgb_bitfields[index][1];
			info->var.blue = rgb_bitfields[index][2];
			info->var.transp = rgb_bitfields[index][3];

			info->fix.visual = FB_VISUAL_TRUECOLOR;
			info->fix.line_length = info->var.xres_virtual << 1; /* depth=16 */
		}
	} else {
		/* mono */
		info->fix.visual = FB_VISUAL_MONO10;
		info->fix.line_length = info->var.xres_virtual / 8;
	}

	info->screen_size = info->fix.line_length * info->var.yres_virtual;
	info->var.rotate = ((fbdev->panel->control_base&LCD_CONTROL_SM_MASK) \
				>> LCD_CONTROL_SM_BIT) * 90;

	/* Determine BPP mode and format */
	fbdev->regs->lcd_control = fbdev->panel->control_base;
	fbdev->regs->lcd_horztiming = fbdev->panel->horztiming;
	fbdev->regs->lcd_verttiming = fbdev->panel->verttiming;
	fbdev->regs->lcd_clkcontrol = fbdev->panel->clkcontrol_base;
	fbdev->regs->lcd_intenable = 0;
	fbdev->regs->lcd_intstatus = 0;
	fbdev->regs->lcd_dmaaddr0 = LCD_DMA_SA_N(fbdev->fb_phys);

	if (panel_is_dual(fbdev->panel)) {
		/* Second panel display seconf half of screen if possible,
		 * otherwise display the same as the first panel */
		if (info->var.yres_virtual >= (info->var.yres << 1)) {
			fbdev->regs->lcd_dmaaddr1 = LCD_DMA_SA_N(fbdev->fb_phys +
							  (info->fix.line_length *
						          (info->var.yres_virtual >> 1)));
		} else {
			fbdev->regs->lcd_dmaaddr1 = LCD_DMA_SA_N(fbdev->fb_phys);
		}
	}

	words = info->fix.line_length / sizeof(u32);
	if (!info->var.rotate || (info->var.rotate == 180)) {
		words *= info->var.yres_virtual;
		if (info->var.rotate /* 180 */) {
			words -= (words % 8); /* should be divisable by 8 */
		}
	}
	fbdev->regs->lcd_words = LCD_WRD_WRDS_N(words);

	fbdev->regs->lcd_pwmdiv = 0;
	fbdev->regs->lcd_pwmhi = 0;

	/* Resume controller */
	fbdev->regs->lcd_control |= LCD_CONTROL_GO;
	mdelay(10);
	au1100fb_fb_blank(VESA_NO_BLANKING, info);

	return 0;
}
