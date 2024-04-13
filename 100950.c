void setFixedFontFamilyWrapper(WebSettings* settings,
                               const string16& font,
                               UScriptCode script) {
  settings->setFixedFontFamily(font, script);
}
