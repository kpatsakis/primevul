  void TestFocusTraversal(RenderViewHost* render_view_host, bool reverse) {
    const char kGetFocusedElementJS[] =
        "window.domAutomationController.send(getFocusedElement());";
    const char* kExpectedIDs[] = { "textEdit", "searchButton", "luckyButton",
                                   "googleLink", "gmailLink", "gmapLink" };
    SCOPED_TRACE(base::StringPrintf("TestFocusTraversal: reverse=%d", reverse));
    ui::KeyboardCode key = ui::VKEY_TAB;
#if defined(OS_MACOSX)
    key = reverse ? ui::VKEY_BACKTAB : ui::VKEY_TAB;
#elif defined(OS_WIN)
    if (base::win::GetVersion() < base::win::VERSION_VISTA)
      return;
#endif

    for (size_t i = 0; i < 2; ++i) {
      SCOPED_TRACE(base::StringPrintf("focus outer loop: %" PRIuS, i));
      ASSERT_TRUE(IsViewFocused(VIEW_ID_OMNIBOX));

#if defined(OS_MACOSX)
      if (ui_controls::IsFullKeyboardAccessEnabled()) {
        ASSERT_TRUE(ui_test_utils::SendKeyPressSync(
            browser(), key, false, reverse, false, false));
        if (reverse) {
          for (int j = 0; j < 3; ++j) {
            ASSERT_TRUE(ui_test_utils::SendKeyPressSync(
                browser(), key, false, reverse, false, false));
          }
        }
      }
#endif

      if (reverse) {
        ASSERT_TRUE(ui_test_utils::SendKeyPressAndWait(
            browser(), key, false, reverse, false, false,
            content::NOTIFICATION_ALL,
            content::NotificationService::AllSources()));
      }

      for (size_t j = 0; j < arraysize(kExpectedIDs); ++j) {
        SCOPED_TRACE(base::StringPrintf("focus inner loop %" PRIuS, j));
        const size_t index = reverse ? arraysize(kExpectedIDs) - 1 - j : j;
        bool is_editable_node = index == 0;

        ASSERT_TRUE(ui_test_utils::SendKeyPressAndWaitWithDetails(
            browser(), key, false, reverse, false, false,
            content::NOTIFICATION_FOCUS_CHANGED_IN_PAGE,
            content::Source<RenderViewHost>(render_view_host),
            content::Details<bool>(&is_editable_node)));
        std::string focused_id;
        EXPECT_TRUE(content::ExecuteScriptAndExtractString(
            render_view_host, kGetFocusedElementJS, &focused_id));
        EXPECT_STREQ(kExpectedIDs[index], focused_id.c_str());
      }

#if defined(OS_MACOSX)
      chrome::FocusLocationBar(browser());
#else
      ASSERT_TRUE(ui_test_utils::SendKeyPressAndWait(
          browser(), key, false, reverse, false, false,
          chrome::NOTIFICATION_FOCUS_RETURNED_TO_BROWSER,
          content::Source<Browser>(browser())));
      EXPECT_TRUE(
          IsViewFocused(reverse ? VIEW_ID_OMNIBOX : VIEW_ID_LOCATION_ICON));

      ASSERT_TRUE(ui_test_utils::SendKeyPressAndWait(
          browser(), key, false, reverse, false, false,
          content::NOTIFICATION_ALL,
          content::NotificationService::AllSources()));
#endif
      content::RunAllPendingInMessageLoop();
      EXPECT_TRUE(
          IsViewFocused(reverse ? VIEW_ID_LOCATION_ICON : VIEW_ID_OMNIBOX));
      if (reverse) {
        ASSERT_TRUE(ui_test_utils::SendKeyPressAndWait(
            browser(), key, false, false, false, false,
            content::NOTIFICATION_ALL,
            content::NotificationService::AllSources()));
      }
    }
  }
