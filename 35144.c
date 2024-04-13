void handle_cookies (SoupSession *session, SoupMessage *msg, gpointer user_data){
    (void) session;
    (void) user_data;

    soup_message_add_header_handler(msg, "got-headers", "Set-Cookie", G_CALLBACK(save_cookies), NULL);
    GString *s = g_string_new ("");
    SoupURI * soup_uri = soup_message_get_uri(msg);
    g_string_printf(s, "GET '%s' '%s' '%s'", soup_uri->scheme, soup_uri->host, soup_uri->path);
    if(uzbl.behave.cookie_handler)
        run_handler(uzbl.behave.cookie_handler, s->str);

    if(uzbl.behave.cookie_handler &&
            uzbl.comm.sync_stdout && strcmp (uzbl.comm.sync_stdout, "") != 0) {
        char *p = strchr(uzbl.comm.sync_stdout, '\n' );
        if ( p != NULL ) *p = '\0';
        soup_message_headers_replace (msg->request_headers, "Cookie", (const char *) uzbl.comm.sync_stdout);

    }
    if (uzbl.comm.sync_stdout)
        uzbl.comm.sync_stdout = strfree(uzbl.comm.sync_stdout);

    g_string_free(s, TRUE);
}
