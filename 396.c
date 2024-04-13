v8::Handle<v8::Value> V8ThrowException::createRangeError(v8::Isolate* isolate, const String& message)
{
    return v8::Exception::RangeError(v8String(isolate, message.isNull() ? "Range error" : message));
}
