static bool ldm_create_data_partitions (struct parsed_partitions *pp,
					const struct ldmdb *ldb)
{
	struct list_head *item;
	struct vblk *vb;
	struct vblk *disk;
	struct vblk_part *part;
	int part_num = 1;

	BUG_ON (!pp || !ldb);

	disk = ldm_get_disk_objid (ldb);
	if (!disk) {
		ldm_crit ("Can't find the ID of this disk in the database.");
		return false;
	}

	strlcat(pp->pp_buf, " [LDM]", PAGE_SIZE);

	/* Create the data partitions */
	list_for_each (item, &ldb->v_part) {
		vb = list_entry (item, struct vblk, list);
		part = &vb->vblk.part;

		if (part->disk_id != disk->obj_id)
			continue;

		put_partition (pp, part_num, ldb->ph.logical_disk_start +
				part->start, part->size);
		part_num++;
	}

	strlcat(pp->pp_buf, "\n", PAGE_SIZE);
	return true;
}
