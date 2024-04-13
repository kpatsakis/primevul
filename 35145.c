hardcopy(WebKitWebView *page, GArray *argv, GString *result) {
    (void) argv;
    (void) result;

    webkit_web_frame_print(webkit_web_view_get_main_frame(page));
}
