void xs_init_target(struct xtables_target *target)
{
	if (target->udata_size != 0) {
		free(target->udata);
		target->udata = calloc(1, target->udata_size);
		if (target->udata == NULL)
			xtables_error(RESOURCE_PROBLEM, "malloc");
	}
	if (target->init != NULL)
		target->init(target->t);
}
