 static Handle<FixedArray> DirectCollectElementIndicesImpl(
 Isolate* isolate, Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store, GetKeysConversion convert,
 PropertyFilter filter, Handle<FixedArray> list, uint32_t* nof_indices,
 uint32_t insertion_index = 0) {
 uint32_t length = Subclass::GetMaxIndex(*object, *backing_store);
 for (uint32_t i = 0; i < length; i++) {
 if (Subclass::HasElementImpl(isolate, object, i, backing_store, filter)) {
 if (convert == GetKeysConversion::kConvertToString) {
 Handle<String> index_string = isolate->factory()->Uint32ToString(i);
 list->set(insertion_index, *index_string);
 } else {
 list->set(insertion_index, Smi::FromInt(i), SKIP_WRITE_BARRIER);
 }
        insertion_index++;
 }
 }
 *nof_indices = insertion_index;
 return list;
 }
