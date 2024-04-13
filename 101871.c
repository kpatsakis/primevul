bool DoUserInfo(const CHAR* username_spec,
                const Component& username,
                const CHAR* password_spec,
                const Component& password,
                CanonOutput* output,
                Component* out_username,
                Component* out_password) {
  if (username.len <= 0 && password.len <= 0) {
    *out_username = Component();
    *out_password = Component();
    return true;
  }

  out_username->begin = output->length();
  if (username.len > 0) {
    AppendStringOfType(&username_spec[username.begin], username.len,
                       CHAR_USERINFO, output);
  }
  out_username->len = output->length() - out_username->begin;

  if (password.len > 0) {
    output->push_back(':');
    out_password->begin = output->length();
    AppendStringOfType(&password_spec[password.begin], password.len,
                       CHAR_USERINFO, output);
    out_password->len = output->length() - out_password->begin;
  } else {
    *out_password = Component();
  }

  output->push_back('@');
  return true;
}
