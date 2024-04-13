builtins() {
    unsigned int i,
             len = LENGTH(cmdlist);
    GString *command_list = g_string_new("");

    for (i = 0; i < len; i++) {
        g_string_append(command_list, cmdlist[i].key);
        g_string_append_c(command_list, ' ');
    }

    send_event(BUILTINS, command_list->str, NULL);
    g_string_free(command_list, TRUE);
}
