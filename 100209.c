static int bin_fields(RCore *r, int mode, int va) {
	RList *fields;
	RListIter *iter;
	RBinField *field;
	int i = 0;
	RBin *bin = r->bin;

	if (!(fields = r_bin_get_fields (bin))) {
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
	} else if (IS_MODE_RAD (mode)) {
		r_cons_println ("fs header");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_println ("[Header fields]");
	}
	r_list_foreach (fields, iter, field) {
		ut64 addr = rva (bin, field->paddr, field->vaddr, va);

		if (IS_MODE_RAD (mode)) {
			char *n = __filterQuotedShell (field->name);
			r_name_filter (n, -1);
			r_cons_printf ("\"f header.%s 1 0x%08"PFMT64x"\"\n", n, addr);
			if (field->comment && *field->comment) {
				char *e = sdb_encode ((const ut8*)field->comment, -1);
				r_cons_printf ("CCu %s @ 0x%"PFMT64x"\n", e, addr);
				free (e);
				char *f = __filterShell (field->format);
				r_cons_printf ("Cf %d .%s @ 0x%"PFMT64x"\n", field->size, f, addr);
				free (f);
			}
			if (field->format && *field->format) {
				r_cons_printf ("pf.%s %s\n", n, field->format);
			}
			free (n);
		} else if (IS_MODE_JSON (mode)) {
			r_cons_printf ("%s{\"name\":\"%s\","
				"\"vaddr\":%"PFMT64d","
				"\"paddr\":%"PFMT64d,
				iter->p? ",": "",
				field->name,
				field->vaddr,
				field->paddr
				);
			if (field->comment && *field->comment) {
				r_cons_printf (",\"comment\":\"%s\"", field->comment);
			}
			if (field->format && *field->format) {
				r_cons_printf (",\"format\":\"%s\"", field->format);
			}
			char *o = r_core_cmd_strf (r, "pfj.%s@0x%"PFMT64x, field->format, field->vaddr);
			if (o && *o) {
				r_cons_printf (",\"pf\":%s", o);
			}
			free (o);
			r_cons_printf ("}");
		} else if (IS_MODE_NORMAL (mode)) {
			const bool haveComment = (field->comment && *field->comment);
			r_cons_printf ("0x%08"PFMT64x" 0x%08"PFMT64x" %s%s%s\n",
				field->vaddr, field->paddr, field->name,
				haveComment? "; ": "",
				haveComment? field->comment: "");
		}
		i++;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("]");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_printf ("\n%i fields\n", i);
	}

	return true;
}
