void WaitForAccessibilityFocusChange() {
  scoped_refptr<content::MessageLoopRunner> loop_runner(
      new content::MessageLoopRunner);
  BrowserAccessibilityManager::SetFocusChangeCallbackForTesting(
      loop_runner->QuitClosure());
  loop_runner->Run();
}
