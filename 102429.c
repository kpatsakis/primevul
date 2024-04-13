static Maybe<char16_t> parseUnicodeCodepoint(const char16_t** start, const char16_t* end) {
 char16_t code = 0;
 for (size_t i = 0; i < 4 && *start != end; i++, (*start)++) {
 char16_t c = **start;
 int a;
 if (c >= '0' && c <= '9') {
            a = c - '0';
 } else if (c >= 'a' && c <= 'f') {
            a = c - 'a' + 10;
 } else if (c >= 'A' && c <= 'F') {
            a = c - 'A' + 10;
 } else {
 return make_nothing<char16_t>();
 }
        code = (code << 4) | a;
 }
 return make_value(code);
}
