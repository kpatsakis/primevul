v8::Handle<v8::Object> V8DOMWrapper::lookupDOMWrapper(V8ClassIndex::V8WrapperType type, v8::Handle<v8::Value> value)
{
    if (value.IsEmpty())
        return notHandledByInterceptor();

    v8::Handle<v8::FunctionTemplate> descriptor = getTemplate(type);
    while (value->IsObject()) {
        v8::Handle<v8::Object> object = v8::Handle<v8::Object>::Cast(value);
        if (descriptor->HasInstance(object))
            return object;

        value = object->GetPrototype();
    }
    return notHandledByInterceptor();
} 
