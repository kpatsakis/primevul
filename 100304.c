bool DecodeWord(const std::string& encoded_word,
                const std::string& referrer_charset,
                bool *is_rfc2047,
                std::string* output) {
  if (!IsStringASCII(encoded_word)) {
    if (IsStringUTF8(encoded_word)) {
      *output = encoded_word;
    } else {
      std::wstring wide_output;
      if (!referrer_charset.empty() &&
          base::CodepageToWide(encoded_word, referrer_charset.c_str(),
                               base::OnStringConversionError::FAIL,
                               &wide_output)) {
        *output = WideToUTF8(wide_output);
      } else {
        *output = WideToUTF8(base::SysNativeMBToWide(encoded_word));
      }
    }
    *is_rfc2047 = false;
    return true;
  }

  std::string tmp;
  *is_rfc2047 = true;
  int part_index = 0;
  std::string charset;
  StringTokenizer t(encoded_word, "?");
  RFC2047EncodingType enc_type = Q_ENCODING;
  while (*is_rfc2047 && t.GetNext()) {
    std::string part = t.token();
    switch (part_index) {
      case 0:
        if (part != "=") {
          *is_rfc2047 = false;
          break;
        }
        ++part_index;
        break;
      case 1:
        charset = part;
        ++part_index;
        break;
      case 2:
        if (part.size() > 1 ||
            part.find_first_of("bBqQ") == std::string::npos) {
          *is_rfc2047 = false;
          break;
        }
        if (part[0] == 'b' || part[0] == 'B') {
          enc_type = B_ENCODING;
        }
        ++part_index;
        break;
      case 3:
        *is_rfc2047 = DecodeBQEncoding(part, enc_type, charset, &tmp);
        if (!*is_rfc2047) {
          return false;
        }
        ++part_index;
        break;
      case 4:
        if (part != "=") {
          *is_rfc2047 = false;
          return false;
        }
        ++part_index;
        break;
      default:
        *is_rfc2047 = false;
        return false;
    }
  }

  if (*is_rfc2047) {
    if (*(encoded_word.end() - 1) == '=') {
      output->swap(tmp);
      return true;
    }
    *is_rfc2047 = false;
    return false;
  }


  tmp = UnescapeURLComponent(encoded_word, UnescapeRule::SPACES);
  if (IsStringUTF8(tmp)) {
    output->swap(tmp);
    return true;
  }
  return false;
}
