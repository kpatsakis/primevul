static bool ldm_ldmdb_add (u8 *data, int len, struct ldmdb *ldb)
{
	struct vblk *vb;
	struct list_head *item;

	BUG_ON (!data || !ldb);

	vb = kmalloc (sizeof (*vb), GFP_KERNEL);
	if (!vb) {
		ldm_crit ("Out of memory.");
		return false;
	}

	if (!ldm_parse_vblk (data, len, vb)) {
		kfree(vb);
		return false;			/* Already logged */
	}

	/* Put vblk into the correct list. */
	switch (vb->type) {
	case VBLK_DGR3:
	case VBLK_DGR4:
		list_add (&vb->list, &ldb->v_dgrp);
		break;
	case VBLK_DSK3:
	case VBLK_DSK4:
		list_add (&vb->list, &ldb->v_disk);
		break;
	case VBLK_VOL5:
		list_add (&vb->list, &ldb->v_volu);
		break;
	case VBLK_CMP3:
		list_add (&vb->list, &ldb->v_comp);
		break;
	case VBLK_PRT3:
		/* Sort by the partition's start sector. */
		list_for_each (item, &ldb->v_part) {
			struct vblk *v = list_entry (item, struct vblk, list);
			if ((v->vblk.part.disk_id == vb->vblk.part.disk_id) &&
			    (v->vblk.part.start > vb->vblk.part.start)) {
				list_add_tail (&vb->list, &v->list);
				return true;
			}
		}
		list_add_tail (&vb->list, &ldb->v_part);
		break;
	}
	return true;
}
