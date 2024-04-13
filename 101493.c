WebString GetFormSignatureAsWebString(const PasswordForm& password_form) {
  return WebString::FromUTF8(
      base::NumberToString(CalculateFormSignature(password_form.form_data)));
}
