void LinkResolver::throw_abstract_method_error(const methodHandle& resolved_method,
                                               const methodHandle& selected_method,
                                               Klass *recv_klass, TRAPS) {
  Klass *resolved_klass = resolved_method->method_holder();
  ResourceMark rm(THREAD);
  stringStream ss;

  if (recv_klass != NULL) {
    ss.print("Receiver class %s does not define or inherit an "
             "implementation of the",
             recv_klass->external_name());
  } else {
    ss.print("Missing implementation of");
  }

  assert(resolved_method.not_null(), "Sanity");
  ss.print(" resolved method '%s%s",
           resolved_method->is_abstract() ? "abstract " : "",
           resolved_method->is_private()  ? "private "  : "");
  resolved_method->signature()->print_as_signature_external_return_type(&ss);
  ss.print(" %s(", resolved_method->name()->as_C_string());
  resolved_method->signature()->print_as_signature_external_parameters(&ss);
  ss.print(")' of %s %s.",
           resolved_klass->external_kind(),
           resolved_klass->external_name());

  if (selected_method.not_null() && !(resolved_method == selected_method)) {
    ss.print(" Selected method is '%s%s",
             selected_method->is_abstract() ? "abstract " : "",
             selected_method->is_private()  ? "private "  : "");
    selected_method->print_external_name(&ss);
    ss.print("'.");
  }

  THROW_MSG(vmSymbols::java_lang_AbstractMethodError(), ss.as_string());
}