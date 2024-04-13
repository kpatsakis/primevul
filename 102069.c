ModuleSystem::ModuleSystem(ScriptContext* context, SourceMap* source_map)
    : ObjectBackedNativeHandler(context),
      context_(context),
      source_map_(source_map),
      natives_enabled_(0),
      exception_handler_(new DefaultExceptionHandler(context)),
      weak_factory_(this) {
  RouteFunction(
      "require",
      base::Bind(&ModuleSystem::RequireForJs, base::Unretained(this)));
  RouteFunction(
      "requireNative",
      base::Bind(&ModuleSystem::RequireNative, base::Unretained(this)));
  RouteFunction(
      "requireAsync",
      base::Bind(&ModuleSystem::RequireAsync, base::Unretained(this)));
  RouteFunction("privates",
                base::Bind(&ModuleSystem::Private, base::Unretained(this)));

  v8::Local<v8::Object> global(context->v8_context()->Global());
  v8::Isolate* isolate = context->isolate();
  SetPrivate(global, kModulesField, v8::Object::New(isolate));
  SetPrivate(global, kModuleSystem, v8::External::New(isolate, this));

  gin::ModuleRegistry::From(context->v8_context())->AddObserver(this);
  if (context_->GetRenderFrame()) {
    context_->GetRenderFrame()->EnsureMojoBuiltinsAreAvailable(
        context->isolate(), context->v8_context());
  }
}
