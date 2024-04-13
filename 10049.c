xfrm_policy_lookup(struct net *net, const struct flowi *fl, u16 family,
		   u8 dir, struct flow_cache_object *old_obj, void *ctx)
{
	struct xfrm_policy *pol;

	if (old_obj)
		xfrm_pol_put(container_of(old_obj, struct xfrm_policy, flo));

	pol = __xfrm_policy_lookup(net, fl, family, flow_to_policy_dir(dir));
	if (IS_ERR_OR_NULL(pol))
		return ERR_CAST(pol);

	/* Resolver returns two references:
	 * one for cache and one for caller of flow_cache_lookup() */
	xfrm_pol_hold(pol);

	return &pol->flo;
}