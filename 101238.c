void V8Window::postMessageMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DOMWindow* window = V8Window::toImpl(info.Holder());
    LocalDOMWindow* source = callingDOMWindow(info.GetIsolate());

    ASSERT(window);
    UseCounter::countIfNotPrivateScript(info.GetIsolate(), window->frame(), UseCounter::WindowPostMessage);

    ExceptionState exceptionState(ExceptionState::ExecutionContext, "postMessage", "Window", info.Holder(), info.GetIsolate());

    if (!source) {
        exceptionState.throwTypeError("No active calling context exists.");
        exceptionState.throwIfNeeded();
        return;
    }

    MessagePortArray portArray;
    ArrayBufferArray arrayBufferArray;
    int targetOriginArgIndex = 1;
    if (info.Length() > 2) {
        int transferablesArgIndex = 2;
        if (isLegacyTargetOriginDesignation(info[2])) {
            UseCounter::countIfNotPrivateScript(info.GetIsolate(), window->frame(), UseCounter::WindowPostMessageWithLegacyTargetOriginArgument);
            targetOriginArgIndex = 2;
            transferablesArgIndex = 1;
        }
        if (!SerializedScriptValue::extractTransferables(info.GetIsolate(), info[transferablesArgIndex], transferablesArgIndex, portArray, arrayBufferArray, exceptionState)) {
            exceptionState.throwIfNeeded();
            return;
        }
    }
    TOSTRING_VOID(V8StringResource<TreatNullAndUndefinedAsNullString>, targetOrigin, info[targetOriginArgIndex]);

    RefPtr<SerializedScriptValue> message = SerializedScriptValueFactory::instance().create(info[0], &portArray, &arrayBufferArray, exceptionState, info.GetIsolate());
    if (exceptionState.throwIfNeeded())
        return;

    window->postMessage(message.release(), &portArray, targetOrigin, source, exceptionState);
    exceptionState.throwIfNeeded();
}
