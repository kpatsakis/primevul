static struct vblk * ldm_get_disk_objid (const struct ldmdb *ldb)
{
	struct list_head *item;

	BUG_ON (!ldb);

	list_for_each (item, &ldb->v_disk) {
		struct vblk *v = list_entry (item, struct vblk, list);
		if (!memcmp (v->vblk.disk.disk_id, ldb->ph.disk_id, GUID_SIZE))
			return v;
	}

	return NULL;
}
