int ldm_partition(struct parsed_partitions *state)
{
	struct ldmdb  *ldb;
	unsigned long base;
	int result = -1;

	BUG_ON(!state);

	/* Look for signs of a Dynamic Disk */
	if (!ldm_validate_partition_table(state))
		return 0;

	ldb = kmalloc (sizeof (*ldb), GFP_KERNEL);
	if (!ldb) {
		ldm_crit ("Out of memory.");
		goto out;
	}

	/* Parse and check privheads. */
	if (!ldm_validate_privheads(state, &ldb->ph))
		goto out;		/* Already logged */

	/* All further references are relative to base (database start). */
	base = ldb->ph.config_start;

	/* Parse and check tocs and vmdb. */
	if (!ldm_validate_tocblocks(state, base, ldb) ||
	    !ldm_validate_vmdb(state, base, ldb))
	    	goto out;		/* Already logged */

	/* Initialize vblk lists in ldmdb struct */
	INIT_LIST_HEAD (&ldb->v_dgrp);
	INIT_LIST_HEAD (&ldb->v_disk);
	INIT_LIST_HEAD (&ldb->v_volu);
	INIT_LIST_HEAD (&ldb->v_comp);
	INIT_LIST_HEAD (&ldb->v_part);

	if (!ldm_get_vblks(state, base, ldb)) {
		ldm_crit ("Failed to read the VBLKs from the database.");
		goto cleanup;
	}

	/* Finally, create the data partition devices. */
	if (ldm_create_data_partitions(state, ldb)) {
		ldm_debug ("Parsed LDM database successfully.");
		result = 1;
	}
	/* else Already logged */

cleanup:
	ldm_free_vblks (&ldb->v_dgrp);
	ldm_free_vblks (&ldb->v_disk);
	ldm_free_vblks (&ldb->v_volu);
	ldm_free_vblks (&ldb->v_comp);
	ldm_free_vblks (&ldb->v_part);
out:
	kfree (ldb);
	return result;
}
