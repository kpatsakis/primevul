void LinkResolver::resolve_invokedynamic(CallInfo& result, const constantPoolHandle& pool, int index, TRAPS) {
  Symbol* method_name       = pool->name_ref_at(index);
  Symbol* method_signature  = pool->signature_ref_at(index);
  Klass* current_klass = pool->pool_holder();

  // Resolve the bootstrap specifier (BSM + optional arguments).
  Handle bootstrap_specifier;
  // Check if CallSite has been bound already:
  ConstantPoolCacheEntry* cpce = pool->invokedynamic_cp_cache_entry_at(index);
  int pool_index = cpce->constant_pool_index();

  if (cpce->is_f1_null()) {
    if (cpce->indy_resolution_failed()) {
      ConstantPool::throw_resolution_error(pool,
                                           ResolutionErrorTable::encode_cpcache_index(index),
                                           CHECK);
    }

    // The initial step in Call Site Specifier Resolution is to resolve the symbolic
    // reference to a method handle which will be the bootstrap method for a dynamic
    // call site.  If resolution for the java.lang.invoke.MethodHandle for the bootstrap
    // method fails, then a MethodHandleInError is stored at the corresponding bootstrap
    // method's CP index for the CONSTANT_MethodHandle_info.  So, there is no need to
    // set the indy_rf flag since any subsequent invokedynamic instruction which shares
    // this bootstrap method will encounter the resolution of MethodHandleInError.
    oop bsm_info = pool->resolve_bootstrap_specifier_at(pool_index, THREAD);
    Exceptions::wrap_dynamic_exception(CHECK);
    assert(bsm_info != NULL, "");
    // FIXME: Cache this once per BootstrapMethods entry, not once per CONSTANT_InvokeDynamic.
    bootstrap_specifier = Handle(THREAD, bsm_info);
  }
  if (!cpce->is_f1_null()) {
    methodHandle method(     THREAD, cpce->f1_as_method());
    Handle       appendix(   THREAD, cpce->appendix_if_resolved(pool));
    Handle       method_type(THREAD, cpce->method_type_if_resolved(pool));
    result.set_handle(method, appendix, method_type, THREAD);
    Exceptions::wrap_dynamic_exception(CHECK);
    return;
  }

  if (TraceMethodHandles) {
    ResourceMark rm(THREAD);
    tty->print_cr("resolve_invokedynamic #%d %s %s in %s",
                  ConstantPool::decode_invokedynamic_index(index),
                  method_name->as_C_string(), method_signature->as_C_string(),
                  current_klass->name()->as_C_string());
    tty->print("  BSM info: "); bootstrap_specifier->print();
  }

  resolve_dynamic_call(result, pool_index, bootstrap_specifier, method_name,
                       method_signature, current_klass, THREAD);
  if (HAS_PENDING_EXCEPTION && PENDING_EXCEPTION->is_a(SystemDictionary::LinkageError_klass())) {
    int encoded_index = ResolutionErrorTable::encode_cpcache_index(index);
    bool recorded_res_status = cpce->save_and_throw_indy_exc(pool, pool_index,
                                                             encoded_index,
                                                             pool()->tag_at(pool_index),
                                                             CHECK);
    if (!recorded_res_status) {
      // Another thread got here just before we did.  So, either use the method
      // that it resolved or throw the LinkageError exception that it threw.
      if (!cpce->is_f1_null()) {
        methodHandle method(     THREAD, cpce->f1_as_method());
        Handle       appendix(   THREAD, cpce->appendix_if_resolved(pool));
        Handle       method_type(THREAD, cpce->method_type_if_resolved(pool));
        result.set_handle(method, appendix, method_type, THREAD);
        Exceptions::wrap_dynamic_exception(CHECK);
      } else {
        assert(cpce->indy_resolution_failed(), "Resolution failure flag not set");
        ConstantPool::throw_resolution_error(pool, encoded_index, CHECK);
      }
      return;
    }
    assert(cpce->indy_resolution_failed(), "Resolution failure flag wasn't set");
  }
}