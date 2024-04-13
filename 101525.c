void PDFiumEngine::Form_SubmitForm(IPDF_JSPLATFORM* param,
                                   void* form_data,
                                   int length,
                                   FPDF_WIDESTRING url) {
  std::string url_str =
      base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(url));
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  engine->client_->SubmitForm(url_str, form_data, length);
}
