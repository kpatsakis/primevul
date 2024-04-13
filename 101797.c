void ImageLoader::DecodeRequest::Resolve() {
  resolver_->Resolve();
  loader_ = nullptr;
}
