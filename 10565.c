void LinkResolver::resolve_dynamic_call(CallInfo& result,
                                        int pool_index,
                                        Handle bootstrap_specifier,
                                        Symbol* method_name, Symbol* method_signature,
                                        Klass* current_klass,
                                        TRAPS) {
  // JSR 292:  this must resolve to an implicitly generated method MH.linkToCallSite(*...)
  // The appendix argument is likely to be a freshly-created CallSite.
  Handle       resolved_appendix;
  Handle       resolved_method_type;
  methodHandle resolved_method =
    SystemDictionary::find_dynamic_call_site_invoker(current_klass,
                                                     pool_index,
                                                     bootstrap_specifier,
                                                     method_name, method_signature,
                                                     &resolved_appendix,
                                                     &resolved_method_type,
                                                     THREAD);
  Exceptions::wrap_dynamic_exception(CHECK);
  result.set_handle(resolved_method, resolved_appendix, resolved_method_type, THREAD);
  Exceptions::wrap_dynamic_exception(CHECK);
}