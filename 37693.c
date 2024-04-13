static bool ldm_compare_privheads (const struct privhead *ph1,
				   const struct privhead *ph2)
{
	BUG_ON (!ph1 || !ph2);

	return ((ph1->ver_major          == ph2->ver_major)		&&
		(ph1->ver_minor          == ph2->ver_minor)		&&
		(ph1->logical_disk_start == ph2->logical_disk_start)	&&
		(ph1->logical_disk_size  == ph2->logical_disk_size)	&&
		(ph1->config_start       == ph2->config_start)		&&
		(ph1->config_size        == ph2->config_size)		&&
		!memcmp (ph1->disk_id, ph2->disk_id, GUID_SIZE));
}
