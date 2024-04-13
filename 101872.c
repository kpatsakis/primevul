void SetupReplComp(
    void (Replacements<CHAR>::*set)(const CHAR*, const Component&),
    void (Replacements<CHAR>::*clear)(),
    Replacements<CHAR>* rep,
    const CHAR* str) {
  if (str && str[0] == kDeleteComp[0]) {
    (rep->*clear)();
  } else if (str) {
    (rep->*set)(str, Component(0, static_cast<int>(strlen(str))));
  }
}
