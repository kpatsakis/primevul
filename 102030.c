bool CSSPaintValue::ParseInputArguments(const Document& document) {
  if (input_arguments_invalid_)
    return false;

  if (parsed_input_arguments_ ||
      !RuntimeEnabledFeatures::CSSPaintAPIArgumentsEnabled())
    return true;

  if (!generator_->IsImageGeneratorReady())
    return false;

  const Vector<CSSSyntaxDescriptor>& input_argument_types =
      generator_->InputArgumentTypes();
  if (argument_variable_data_.size() != input_argument_types.size()) {
    input_arguments_invalid_ = true;
    return false;
  }

  parsed_input_arguments_ = new CSSStyleValueVector();

  for (size_t i = 0; i < argument_variable_data_.size(); ++i) {
    DCHECK_EQ(SecureContextMode::kSecureContext,
              document.GetSecureContextMode());
    const CSSValue* parsed_value = argument_variable_data_[i]->ParseForSyntax(
        input_argument_types[i], SecureContextMode::kSecureContext);
    if (!parsed_value) {
      input_arguments_invalid_ = true;
      parsed_input_arguments_ = nullptr;
      return false;
    }
    parsed_input_arguments_->AppendVector(
        StyleValueFactory::CssValueToStyleValueVector(*parsed_value));
  }
  return true;
}
