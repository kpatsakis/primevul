v8::Handle<v8::Value> V8ThrowException::createSyntaxError(v8::Isolate* isolate, const String& message)
{
    return v8::Exception::SyntaxError(v8String(isolate, message.isNull() ? "Syntax error" : message));
}
