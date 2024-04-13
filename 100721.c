void ScriptController::bindToWindowObject(Frame* frame, const String& key, NPObject* object)
{
    v8::HandleScope handleScope(m_isolate);

    v8::Handle<v8::Context> v8Context = ScriptController::mainWorldContext(frame);
    if (v8Context.IsEmpty())
        return;

    v8::Context::Scope scope(v8Context);

    v8::Handle<v8::Object> value = createV8ObjectForNPObject(object, 0);

    v8::Handle<v8::Object> global = v8Context->Global();
    global->Set(v8String(key, m_isolate), value);
}
