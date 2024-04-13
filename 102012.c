std::string PlatformFontSkia::GetActualFontNameForTesting() const {
  SkString family_name;
  typeface_->getFamilyName(&family_name);
  return family_name.c_str();
}
