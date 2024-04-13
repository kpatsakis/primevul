static int fbinfo2index (struct fb_info *fb_info)
{
	int i;

	for (i = 0; i < device_count; ++i) {
		if (fb_info == _au1200fb_infos[i])
			return i;
	}
	printk("au1200fb: ERROR: fbinfo2index failed!\n");
	return -1;
}
