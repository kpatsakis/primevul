void CallInfo::set_handle(const methodHandle& resolved_method,
                          Handle resolved_appendix,
                          Handle resolved_method_type, TRAPS) {
  set_handle(SystemDictionary::MethodHandle_klass(), resolved_method, resolved_appendix, resolved_method_type, CHECK);
}