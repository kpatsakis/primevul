static bool ldm_validate_privheads(struct parsed_partitions *state,
				   struct privhead *ph1)
{
	static const int off[3] = { OFF_PRIV1, OFF_PRIV2, OFF_PRIV3 };
	struct privhead *ph[3] = { ph1 };
	Sector sect;
	u8 *data;
	bool result = false;
	long num_sects;
	int i;

	BUG_ON (!state || !ph1);

	ph[1] = kmalloc (sizeof (*ph[1]), GFP_KERNEL);
	ph[2] = kmalloc (sizeof (*ph[2]), GFP_KERNEL);
	if (!ph[1] || !ph[2]) {
		ldm_crit ("Out of memory.");
		goto out;
	}

	/* off[1 & 2] are relative to ph[0]->config_start */
	ph[0]->config_start = 0;

	/* Read and parse privheads */
	for (i = 0; i < 3; i++) {
		data = read_part_sector(state, ph[0]->config_start + off[i],
					&sect);
		if (!data) {
			ldm_crit ("Disk read failed.");
			goto out;
		}
		result = ldm_parse_privhead (data, ph[i]);
		put_dev_sector (sect);
		if (!result) {
			ldm_error ("Cannot find PRIVHEAD %d.", i+1); /* Log again */
			if (i < 2)
				goto out;	/* Already logged */
			else
				break;	/* FIXME ignore for now, 3rd PH can fail on odd-sized disks */
		}
	}

	num_sects = state->bdev->bd_inode->i_size >> 9;

	if ((ph[0]->config_start > num_sects) ||
	   ((ph[0]->config_start + ph[0]->config_size) > num_sects)) {
		ldm_crit ("Database extends beyond the end of the disk.");
		goto out;
	}

	if ((ph[0]->logical_disk_start > ph[0]->config_start) ||
	   ((ph[0]->logical_disk_start + ph[0]->logical_disk_size)
		    > ph[0]->config_start)) {
		ldm_crit ("Disk and database overlap.");
		goto out;
	}

	if (!ldm_compare_privheads (ph[0], ph[1])) {
		ldm_crit ("Primary and backup PRIVHEADs don't match.");
		goto out;
	}
	/* FIXME ignore this for now
	if (!ldm_compare_privheads (ph[0], ph[2])) {
		ldm_crit ("Primary and backup PRIVHEADs don't match.");
		goto out;
	}*/
	ldm_debug ("Validated PRIVHEADs successfully.");
	result = true;
out:
	kfree (ph[1]);
	kfree (ph[2]);
	return result;
}
