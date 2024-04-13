menu_remove(WebKitWebView *page, GArray *argv, GString *result) {
    (void) page;
    (void) result;

    remove_from_menu(argv, WEBKIT_HIT_TEST_RESULT_CONTEXT_DOCUMENT);
}
