  virtual void TearDown() {
    profile()->set_session_service(NULL);
    session_helper_.set_service(NULL);

    HistoryService* history =
        profile()->GetHistoryService(Profile::IMPLICIT_ACCESS);
    if (history) {
      history->SetOnBackendDestroyTask(new MessageLoop::QuitTask);
      MessageLoop::current()->Run();
    }

    NavigationControllerTest::TearDown();

    ASSERT_TRUE(file_util::Delete(test_dir_, true));
    ASSERT_FALSE(file_util::PathExists(test_dir_));
  }
