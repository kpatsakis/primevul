static void __free_resource(void *resource) {
	r_ne_resource *res = (r_ne_resource *)resource;
	free (res->name);
	r_list_free (res->entry);
	free (res);
}