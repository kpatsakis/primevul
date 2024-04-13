rtadv_prefix_get (struct list *rplist, struct prefix_ipv6 *p)
{
  struct rtadv_prefix *rprefix;
  
  rprefix = rtadv_prefix_lookup (rplist, p);
  if (rprefix)
    return rprefix;

  rprefix = rtadv_prefix_new ();
  memcpy (&rprefix->prefix, p, sizeof (struct prefix_ipv6));
  listnode_add (rplist, rprefix);

  return rprefix;
}
