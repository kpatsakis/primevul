UploadProgress HttpStreamParser::GetUploadProgress() const {
  if (!request_->upload_data_stream)
    return UploadProgress();

  return UploadProgress(request_->upload_data_stream->position(),
                        request_->upload_data_stream->size());
}
