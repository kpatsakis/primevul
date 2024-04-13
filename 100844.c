v8::Handle<v8::Value> V8DOMWindow::postMessageMethodCustom(const v8::Arguments& args)
{
    DOMWindow* window = V8DOMWindow::toNative(args.Holder());
    DOMWindow* source = activeDOMWindow();

    if (!source)
        return throwTypeError(0, args.GetIsolate());

    MessagePortArray portArray;
    ArrayBufferArray arrayBufferArray;
    int targetOriginArgIndex = 1;
    if (args.Length() > 2) {
        int transferablesArgIndex = 2;
        if (isLegacyTargetOriginDesignation(args[2])) {
            targetOriginArgIndex = 2;
            transferablesArgIndex = 1;
        }
        if (!extractTransferables(args[transferablesArgIndex], portArray, arrayBufferArray, args.GetIsolate()))
            return v8::Undefined();
    }
    V8TRYCATCH_FOR_V8STRINGRESOURCE(V8StringResource<WithUndefinedOrNullCheck>, targetOrigin, args[targetOriginArgIndex]);

    bool didThrow = false;
    RefPtr<SerializedScriptValue> message =
        SerializedScriptValue::create(args[0],
                                      &portArray,
                                      &arrayBufferArray,
                                      didThrow,
                                      args.GetIsolate());
    if (didThrow)
        return v8::Undefined();

    ExceptionCode ec = 0;
    window->postMessage(message.release(), &portArray, targetOrigin, source, ec);
    return setDOMException(ec, args.GetIsolate());
}
