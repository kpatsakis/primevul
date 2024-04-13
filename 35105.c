test_run_handler_expand (void) {
    uzbl.net.useragent = "Test uzbl uzr agent";
    run_handler("sync_spawn echo @useragent", "result:");

    assert(uzbl.comm.sync_stdout);

    /* the result should begin with the standard handler arguments */
    gchar *rest = assert_sync_beginswith_stdarg();

    /* the rest of the result should be the arguments passed to run_handler. */
    /* the user-specified arguments to the handler should have been expanded */
    g_assert_cmpstr("result: Test uzbl uzr agent\n", ==, rest);
}
