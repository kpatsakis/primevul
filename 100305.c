bool GetExemplarSetForLang(const std::string& lang,
                           icu::UnicodeSet** lang_set) {
  const LangToExemplarSetMap& map = Singleton<LangToExemplarSet>()->map;
  LangToExemplarSetMap::const_iterator pos = map.find(lang);
  if (pos != map.end()) {
    *lang_set = pos->second;
    return true;
  }
  return false;
}
