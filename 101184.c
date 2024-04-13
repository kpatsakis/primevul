v8::Local<v8::Object> Document::wrap(v8::Isolate* isolate, v8::Local<v8::Object> creationContext)
{
    RefPtrWillBeRawPtr<Document> protect(this);

    ASSERT(!DOMDataStore::containsWrapper(this, isolate));

    const WrapperTypeInfo* wrapperType = wrapperTypeInfo();

    if (frame() && frame()->script().initializeMainWorld()) {
        v8::Local<v8::Object> wrapper = DOMDataStore::getWrapper(this, isolate);
        if (!wrapper.IsEmpty())
            return wrapper;
    }

    v8::Local<v8::Object> wrapper = V8DOMWrapper::createWrapper(isolate, creationContext, wrapperType, this);
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    wrapperType->installConditionallyEnabledProperties(wrapper, isolate);
    return associateWithWrapper(isolate, wrapperType, wrapper);
}
