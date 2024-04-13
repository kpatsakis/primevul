 void free_user_ns(struct user_namespace *ns)
{
	struct user_namespace *parent;

	do {
		parent = ns->parent;
		proc_free_inum(ns->proc_inum);
		kmem_cache_free(user_ns_cachep, ns);
		ns = parent;
	} while (atomic_dec_and_test(&parent->count));
}
