test_print (void) {
    GString *result = g_string_new("");

    /* a simple message can be returned as a result */
    parse_cmd_line("print A simple test", result);
    g_assert_cmpstr("A simple test", ==, result->str);

    /* arguments to print should be expanded */
    parse_cmd_line("print A simple @(echo expansion)@ test", result);
    g_assert_cmpstr("A simple expansion test", ==, result->str);

    g_string_free(result, TRUE);
}
