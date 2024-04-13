void CallInfo::set_resolved_method_name(TRAPS) {
  Method* m = _resolved_method();
  assert(m != NULL, "Should already have a Method*");
  oop rmethod_name = java_lang_invoke_ResolvedMethodName::find_resolved_method(m, CHECK);
  _resolved_method_name = Handle(THREAD, rmethod_name);
}