Skeletons IDNSpoofChecker::GetSkeletons(base::StringPiece16 hostname) {
  Skeletons skeletons;
  size_t hostname_length = hostname.length() - (hostname.back() == '.' ? 1 : 0);
  icu::UnicodeString host(FALSE, hostname.data(), hostname_length);
  if (lgc_letters_n_ascii_.span(host, 0, USET_SPAN_CONTAINED) == host.length())
    diacritic_remover_->transliterate(host);
  extra_confusable_mapper_->transliterate(host);

  UErrorCode status = U_ZERO_ERROR;
  icu::UnicodeString ustr_skeleton;

  int32_t u04cf_pos;
  if ((u04cf_pos = host.indexOf(0x4CF)) != -1) {
    icu::UnicodeString host_alt(host);
    size_t length = host_alt.length();
    char16_t* buffer = host_alt.getBuffer(-1);
    for (char16_t* uc = buffer + u04cf_pos; uc < buffer + length; ++uc) {
      if (*uc == 0x4CF)
        *uc = 0x6C;  // Lowercase L
    }
    host_alt.releaseBuffer(length);
    uspoof_getSkeletonUnicodeString(checker_, 0, host_alt, ustr_skeleton,
                                    &status);
    if (U_SUCCESS(status)) {
      std::string skeleton;
      ustr_skeleton.toUTF8String(skeleton);
      skeletons.insert(skeleton);
    }
  }

  uspoof_getSkeletonUnicodeString(checker_, 0, host, ustr_skeleton, &status);
  if (U_SUCCESS(status)) {
    std::string skeleton;
    ustr_skeleton.toUTF8String(skeleton);
    skeletons.insert(skeleton);
  }
  return skeletons;
}
