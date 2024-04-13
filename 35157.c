menu_add_image(WebKitWebView *page, GArray *argv, GString *result) {
    (void) page;
    (void) result;

    add_to_menu(argv, WEBKIT_HIT_TEST_RESULT_CONTEXT_IMAGE);
}
