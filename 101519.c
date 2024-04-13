int PDFiumEngine::Form_Browse(IPDF_JSPLATFORM* param,
                              void* file_path,
                              int length) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  std::string path = engine->client_->ShowFileSelectionDialog();
  if (path.size() + 1 <= static_cast<size_t>(length))
    memcpy(file_path, &path[0], path.size() + 1);
  return path.size() + 1;
}
