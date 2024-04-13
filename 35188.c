settings_init () {
    State *s = &uzbl.state;
    Network *n = &uzbl.net;

    int i;
    for (i = 0; default_config[i].command != NULL; i++) {
        parse_cmd_line(default_config[i].command, NULL);
    }

    if (g_strcmp0(s->config_file, "-") == 0) {
        s->config_file = NULL;
        create_stdin();
    }

    else if (!s->config_file) {
        s->config_file = find_xdg_file (0, "/uzbl/config");
    }

    if (s->config_file) {
        GArray* lines = read_file_by_line (s->config_file);
        int i = 0;
        gchar* line;

        while ((line = g_array_index(lines, gchar*, i))) {
            parse_cmd_line (line, NULL);
            i ++;
            g_free (line);
        }
        g_array_free (lines, TRUE);
    } else {
        if (uzbl.state.verbose)
            printf ("No configuration file loaded.\n");
    }

    if(s->connect_socket_names)
        init_connect_socket();

    g_signal_connect_after(n->soup_session, "request-started", G_CALLBACK(handle_cookies), NULL);
}
