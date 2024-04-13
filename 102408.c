 static uint32_t NumberOfElementsImpl(JSObject* receiver,
 FixedArrayBase* backing_store) {
 return AccessorClass::GetCapacityImpl(receiver, backing_store);
 }
