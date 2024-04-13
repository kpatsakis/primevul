remove_from_menu(GArray *argv, guint context) {
    GUI *g = &uzbl.gui;
    MenuItem *mi;
    gchar *name = NULL;
    guint i=0;

    if(!g->menu_items)
        return;

    if(!argv_idx(argv, 0))
        return;
    else
        name = argv_idx(argv, 0);

    for(i=0; i < g->menu_items->len; i++) {
        mi = g_ptr_array_index(g->menu_items, i);

        if((context == mi->context) && !strcmp(name, mi->name)) {
            g_free(mi->name);
            g_free(mi->cmd);
            g_free(mi);
            g_ptr_array_remove_index(g->menu_items, i);
        }
    }
}
