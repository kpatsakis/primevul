std::vector<std::wstring> TokenizeString16(const std::wstring& str,
                                           wchar_t delimiter,
                                           bool trim_spaces) {
  return TokenizeStringT<std::wstring>(str, delimiter, trim_spaces);
}
