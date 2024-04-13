void r_bin_ne_free(r_bin_ne_obj_t *bin) {
	// r_list_free (bin->imports); // double free
	r_list_free (bin->resources);
	free (bin->entry_table);
	free (bin->ne_header);
	free (bin->resident_name_table);
	free (bin->segment_entries);
	free (bin);
}