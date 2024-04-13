load_uri (WebKitWebView *web_view, GArray *argv, GString *result) {
    (void) web_view; (void) result;
    load_uri_imp (argv_idx (argv, 0));
}
