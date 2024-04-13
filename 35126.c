create_browser () {
    GUI *g = &uzbl.gui;

    g->web_view = WEBKIT_WEB_VIEW (webkit_web_view_new ());

    g_object_connect((GObject*)g->web_view,
      "signal::key-press-event",                      (GCallback)key_press_cb,            NULL,
      "signal::key-release-event",                    (GCallback)key_release_cb,          NULL,
      "signal::button-press-event",                   (GCallback)button_press_cb,         NULL,
      "signal::button-release-event",                 (GCallback)button_release_cb,       NULL,
      "signal::title-changed",                        (GCallback)title_change_cb,         NULL,
      "signal::selection-changed",                    (GCallback)selection_changed_cb,    NULL,
      "signal::load-progress-changed",                (GCallback)progress_change_cb,      NULL,
      "signal::load-committed",                       (GCallback)load_commit_cb,          NULL,
      "signal::load-started",                         (GCallback)load_start_cb,           NULL,
      "signal::load-finished",                        (GCallback)load_finish_cb,          NULL,
      "signal::load-error",                           (GCallback)load_error_cb,           NULL,
      "signal::hovering-over-link",                   (GCallback)link_hover_cb,           NULL,
      "signal::navigation-policy-decision-requested", (GCallback)navigation_decision_cb,  NULL,
      "signal::new-window-policy-decision-requested", (GCallback)new_window_cb,           NULL,
      "signal::download-requested",                   (GCallback)download_cb,             NULL,
      "signal::create-web-view",                      (GCallback)create_web_view_cb,      NULL,
      "signal::mime-type-policy-decision-requested",  (GCallback)mime_policy_cb,          NULL,
      "signal::populate-popup",                       (GCallback)populate_popup_cb,       NULL,
      "signal::focus-in-event",                       (GCallback)focus_cb,                NULL,
      "signal::focus-out-event",                      (GCallback)focus_cb,                NULL,
      NULL);
}
