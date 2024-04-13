static void ldm_free_vblks (struct list_head *lh)
{
	struct list_head *item, *tmp;

	BUG_ON (!lh);

	list_for_each_safe (item, tmp, lh)
		kfree (list_entry (item, struct vblk, list));
}
