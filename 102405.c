 static bool HasElementImpl(Isolate* isolate, Handle<JSObject> holder,
 uint32_t index,
 Handle<FixedArrayBase> backing_store,
 PropertyFilter filter = ALL_PROPERTIES) {
 return Subclass::GetEntryForIndexImpl(isolate, *holder, *backing_store,
                                          index, filter) != kMaxUInt32;
 }
