void V8DOMWrapper::setHiddenWindowReference(Frame* frame, const int internalIndex, v8::Handle<v8::Object> jsObject)
{
    if (!frame)
        return; // Object might be detached from window
    v8::Handle<v8::Context> context = getWrapperContext(frame);
    if (context.IsEmpty())
        return;

    ASSERT(internalIndex < V8Custom::kDOMWindowInternalFieldCount);

    v8::Handle<v8::Object> global = context->Global();
    global = V8DOMWrapper::lookupDOMWrapper(V8ClassIndex::DOMWINDOW, global);
    ASSERT(!global.IsEmpty());
    ASSERT(global->GetInternalField(internalIndex)->IsUndefined());
    global->SetInternalField(internalIndex, jsObject);
}
