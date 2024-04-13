RList *r_bin_ne_get_imports(r_bin_ne_obj_t *bin) {
	RList *imports = r_list_newf ((RListFree)r_bin_import_free);
	if (!imports) {
		return NULL;
	}
	ut16 off = bin->ne_header->ImportNameTable + bin->header_offset + 1;
	int i;
	for (i = 0; i < bin->ne_header->ModRefs; i++) {
		RBinImport *imp = R_NEW0 (RBinImport);
		if (!imp) {
			break;
		}
		ut8 sz = r_buf_read8_at (bin->buf, off);
		if (!sz) {
			r_bin_import_free (imp);
			break;
		}
		off++;
		char *name = malloc ((ut64)sz + 1);
		if (!name) {
			break;
		}
		r_buf_read_at (bin->buf, off, (ut8 *)name, sz);
		name[sz] = '\0';
		imp->name = name;
		imp->ordinal = i + 1;
		r_list_append (imports, imp);
		off += sz;
	}
	bin->imports = imports;
	return imports;
}