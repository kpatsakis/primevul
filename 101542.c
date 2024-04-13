void OmniboxViewViews::AppendDropFormats(
    int* formats,
    std::set<ui::ClipboardFormatType>* format_types) {
  *formats = *formats | ui::OSExchangeData::URL;
}
