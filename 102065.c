void WaitForInterstitialAttach(content::WebContents* web_contents) {
  if (web_contents->ShowingInterstitialPage())
    return;
  scoped_refptr<content::MessageLoopRunner> loop_runner(
      new content::MessageLoopRunner);
  InterstitialObserver observer(web_contents,
                                loop_runner->QuitClosure(),
                                base::Closure());
  loop_runner->Run();
}
