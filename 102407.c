 static bool IsPackedImpl(Handle<JSObject> holder,
 Handle<FixedArrayBase> backing_store, uint32_t start,
 uint32_t end) {
 if (IsFastPackedElementsKind(kind())) return true;
 Isolate* isolate = backing_store->GetIsolate();
 for (uint32_t i = start; i < end; i++) {
 if (!Subclass::HasElementImpl(isolate, holder, i, backing_store,
                                    ALL_PROPERTIES)) {
 return false;
 }
 }
 return true;
 }
