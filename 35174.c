retrieve_geometry() {
    int w, h, x, y;
    GString *buf = g_string_new("");

    gtk_window_get_size(GTK_WINDOW(uzbl.gui.main_window), &w, &h);
    gtk_window_get_position(GTK_WINDOW(uzbl.gui.main_window), &x, &y);

    g_string_printf(buf, "%dx%d+%d+%d", w, h, x, y);

    if(uzbl.gui.geometry)
        g_free(uzbl.gui.geometry);
    uzbl.gui.geometry = g_string_free(buf, FALSE);
}
