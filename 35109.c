test_toggle_status (void) {
    g_assert(!uzbl.behave.show_status);

    /* status bar can be toggled on */
    parse_cmd_line("toggle_status", NULL);
    g_assert(uzbl.behave.show_status);

    /* status bar can be toggled back off */
    parse_cmd_line("toggle_status", NULL);
    g_assert(!uzbl.behave.show_status);
}
