 static void AddImpl(Handle<JSObject> object, uint32_t index,
 Handle<Object> value, PropertyAttributes attributes,
 uint32_t new_capacity) {
    DCHECK_EQ(NONE, attributes);
 Handle<FixedArray> parameter_map(FixedArray::cast(object->elements()));
 Handle<FixedArrayBase> old_elements(
 FixedArrayBase::cast(parameter_map->get(1)));
 if (old_elements->IsSeededNumberDictionary() ||
 static_cast<uint32_t>(old_elements->length()) < new_capacity) {
 GrowCapacityAndConvertImpl(object, new_capacity);
 }
 FixedArray* arguments = FixedArray::cast(parameter_map->get(1));
 FastHoleyObjectElementsAccessor::SetImpl(arguments, index, *value);
 }
