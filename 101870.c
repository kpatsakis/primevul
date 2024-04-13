const CHAR* DoRemoveURLWhitespace(const CHAR* input,
                                  int input_len,
                                  CanonOutputT<CHAR>* buffer,
                                  int* output_len,
                                  bool* potentially_dangling_markup) {
  int found_whitespace = false;
  for (int i = 0; i < input_len; i++) {
    if (!IsRemovableURLWhitespace(input[i]))
      continue;
    found_whitespace = true;
    break;
  }

  if (!found_whitespace) {
    *output_len = input_len;
    return input;
  }

  if (input_len > 5 && input[0] == 'd' && input[1] == 'a' && input[2] == 't' &&
      input[3] == 'a' && input[4] == ':') {
    *output_len = input_len;
    return input;
  }

  for (int i = 0; i < input_len; i++) {
    if (!IsRemovableURLWhitespace(input[i])) {
      if (potentially_dangling_markup && input[i] == 0x3C)
        *potentially_dangling_markup = true;
      buffer->push_back(input[i]);
    }
  }
  *output_len = buffer->length();
  return buffer->data();
}
