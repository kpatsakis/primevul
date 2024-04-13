main (int argc, char* argv[]) {
    initialize(argc, argv);

    uzbl.gui.scrolled_win = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (uzbl.gui.scrolled_win),
        GTK_POLICY_NEVER, GTK_POLICY_NEVER);

    gtk_container_add (GTK_CONTAINER (uzbl.gui.scrolled_win),
        GTK_WIDGET (uzbl.gui.web_view));

    uzbl.gui.vbox = gtk_vbox_new (FALSE, 0);

    /* initial packing */
    gtk_box_pack_start (GTK_BOX (uzbl.gui.vbox), uzbl.gui.scrolled_win, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (uzbl.gui.vbox), uzbl.gui.mainbar, FALSE, TRUE, 0);

    if (uzbl.state.plug_mode) {
        uzbl.gui.plug = create_plug ();
        gtk_container_add (GTK_CONTAINER (uzbl.gui.plug), uzbl.gui.vbox);
        gtk_widget_show_all (GTK_WIDGET (uzbl.gui.plug));
        /* in xembed mode the window has no unique id and thus
         * socket/fifo names aren't unique either.
         * we use a custom randomizer to create a random id
        */
        struct timeval tv;
        gettimeofday(&tv, NULL);
        srand((unsigned int)tv.tv_sec*tv.tv_usec);
        uzbl.xwin = rand();
    } else {
        uzbl.gui.main_window = create_window ();
        gtk_container_add (GTK_CONTAINER (uzbl.gui.main_window), uzbl.gui.vbox);
        gtk_widget_show_all (uzbl.gui.main_window);
        uzbl.xwin = GDK_WINDOW_XID (GTK_WIDGET (uzbl.gui.main_window)->window);
    }

    uzbl.gui.scbar_v = (GtkScrollbar*) gtk_vscrollbar_new (NULL);
    uzbl.gui.bar_v = gtk_range_get_adjustment((GtkRange*) uzbl.gui.scbar_v);
    uzbl.gui.scbar_h = (GtkScrollbar*) gtk_hscrollbar_new (NULL);
    uzbl.gui.bar_h = gtk_range_get_adjustment((GtkRange*) uzbl.gui.scbar_h);
    gtk_widget_set_scroll_adjustments ((GtkWidget*) uzbl.gui.web_view, uzbl.gui.bar_h, uzbl.gui.bar_v);

    if(!uzbl.state.instance_name)
        uzbl.state.instance_name = itos((int)uzbl.xwin);

    GString *tmp = g_string_new("");
    g_string_printf(tmp, "%d", getpid());
    uzbl.info.pid_str = g_string_free(tmp, FALSE);
    send_event(INSTANCE_START, uzbl.info.pid_str, NULL);

    if(uzbl.state.plug_mode) {
        char *t = itos(gtk_plug_get_id(uzbl.gui.plug));
        send_event(PLUG_CREATED, t, NULL);
        g_free(t);
    }

    /* generate an event with a list of built in commands */
    builtins();

    gtk_widget_grab_focus (GTK_WIDGET (uzbl.gui.web_view));

    if (uzbl.state.verbose) {
        printf("Uzbl start location: %s\n", argv[0]);
        if (uzbl.state.socket_id)
            printf("plug_id %i\n", gtk_plug_get_id(uzbl.gui.plug));
        else
            printf("window_id %i\n",(int) uzbl.xwin);
        printf("pid %i\n", getpid ());
        printf("name: %s\n", uzbl.state.instance_name);
        printf("commit: %s\n", uzbl.info.commit);
    }

    /* Check uzbl is in window mode before getting/setting geometry */
    if (uzbl.gui.main_window) {
        if(uzbl.gui.geometry)
            cmd_set_geometry();
        else
            retrieve_geometry();
    }

    gchar *uri_override = (uzbl.state.uri ? g_strdup(uzbl.state.uri) : NULL);
    if (argc > 1 && !uzbl.state.uri)
        uri_override = g_strdup(argv[1]);
    gboolean verbose_override = uzbl.state.verbose;

    settings_init ();

    if (!uzbl.behave.show_status)
        gtk_widget_hide(uzbl.gui.mainbar);
    else
        update_title();

    /* WebInspector */
    set_up_inspector();

    if (verbose_override > uzbl.state.verbose)
        uzbl.state.verbose = verbose_override;

    if (uri_override) {
        set_var_value("uri", uri_override);
        g_free(uri_override);
    }

    gtk_main ();
    clean_up();

    return EXIT_SUCCESS;
}
