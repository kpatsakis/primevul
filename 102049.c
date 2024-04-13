RenderWidgetHost* GetMouseLockWidget(WebContents* web_contents) {
  return static_cast<WebContentsImpl*>(web_contents)->GetMouseLockWidget();
}
