bool PDFiumEngine::TryLoadingDoc(const std::string& password,
                                 bool* needs_password) {
  *needs_password = false;
  if (doc_) {
    FPDFAvail_IsDocAvail(fpdf_availability_, &download_hints_);
    return true;
  }

  const char* password_cstr = nullptr;
  if (!password.empty()) {
    password_cstr = password.c_str();
    password_tries_remaining_--;
  }
  if (doc_loader_->IsDocumentComplete() &&
      !FPDFAvail_IsLinearized(fpdf_availability_)) {
    doc_ = FPDF_LoadCustomDocument(&file_access_, password_cstr);
  } else {
    doc_ = FPDFAvail_GetDocument(fpdf_availability_, password_cstr);
  }
  if (!doc_) {
    if (FPDF_GetLastError() == FPDF_ERR_PASSWORD)
      *needs_password = true;
    return false;
  }

  FPDFAvail_IsDocAvail(fpdf_availability_, &download_hints_);
  return true;
}
