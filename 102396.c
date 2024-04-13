MaybeHandle<Object> ArrayConstructInitializeElements(Handle<JSArray> array,
 Arguments* args) {
 if (args->length() == 0) {
 JSArray::Initialize(array, JSArray::kPreallocatedArrayElements);
 return array;

 } else if (args->length() == 1 && args->at(0)->IsNumber()) {
 uint32_t length;
 if (!args->at(0)->ToArrayLength(&length)) {
 return ThrowArrayLengthRangeError(array->GetIsolate());
 }

 if (length > 0 && length < JSArray::kInitialMaxFastElementArray) {
 ElementsKind elements_kind = array->GetElementsKind();
 JSArray::Initialize(array, length, length);

 if (!IsFastHoleyElementsKind(elements_kind)) {
        elements_kind = GetHoleyElementsKind(elements_kind);
 JSObject::TransitionElementsKind(array, elements_kind);
 }
 } else if (length == 0) {
 JSArray::Initialize(array, JSArray::kPreallocatedArrayElements);
 } else {
 JSArray::Initialize(array, 0);
 JSArray::SetLength(array, length);
 }
 return array;
 }

 Factory* factory = array->GetIsolate()->factory();

 int number_of_elements = args->length();
 JSObject::EnsureCanContainElements(
      array, args, 0, number_of_elements, ALLOW_CONVERTED_DOUBLE_ELEMENTS);

 ElementsKind elements_kind = array->GetElementsKind();
 Handle<FixedArrayBase> elms;
 if (IsFastDoubleElementsKind(elements_kind)) {
    elms = Handle<FixedArrayBase>::cast(
        factory->NewFixedDoubleArray(number_of_elements));
 } else {
    elms = Handle<FixedArrayBase>::cast(
        factory->NewFixedArrayWithHoles(number_of_elements));
 }

 switch (elements_kind) {
 case FAST_HOLEY_SMI_ELEMENTS:
 case FAST_SMI_ELEMENTS: {
 Handle<FixedArray> smi_elms = Handle<FixedArray>::cast(elms);
 for (int entry = 0; entry < number_of_elements; entry++) {
        smi_elms->set(entry, (*args)[entry], SKIP_WRITE_BARRIER);
 }
 break;
 }
 case FAST_HOLEY_ELEMENTS:
 case FAST_ELEMENTS: {
 DisallowHeapAllocation no_gc;
 WriteBarrierMode mode = elms->GetWriteBarrierMode(no_gc);
 Handle<FixedArray> object_elms = Handle<FixedArray>::cast(elms);
 for (int entry = 0; entry < number_of_elements; entry++) {
        object_elms->set(entry, (*args)[entry], mode);
 }
 break;
 }
 case FAST_HOLEY_DOUBLE_ELEMENTS:
 case FAST_DOUBLE_ELEMENTS: {
 Handle<FixedDoubleArray> double_elms =
 Handle<FixedDoubleArray>::cast(elms);
 for (int entry = 0; entry < number_of_elements; entry++) {
        double_elms->set(entry, (*args)[entry]->Number());
 }
 break;
 }
 default:
      UNREACHABLE();
 break;
 }

  array->set_elements(*elms);
  array->set_length(Smi::FromInt(number_of_elements));
 return array;
}
