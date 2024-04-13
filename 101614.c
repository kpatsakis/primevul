Response PageHandler::SetWebLifecycleState(const std::string& state) {
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents)
    return Response::Error("Not attached to a page");
  if (state == Page::SetWebLifecycleState::StateEnum::Frozen) {
    web_contents->WasHidden();
    web_contents->SetPageFrozen(true);
    return Response::OK();
  }
  if (state == Page::SetWebLifecycleState::StateEnum::Active) {
    web_contents->SetPageFrozen(false);
    return Response::OK();
  }
  return Response::Error("Unidentified lifecycle state");
}
