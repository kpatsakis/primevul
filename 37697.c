static void ldm_frag_free (struct list_head *list)
{
	struct list_head *item, *tmp;

	BUG_ON (!list);

	list_for_each_safe (item, tmp, list)
		kfree (list_entry (item, struct frag, list));
}
