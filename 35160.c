menu_add_separator_edit(WebKitWebView *page, GArray *argv, GString *result) {
    (void) page;
    (void) result;

    add_separator_to_menu(argv, WEBKIT_HIT_TEST_RESULT_CONTEXT_EDITABLE);
}
