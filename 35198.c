update_title (void) {
    Behaviour *b = &uzbl.behave;
    gchar *parsed;

    if (b->show_status) {
        if (b->title_format_short) {
            parsed = expand(b->title_format_short, 0);
            if (uzbl.gui.main_window)
                gtk_window_set_title (GTK_WINDOW(uzbl.gui.main_window), parsed);
            g_free(parsed);
        }
        if (b->status_format) {
            parsed = expand(b->status_format, 0);
            gtk_label_set_markup(GTK_LABEL(uzbl.gui.mainbar_label), parsed);
            g_free(parsed);
        }
        if (b->status_background) {
            GdkColor color;
            gdk_color_parse (b->status_background, &color);
            if (uzbl.gui.main_window)
                gtk_widget_modify_bg (uzbl.gui.main_window, GTK_STATE_NORMAL, &color);
            else if (uzbl.gui.plug)
                gtk_widget_modify_bg (GTK_WIDGET(uzbl.gui.plug), GTK_STATE_NORMAL, &color);
        }
    } else {
        if (b->title_format_long) {
            parsed = expand(b->title_format_long, 0);
            if (uzbl.gui.main_window)
                gtk_window_set_title (GTK_WINDOW(uzbl.gui.main_window), parsed);
            g_free(parsed);
        }
    }
}
