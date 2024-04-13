scroll_cmd(WebKitWebView* page, GArray *argv, GString *result) {
    (void) page; (void) result;
    gchar *direction = g_array_index(argv, gchar*, 0);
    gchar *argv1     = g_array_index(argv, gchar*, 1);

    if (g_strcmp0(direction, "horizontal") == 0)
    {
      if (g_strcmp0(argv1, "begin") == 0)
        gtk_adjustment_set_value(uzbl.gui.bar_h, gtk_adjustment_get_lower(uzbl.gui.bar_h));
      else if (g_strcmp0(argv1, "end") == 0)
        gtk_adjustment_set_value (uzbl.gui.bar_h, gtk_adjustment_get_upper(uzbl.gui.bar_h) -
                                  gtk_adjustment_get_page_size(uzbl.gui.bar_h));
      else
        scroll(uzbl.gui.bar_h, argv1);
    }
    else if (g_strcmp0(direction, "vertical") == 0)
    {
      if (g_strcmp0(argv1, "begin") == 0)
        gtk_adjustment_set_value(uzbl.gui.bar_v, gtk_adjustment_get_lower(uzbl.gui.bar_v));
      else if (g_strcmp0(argv1, "end") == 0)
        gtk_adjustment_set_value (uzbl.gui.bar_v, gtk_adjustment_get_upper(uzbl.gui.bar_v) -
                                  gtk_adjustment_get_page_size(uzbl.gui.bar_v));
      else
        scroll(uzbl.gui.bar_v, argv1);
    }
    else
      if(uzbl.state.verbose)
        puts("Unrecognized scroll format");
}
