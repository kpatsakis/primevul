void PDFiumEngine::OnNewDataAvailable() {
  client_->DocumentLoadProgress(doc_loader_.GetAvailableData(),
                                doc_loader_.document_size());
}
