main (int argc, char *argv[]) {
    /* set up tests */
    g_type_init();
    g_test_init(&argc, &argv, NULL);

    g_test_add("/test-command/set-variable",   struct EventFixture, NULL, event_fixture_setup, test_set_variable, event_fixture_teardown);
    g_test_add("/test-command/event",          struct EventFixture, NULL, event_fixture_setup, test_event,        event_fixture_teardown);

    g_test_add_func("/test-command/print",          test_print);
    g_test_add_func("/test-command/scroll",         test_scroll);
    g_test_add_func("/test-command/toggle-status",  test_toggle_status);
    g_test_add_func("/test-command/sync-sh",        test_sync_sh);

    g_test_add_func("/test-command/js",             test_js);

    /* the following aren't really "command" tests, but they're not worth
     * splitting into a separate file yet */
    g_test_add_func("/test-command/run_handler/arg-order",      test_run_handler_arg_order);
    g_test_add_func("/test-command/run_handler/expand",         test_run_handler_expand);

    /* set up uzbl */
    initialize(argc, argv);

    uzbl.state.config_file = "/tmp/uzbl-config";
    uzbl.comm.fifo_path = "/tmp/some-nonexistant-fifo";
    uzbl.comm.socket_path = "/tmp/some-nonexistant-socket";
    uzbl.state.uri = "http://example.org/";
    uzbl.gui.main_title = "Example.Org";

    uzbl.state.instance_name = INSTANCE_NAME;
    uzbl.behave.shell_cmd = "sh -c";

    return g_test_run();
}
