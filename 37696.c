static bool ldm_frag_commit (struct list_head *frags, struct ldmdb *ldb)
{
	struct frag *f;
	struct list_head *item;

	BUG_ON (!frags || !ldb);

	list_for_each (item, frags) {
		f = list_entry (item, struct frag, list);

		if (f->map != 0xFF) {
			ldm_error ("VBLK group %d is incomplete (0x%02x).",
				f->group, f->map);
			return false;
		}

		if (!ldm_ldmdb_add (f->data, f->num*ldb->vm.vblk_size, ldb))
			return false;		/* Already logged */
	}
	return true;
}
