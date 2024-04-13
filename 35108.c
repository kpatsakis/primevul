test_sync_sh (void) {
    parse_cmd_line("sync_sh 'echo Test echo.'", NULL);
    g_assert_cmpstr("Test echo.\n", ==, uzbl.comm.sync_stdout);

    /* clean up after ourselves */
    uzbl.comm.sync_stdout = strfree(uzbl.comm.sync_stdout);
}
