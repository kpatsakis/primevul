void LinkResolver::resolve_handle_call(CallInfo& result,
                                       const LinkInfo& link_info,
                                       TRAPS) {
  // JSR 292:  this must be an implicitly generated method MethodHandle.invokeExact(*...) or similar
  Klass* resolved_klass = link_info.resolved_klass();
  assert(resolved_klass == SystemDictionary::MethodHandle_klass() ||
         resolved_klass == SystemDictionary::VarHandle_klass(), "");
  assert(MethodHandles::is_signature_polymorphic_name(link_info.name()), "");
  Handle resolved_appendix;
  Handle resolved_method_type;
  methodHandle resolved_method = lookup_polymorphic_method(link_info,
          &resolved_appendix, &resolved_method_type, CHECK);

  if (link_info.check_access()) {
    Symbol* name = link_info.name();
    vmIntrinsics::ID iid = MethodHandles::signature_polymorphic_name_id(name);
    if (MethodHandles::is_signature_polymorphic_intrinsic(iid)) {
      // Check if method can be accessed by the referring class.
      // MH.linkTo* invocations are not rewritten to invokehandle.
      assert(iid == vmIntrinsics::_invokeBasic, "%s", vmIntrinsics::name_at(iid));

      Klass* current_klass = link_info.current_klass();
      assert(current_klass != NULL , "current_klass should not be null");
      check_method_accessability(current_klass,
                                 resolved_klass,
                                 resolved_method->method_holder(),
                                 resolved_method,
                                 CHECK);
    } else {
      // Java code is free to arbitrarily link signature-polymorphic invokers.
      assert(iid == vmIntrinsics::_invokeGeneric, "not an invoker: %s", vmIntrinsics::name_at(iid));
      assert(MethodHandles::is_signature_polymorphic_public_name(resolved_klass, name), "not public");
    }
  }
  result.set_handle(resolved_klass, resolved_method, resolved_appendix, resolved_method_type, CHECK);
}