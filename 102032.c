  void TestStoreInUnloadHandlerForCrossSiteNavigation(
      bool preestablish_mojo_pipe) {
    const GURL a_url = https_test_server().GetURL("a.com", "/title1.html");
    const GURL b_url = https_test_server().GetURL("b.com", "/title2.html");

    ui_test_utils::NavigateToURL(browser(), a_url);

    ChromePasswordManagerClient* client =
        ChromePasswordManagerClient::FromWebContents(WebContents());

    if (preestablish_mojo_pipe) {
      EXPECT_FALSE(client->has_binding_for_credential_manager());
      ASSERT_NO_FATAL_FAILURE(
          TriggerNavigatorGetPasswordCredentialsAndExpectHasResult(
              WebContents(), false));
      EXPECT_TRUE(client->has_binding_for_credential_manager());
    }

    ASSERT_NO_FATAL_FAILURE(ScheduleNavigatorStoreCredentialAtUnload(
        WebContents(), "user", "hunter2"));

    content::RenderFrameDeletedObserver rfh_destruction_observer(
        WebContents()->GetMainFrame());
    ui_test_utils::NavigateToURL(browser(), b_url);

    rfh_destruction_observer.WaitUntilDeleted();
    EXPECT_FALSE(client->has_binding_for_credential_manager());
    EXPECT_FALSE(client->was_store_ever_called());
  }
