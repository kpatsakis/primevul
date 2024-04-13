static bool ldm_get_vblks(struct parsed_partitions *state, unsigned long base,
			  struct ldmdb *ldb)
{
	int size, perbuf, skip, finish, s, v, recs;
	u8 *data = NULL;
	Sector sect;
	bool result = false;
	LIST_HEAD (frags);

	BUG_ON(!state || !ldb);

	size   = ldb->vm.vblk_size;
	perbuf = 512 / size;
	skip   = ldb->vm.vblk_offset >> 9;		/* Bytes to sectors */
	finish = (size * ldb->vm.last_vblk_seq) >> 9;

	for (s = skip; s < finish; s++) {		/* For each sector */
		data = read_part_sector(state, base + OFF_VMDB + s, &sect);
		if (!data) {
			ldm_crit ("Disk read failed.");
			goto out;
		}

		for (v = 0; v < perbuf; v++, data+=size) {  /* For each vblk */
			if (MAGIC_VBLK != get_unaligned_be32(data)) {
				ldm_error ("Expected to find a VBLK.");
				goto out;
			}

			recs = get_unaligned_be16(data + 0x0E);	/* Number of records */
			if (recs == 1) {
				if (!ldm_ldmdb_add (data, size, ldb))
					goto out;	/* Already logged */
			} else if (recs > 1) {
				if (!ldm_frag_add (data, size, &frags))
					goto out;	/* Already logged */
			}
			/* else Record is not in use, ignore it. */
		}
		put_dev_sector (sect);
		data = NULL;
	}

	result = ldm_frag_commit (&frags, ldb);	/* Failures, already logged */
out:
	if (data)
		put_dev_sector (sect);
	ldm_frag_free (&frags);

	return result;
}
