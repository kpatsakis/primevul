void Instance::LoadPreviewUrl(const std::string& url) {
  LoadUrlInternal(url, &embed_preview_loader_, &Instance::DidOpenPreview);
}
