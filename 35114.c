add_to_menu(GArray *argv, guint context) {
    GUI *g = &uzbl.gui;
    MenuItem *m;
    gchar *item_cmd = NULL;

    if(!argv_idx(argv, 0))
        return;

    gchar **split = g_strsplit(argv_idx(argv, 0), "=", 2);
    if(!g->menu_items)
        g->menu_items = g_ptr_array_new();

    if(split[1])
        item_cmd = g_strdup(split[1]);

    if(split[0]) {
        m = malloc(sizeof(MenuItem));
        m->name = g_strdup(split[0]);
        m->cmd  = g_strdup(item_cmd?item_cmd:"");
        m->context = context;
        m->issep = FALSE;
        g_ptr_array_add(g->menu_items, m);
    }
    else
        g_free(item_cmd);

    g_strfreev(split);
}
