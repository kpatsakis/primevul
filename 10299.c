static void __free_resource_entry(void *entry) {
	r_ne_resource_entry *en = (r_ne_resource_entry *)entry;
	free (en->name);
	free (en);
}