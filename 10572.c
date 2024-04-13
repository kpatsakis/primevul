methodHandle LinkResolver::lookup_polymorphic_method(
                                             const LinkInfo& link_info,
                                             Handle *appendix_result_or_null,
                                             Handle *method_type_result,
                                             TRAPS) {
  Klass* klass = link_info.resolved_klass();
  Symbol* name = link_info.name();
  Symbol* full_signature = link_info.signature();

  vmIntrinsics::ID iid = MethodHandles::signature_polymorphic_name_id(name);
  if (TraceMethodHandles) {
    ResourceMark rm(THREAD);
    tty->print_cr("lookup_polymorphic_method iid=%s %s.%s%s",
                  vmIntrinsics::name_at(iid), klass->external_name(),
                  name->as_C_string(), full_signature->as_C_string());
  }
  if ((klass == SystemDictionary::MethodHandle_klass() ||
       klass == SystemDictionary::VarHandle_klass()) &&
      iid != vmIntrinsics::_none) {
    if (MethodHandles::is_signature_polymorphic_intrinsic(iid)) {
      // Most of these do not need an up-call to Java to resolve, so can be done anywhere.
      // Do not erase last argument type (MemberName) if it is a static linkTo method.
      bool keep_last_arg = MethodHandles::is_signature_polymorphic_static(iid);
      TempNewSymbol basic_signature =
        MethodHandles::lookup_basic_type_signature(full_signature, keep_last_arg, CHECK_NULL);
      if (TraceMethodHandles) {
        ResourceMark rm(THREAD);
        tty->print_cr("lookup_polymorphic_method %s %s => basic %s",
                      name->as_C_string(),
                      full_signature->as_C_string(),
                      basic_signature->as_C_string());
      }
      methodHandle result = SystemDictionary::find_method_handle_intrinsic(iid,
                                                              basic_signature,
                                                              CHECK_NULL);
      if (result.not_null()) {
        assert(result->is_method_handle_intrinsic(), "MH.invokeBasic or MH.linkTo* intrinsic");
        assert(result->intrinsic_id() != vmIntrinsics::_invokeGeneric, "wrong place to find this");
        assert(basic_signature == result->signature(), "predict the result signature");
        if (TraceMethodHandles) {
          ttyLocker ttyl;
          tty->print("lookup_polymorphic_method => intrinsic ");
          result->print_on(tty);
        }
      }
      return result;
    } else if (iid == vmIntrinsics::_invokeGeneric
               && THREAD->can_call_java()
               && appendix_result_or_null != NULL) {
      // This is a method with type-checking semantics.
      // We will ask Java code to spin an adapter method for it.
      if (!MethodHandles::enabled()) {
        // Make sure the Java part of the runtime has been booted up.
        Klass* natives = SystemDictionary::MethodHandleNatives_klass();
        if (natives == NULL || InstanceKlass::cast(natives)->is_not_initialized()) {
          SystemDictionary::resolve_or_fail(vmSymbols::java_lang_invoke_MethodHandleNatives(),
                                            Handle(),
                                            Handle(),
                                            true,
                                            CHECK_NULL);
        }
      }

      Handle appendix;
      Handle method_type;
      methodHandle result = SystemDictionary::find_method_handle_invoker(
                                                            klass,
                                                            name,
                                                            full_signature,
                                                            link_info.current_klass(),
                                                            &appendix,
                                                            &method_type,
                                                            CHECK_NULL);
      if (TraceMethodHandles) {
        ttyLocker ttyl;
        tty->print("lookup_polymorphic_method => (via Java) ");
        result->print_on(tty);
        tty->print("  lookup_polymorphic_method => appendix = ");
        if (appendix.is_null())  tty->print_cr("(none)");
        else                     appendix->print_on(tty);
      }
      if (result.not_null()) {
#ifdef ASSERT
        ResourceMark rm(THREAD);

        TempNewSymbol basic_signature =
          MethodHandles::lookup_basic_type_signature(full_signature, CHECK_NULL);
        int actual_size_of_params = result->size_of_parameters();
        int expected_size_of_params = ArgumentSizeComputer(basic_signature).size();
        // +1 for MethodHandle.this, +1 for trailing MethodType
        if (!MethodHandles::is_signature_polymorphic_static(iid))  expected_size_of_params += 1;
        if (appendix.not_null())                                   expected_size_of_params += 1;
        if (actual_size_of_params != expected_size_of_params) {
          tty->print_cr("*** basic_signature=%s", basic_signature->as_C_string());
          tty->print_cr("*** result for %s: ", vmIntrinsics::name_at(iid));
          result->print();
        }
        assert(actual_size_of_params == expected_size_of_params,
               "%d != %d", actual_size_of_params, expected_size_of_params);
#endif //ASSERT

        assert(appendix_result_or_null != NULL, "");
        (*appendix_result_or_null) = appendix;
        (*method_type_result)      = method_type;
      }
      return result;
    }
  }
  return NULL;
}