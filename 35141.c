get_click_context() {
    GUI *g = &uzbl.gui;
    WebKitHitTestResult *ht;
    guint context;

    if(!uzbl.state.last_button)
        return -1;

    ht = webkit_web_view_get_hit_test_result(g->web_view, uzbl.state.last_button);
    g_object_get(ht, "context", &context, NULL);

    return (gint)context;
}
