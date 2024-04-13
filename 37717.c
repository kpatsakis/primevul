static bool ldm_validate_tocblocks(struct parsed_partitions *state,
				   unsigned long base, struct ldmdb *ldb)
{
	static const int off[4] = { OFF_TOCB1, OFF_TOCB2, OFF_TOCB3, OFF_TOCB4};
	struct tocblock *tb[4];
	struct privhead *ph;
	Sector sect;
	u8 *data;
	int i, nr_tbs;
	bool result = false;

	BUG_ON(!state || !ldb);
	ph = &ldb->ph;
	tb[0] = &ldb->toc;
	tb[1] = kmalloc(sizeof(*tb[1]) * 3, GFP_KERNEL);
	if (!tb[1]) {
		ldm_crit("Out of memory.");
		goto err;
	}
	tb[2] = (struct tocblock*)((u8*)tb[1] + sizeof(*tb[1]));
	tb[3] = (struct tocblock*)((u8*)tb[2] + sizeof(*tb[2]));
	/*
	 * Try to read and parse all four TOCBLOCKs.
	 *
	 * Windows Vista LDM v2.12 does not always have all four TOCBLOCKs so
	 * skip any that fail as long as we get at least one valid TOCBLOCK.
	 */
	for (nr_tbs = i = 0; i < 4; i++) {
		data = read_part_sector(state, base + off[i], &sect);
		if (!data) {
			ldm_error("Disk read failed for TOCBLOCK %d.", i);
			continue;
		}
		if (ldm_parse_tocblock(data, tb[nr_tbs]))
			nr_tbs++;
		put_dev_sector(sect);
	}
	if (!nr_tbs) {
		ldm_crit("Failed to find a valid TOCBLOCK.");
		goto err;
	}
	/* Range check the TOCBLOCK against a privhead. */
	if (((tb[0]->bitmap1_start + tb[0]->bitmap1_size) > ph->config_size) ||
			((tb[0]->bitmap2_start + tb[0]->bitmap2_size) >
			ph->config_size)) {
		ldm_crit("The bitmaps are out of range.  Giving up.");
		goto err;
	}
	/* Compare all loaded TOCBLOCKs. */
	for (i = 1; i < nr_tbs; i++) {
		if (!ldm_compare_tocblocks(tb[0], tb[i])) {
			ldm_crit("TOCBLOCKs 0 and %d do not match.", i);
			goto err;
		}
	}
	ldm_debug("Validated %d TOCBLOCKs successfully.", nr_tbs);
	result = true;
err:
	kfree(tb[1]);
	return result;
}
