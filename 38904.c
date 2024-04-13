static void ip_vs_trash_cleanup(void)
{
	struct ip_vs_dest *dest, *nxt;

	list_for_each_entry_safe(dest, nxt, &ip_vs_dest_trash, n_list) {
		list_del(&dest->n_list);
		ip_vs_dst_reset(dest);
		__ip_vs_unbind_svc(dest);
		kfree(dest);
	}
}
