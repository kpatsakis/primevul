bool IsControlChar(const base::string16& text) {
  const std::string str = base::UTF16ToUTF8(text);
  if (str.length() != 1)
    return false;
  for (const auto& t : kControlCharToKeyEvent) {
    if (str[0] == std::get<0>(t))
      return true;
  }
  return false;
}
