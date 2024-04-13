bool GetValueFromVersionResource(const char* version_resource,
                                 const std::wstring& name,
                                 std::wstring* value_str) {
  assert(value_str);
  value_str->clear();

  WORD language = 0;
  WORD code_page = 0;
  if (!GetLanguageAndCodePageFromVersionResource(version_resource, &language,
                                                 &code_page)) {
    return false;
  }

  const size_t array_size = 8;
  WORD lang_codepage[array_size] = {};
  size_t i = 0;
  lang_codepage[i++] = language;
  lang_codepage[i++] = code_page;
  lang_codepage[i++] = ::GetUserDefaultLangID();
  lang_codepage[i++] = code_page;
  lang_codepage[i++] = language;
  lang_codepage[i++] = 1252;
  lang_codepage[i++] = ::GetUserDefaultLangID();
  lang_codepage[i++] = 1252;

  static_assert((array_size % 2) == 0,
                "Language code page size should be a multiple of 2");
  assert(array_size == i);

  for (i = 0; i < array_size;) {
    wchar_t sub_block[MAX_PATH];
    WORD language = lang_codepage[i++];
    WORD code_page = lang_codepage[i++];
    _snwprintf_s(sub_block, MAX_PATH, MAX_PATH,
                 L"\\StringFileInfo\\%04hx%04hx\\%ls", language, code_page,
                 name.c_str());
    void* value = nullptr;
    uint32_t size = 0;
    BOOL r = ::VerQueryValueW(version_resource, sub_block, &value, &size);
    if (r && value) {
      value_str->assign(static_cast<wchar_t*>(value));
      return true;
    }
  }
  return false;
}
