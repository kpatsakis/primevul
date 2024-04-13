menu_add_link(WebKitWebView *page, GArray *argv, GString *result) {
    (void) page;
    (void) result;

    add_to_menu(argv, WEBKIT_HIT_TEST_RESULT_CONTEXT_LINK);
}
