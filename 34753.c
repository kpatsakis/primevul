static struct module_entry *helper_get_module(const char *module, int create)
{
	char modname[MODULE_NAME_LEN];
	struct module_entry *e;
	llist_t *l;
	unsigned i;
	unsigned hash;

	filename2modname(module, modname);

	hash = 0;
	for (i = 0; modname[i]; i++)
		hash = ((hash << 5) + hash) + modname[i];
	hash %= DB_HASH_SIZE;

	for (l = G.db[hash]; l; l = l->link) {
		e = (struct module_entry *) l->data;
		if (strcmp(e->modname, modname) == 0)
			return e;
	}
	if (!create)
		return NULL;

	e = xzalloc(sizeof(*e));
	e->modname = xstrdup(modname);
	llist_add_to(&G.db[hash], e);

	return e;
}
