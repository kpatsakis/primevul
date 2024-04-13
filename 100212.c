static void classdump_java(RCore *r, RBinClass *c) {
	RBinField *f;
	RListIter *iter2, *iter3;
	RBinSymbol *sym;
	char *pn = strdup (c->name);
	char *cn = (char *)r_str_rchr (pn, NULL, '/');
	if (cn) {
		*cn = 0;
		cn++;
		r_str_replace_char (pn, '/', '.');
	}
	r_cons_printf ("package %s;\n\n", pn);
	r_cons_printf ("public class %s {\n", cn);
	free (pn);
	r_list_foreach (c->fields, iter2, f) {
		if (f->name && r_regex_match ("ivar","e", f->name)) {
			r_cons_printf ("  public %s %s\n", f->type, f->name);
		}
	}
	r_list_foreach (c->methods, iter3, sym) {
		const char *mn = sym->dname? sym->dname: sym->name;
		const char *ms = strstr (mn, "method.");
		if (ms) {
			mn = ms + strlen ("method.");
		}
		r_cons_printf ("  public %s ();\n", mn);
	}
	r_cons_printf ("}\n\n");
}
