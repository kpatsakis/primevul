static int au1200fb_ioctl(struct fb_info *info, unsigned int cmd,
                          unsigned long arg)
{
	struct au1200fb_device *fbdev = info->par;
	int plane;
	int val;

	plane = fbinfo2index(info);
	print_dbg("au1200fb: ioctl %d on plane %d\n", cmd, plane);

	if (cmd == AU1200_LCD_FB_IOCTL) {
		struct au1200_lcd_iodata_t iodata;

		if (copy_from_user(&iodata, (void __user *) arg, sizeof(iodata)))
			return -EFAULT;

		print_dbg("FB IOCTL called\n");

		switch (iodata.subcmd) {
		case AU1200_LCD_SET_SCREEN:
			print_dbg("AU1200_LCD_SET_SCREEN\n");
			set_global(cmd, &iodata.global);
			break;

		case AU1200_LCD_GET_SCREEN:
			print_dbg("AU1200_LCD_GET_SCREEN\n");
			get_global(cmd, &iodata.global);
			break;

		case AU1200_LCD_SET_WINDOW:
			print_dbg("AU1200_LCD_SET_WINDOW\n");
			set_window(plane, &iodata.window);
			break;

		case AU1200_LCD_GET_WINDOW:
			print_dbg("AU1200_LCD_GET_WINDOW\n");
			get_window(plane, &iodata.window);
			break;

		case AU1200_LCD_SET_PANEL:
			print_dbg("AU1200_LCD_SET_PANEL\n");
			if ((iodata.global.panel_choice >= 0) &&
					(iodata.global.panel_choice <
					 NUM_PANELS))
			{
				struct panel_settings *newpanel;
				panel_index = iodata.global.panel_choice;
				newpanel = &known_lcd_panels[panel_index];
				au1200_setpanel(newpanel, fbdev->pd);
			}
			break;

		case AU1200_LCD_GET_PANEL:
			print_dbg("AU1200_LCD_GET_PANEL\n");
			iodata.global.panel_choice = panel_index;
			break;

		default:
			return -EINVAL;
		}

		val = copy_to_user((void __user *) arg, &iodata, sizeof(iodata));
		if (val) {
			print_dbg("error: could not copy %d bytes\n", val);
			return -EFAULT;
		}
	}

	return 0;
}
