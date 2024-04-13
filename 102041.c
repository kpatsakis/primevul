 void CrashTab(WebContents* web_contents) {
   RenderProcessHost* rph = web_contents->GetMainFrame()->GetProcess();
   RenderProcessHostWatcher watcher(
      rph, RenderProcessHostWatcher::WATCH_FOR_PROCESS_EXIT);
  rph->Shutdown(0);
  watcher.Wait();
}
