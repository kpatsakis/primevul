static int parse_cb(struct nl_object *obj, struct nl_parser_param *p)
{
	struct dp_xdata *x = p->pp_arg;

	x->cb(obj, x->arg);
	return 0;
}
