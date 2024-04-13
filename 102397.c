 static void DeleteAtEnd(Handle<JSObject> obj,
 Handle<BackingStore> backing_store, uint32_t entry) {
 uint32_t length = static_cast<uint32_t>(backing_store->length());
 Isolate* isolate = obj->GetIsolate();
 for (; entry > 0; entry--) {
 if (!backing_store->is_the_hole(isolate, entry - 1)) break;
 }
 if (entry == 0) {
 FixedArray* empty = isolate->heap()->empty_fixed_array();
 if (obj->GetElementsKind() == FAST_SLOPPY_ARGUMENTS_ELEMENTS) {
 FixedArray::cast(obj->elements())->set(1, empty);
 } else {
        obj->set_elements(empty);
 }
 return;
 }

    isolate->heap()->RightTrimFixedArray(*backing_store, length - entry);
 }
