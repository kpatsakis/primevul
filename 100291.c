static v8::Persistent<v8::String> GetToStringName()
{
    DEFINE_STATIC_LOCAL(v8::Persistent<v8::String>, value, ());
    if (value.IsEmpty())
        value = v8::Persistent<v8::String>::New(v8::String::New("toString"));
    return value;
}
