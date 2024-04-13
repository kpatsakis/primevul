void FileReaderLoader::OnFinishLoading() {
  if (read_type_ != kReadByClient && raw_data_) {
    raw_data_->ShrinkToFit();
    is_raw_data_converted_ = false;
  }

  finished_loading_ = true;

  Cleanup();
  if (client_)
    client_->DidFinishLoading();
}
