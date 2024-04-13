std::wstring StripWWW(const std::wstring& text) {
  const std::wstring www(L"www.");
  return (text.compare(0, www.length(), www) == 0) ?
      text.substr(www.length()) : text;
}
