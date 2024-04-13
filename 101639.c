void OpenURLBlockUntilNavigationComplete(Shell* shell, const GURL& url) {
  WaitForLoadStop(shell->web_contents());
  TestNavigationObserver same_tab_observer(shell->web_contents(), 1);

  OpenURLParams params(
      url,
      content::Referrer(shell->web_contents()->GetLastCommittedURL(),
                        blink::kWebReferrerPolicyAlways),
      WindowOpenDisposition::CURRENT_TAB, ui::PAGE_TRANSITION_LINK,
      true /* is_renderer_initiated */);
  shell->OpenURLFromTab(shell->web_contents(), params);

  same_tab_observer.Wait();
}
