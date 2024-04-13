 static Handle<Object> GetImpl(Handle<JSObject> holder, uint32_t entry) {
 Isolate* isolate = holder->GetIsolate();
 Handle<String> string(GetString(*holder), isolate);
 uint32_t length = static_cast<uint32_t>(string->length());
 if (entry < length) {
 return isolate->factory()->LookupSingleCharacterStringFromCode(
 String::Flatten(string)->Get(entry));
 }
 return BackingStoreAccessor::GetImpl(isolate, holder->elements(),
                                         entry - length);
 }
