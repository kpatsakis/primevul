void PageHandler::GetAppManifest(
    std::unique_ptr<GetAppManifestCallback> callback) {
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents || !web_contents->GetManifestManagerHost()) {
    callback->sendFailure(Response::Error("Cannot retrieve manifest"));
    return;
  }
  web_contents->GetManifestManagerHost()->RequestManifestDebugInfo(
      base::BindOnce(&PageHandler::GotManifest, weak_factory_.GetWeakPtr(),
                     std::move(callback)));
}
