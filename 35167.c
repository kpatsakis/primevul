new_window_load_uri (const gchar * uri) {
    if (uzbl.behave.new_window) {
        GString *s = g_string_new ("");
        g_string_printf(s, "'%s'", uri);
        run_handler(uzbl.behave.new_window, s->str);
        send_event(NEW_WINDOW, s->str, NULL);
        return;
    }
    GString* to_execute = g_string_new ("");
    g_string_append_printf (to_execute, "%s --uri '%s'", uzbl.state.executable_path, uri);
    int i;
    for (i = 0; entries[i].long_name != NULL; i++) {
        if ((entries[i].arg == G_OPTION_ARG_STRING) &&
                !strcmp(entries[i].long_name,"uri") &&
                !strcmp(entries[i].long_name,"name")) {
            gchar** str = (gchar**)entries[i].arg_data;
            if (*str!=NULL)
                g_string_append_printf (to_execute, " --%s '%s'", entries[i].long_name, *str);
        }
        else if(entries[i].arg == G_OPTION_ARG_STRING_ARRAY) {
            int j;
            gchar **str = *((gchar ***)entries[i].arg_data);
            for(j=0; str[j]; j++)
                g_string_append_printf(to_execute, " --%s '%s'", entries[i].long_name, str[j]);
        }
    }
    if (uzbl.state.verbose)
        printf("\n%s\n", to_execute->str);
    g_spawn_command_line_async (to_execute->str, NULL);
    /* TODO: should we just report the uri as event detail? */
    send_event(NEW_WINDOW, to_execute->str, NULL);
    g_string_free (to_execute, TRUE);
}
