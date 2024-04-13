v8::Handle<v8::Value> V8ThrowException::createTypeError(v8::Isolate* isolate, const String& message)
{
    return v8::Exception::TypeError(v8String(isolate, message.isNull() ? "Type error" : message));
}
