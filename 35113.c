add_separator_to_menu(GArray *argv, guint context) {
    GUI *g = &uzbl.gui;
    MenuItem *m;
    gchar *sep_name;

    if(!g->menu_items)
        g->menu_items = g_ptr_array_new();

    if(!argv_idx(argv, 0))
        return;
    else
        sep_name = argv_idx(argv, 0);

    m = malloc(sizeof(MenuItem));
    m->name    = g_strdup(sep_name);
    m->cmd     = NULL;
    m->context = context;
    m->issep   = TRUE;
    g_ptr_array_add(g->menu_items, m);
}
