v8::Local<v8::Value> V8LazyEventListener::callListenerFunction(ScriptState* scriptState, v8::Handle<v8::Value> jsEvent, Event* event)
{
    v8::Local<v8::Object> listenerObject = getListenerObject(scriptState->executionContext());
    if (listenerObject.IsEmpty())
        return v8::Local<v8::Value>();

    v8::Local<v8::Function> handlerFunction = listenerObject.As<v8::Function>();
    v8::Local<v8::Object> receiver = getReceiverObject(scriptState, event);
    if (handlerFunction.IsEmpty() || receiver.IsEmpty())
        return v8::Local<v8::Value>();

    if (!scriptState->executionContext()->isDocument())
        return v8::Local<v8::Value>();

    LocalFrame* frame = toDocument(scriptState->executionContext())->frame();
    if (!frame)
        return v8::Local<v8::Value>();

    if (!frame->script().canExecuteScripts(AboutToExecuteScript))
        return v8::Local<v8::Value>();

    v8::Handle<v8::Value> parameters[1] = { jsEvent };
    return frame->script().callFunction(handlerFunction, receiver, WTF_ARRAY_LENGTH(parameters), parameters);
}
