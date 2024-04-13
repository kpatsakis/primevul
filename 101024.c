int Instance::GetDocumentPixelHeight() const {
  return static_cast<int>(ceil(document_size_.height() *
                               zoom_ *
                               device_scale_));
}
