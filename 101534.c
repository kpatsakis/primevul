void PDFiumEngine::OnGetPasswordComplete(int32_t result,
                                         const pp::Var& password) {
  getting_password_ = false;

  std::string password_text;
  if (result == PP_OK && password.is_string())
    password_text = password.AsString();
  ContinueLoadingDocument(password_text);
}
