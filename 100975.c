  void SendKeyToPageAndWait(ui::KeyboardCode key) {
    test_delegate_.Reset();
    content::SimulateKeyPress(
        browser()->tab_strip_model()->GetActiveWebContents(),
        key, false, false, false, false);
    test_delegate_.Wait();
  }
