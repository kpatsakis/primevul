search_clear(WebKitWebView *page, GArray *argv, GString *result) {
    (void) argv;
    (void) result;

    webkit_web_view_unmark_text_matches (page);
    if(uzbl.state.searchtx) {
        g_free(uzbl.state.searchtx);
        uzbl.state.searchtx = NULL;
    }
}
