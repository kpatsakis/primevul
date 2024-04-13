pp::Var Instance::GetSelectedText(bool html) {
  if (html || !engine_->HasPermission(PDFEngine::PERMISSION_COPY))
    return pp::Var();
  return engine_->GetSelectedText();
}
