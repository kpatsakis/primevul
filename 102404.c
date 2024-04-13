 static bool HasAccessorsImpl(JSObject* holder,
 FixedArrayBase* backing_store) {
 return DictionaryElementsAccessor::HasAccessorsImpl(holder, backing_store);
 }
