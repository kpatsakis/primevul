static void domExceptionStackGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    ASSERT(info.Data()->IsObject());
    v8SetReturnValue(info, info.Data()->ToObject(info.GetIsolate())->Get(v8AtomicString(info.GetIsolate(), "stack")));
}
