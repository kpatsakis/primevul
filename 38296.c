struct fib6_node * fib6_lookup(struct fib6_node *root, const struct in6_addr *daddr,
			       const struct in6_addr *saddr)
{
	struct fib6_node *fn;
	struct lookup_args args[] = {
		{
			.offset = offsetof(struct rt6_info, rt6i_dst),
			.addr = daddr,
		},
#ifdef CONFIG_IPV6_SUBTREES
		{
			.offset = offsetof(struct rt6_info, rt6i_src),
			.addr = saddr,
		},
#endif
		{
			.offset = 0,	/* sentinel */
		}
	};

	fn = fib6_lookup_1(root, daddr ? args : args + 1);
	if (!fn || fn->fn_flags & RTN_TL_ROOT)
		fn = root;

	return fn;
}
