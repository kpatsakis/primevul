static const char *humanly_readable_name(struct module_entry *m)
{
	/* probed_name may be NULL. modname always exists. */
	return m->probed_name ? m->probed_name : m->modname;
}
