static int au1200fb_setup(struct au1200fb_platdata *pd)
{
	char *options = NULL;
	char *this_opt, *endptr;
	int num_panels = ARRAY_SIZE(known_lcd_panels);
	int panel_idx = -1;

	fb_get_options(DRIVER_NAME, &options);

	if (!options)
		goto out;

	while ((this_opt = strsep(&options, ",")) != NULL) {
		/* Panel option - can be panel name,
		 * "bs" for board-switch, or number/index */
		if (!strncmp(this_opt, "panel:", 6)) {
			int i;
			long int li;
			char *endptr;
			this_opt += 6;
			/* First check for index, which allows
			 * to short circuit this mess */
			li = simple_strtol(this_opt, &endptr, 0);
			if (*endptr == '\0')
				panel_idx = (int)li;
			else if (strcmp(this_opt, "bs") == 0)
				panel_idx = pd->panel_index();
			else {
				for (i = 0; i < num_panels; i++) {
					if (!strcmp(this_opt,
						    known_lcd_panels[i].name)) {
						panel_idx = i;
						break;
					}
				}
			}
			if ((panel_idx < 0) || (panel_idx >= num_panels))
				print_warn("Panel %s not supported!", this_opt);
			else
				panel_index = panel_idx;

		} else if (strncmp(this_opt, "nohwcursor", 10) == 0)
			nohwcursor = 1;
		else if (strncmp(this_opt, "devices:", 8) == 0) {
			this_opt += 8;
			device_count = simple_strtol(this_opt, &endptr, 0);
			if ((device_count < 0) ||
			    (device_count > MAX_DEVICE_COUNT))
				device_count = MAX_DEVICE_COUNT;
		} else if (strncmp(this_opt, "wincfg:", 7) == 0) {
			this_opt += 7;
			window_index = simple_strtol(this_opt, &endptr, 0);
			if ((window_index < 0) ||
			    (window_index >= ARRAY_SIZE(windows)))
				window_index = DEFAULT_WINDOW_INDEX;
		} else if (strncmp(this_opt, "off", 3) == 0)
			return 1;
		else
			print_warn("Unsupported option \"%s\"", this_opt);
	}

out:
	return 0;
}
