 static PropertyDetails GetDetailsImpl(JSObject* holder, uint32_t entry) {
 uint32_t length = static_cast<uint32_t>(GetString(holder)->length());
 if (entry < length) {
 PropertyAttributes attributes =
 static_cast<PropertyAttributes>(READ_ONLY | DONT_DELETE);
 return PropertyDetails(kData, attributes, 0, PropertyCellType::kNoCell);
 }
 return BackingStoreAccessor::GetDetailsImpl(holder, entry - length);
 }
