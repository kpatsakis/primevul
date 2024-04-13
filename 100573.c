void Clipboard::DidWriteURL(const std::string& utf8_text) {
  gtk_clipboard_set_text(primary_selection_, utf8_text.c_str(),
                         utf8_text.length());
}
