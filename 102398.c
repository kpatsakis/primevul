 static void DeleteImpl(Handle<JSObject> obj, uint32_t entry) {
 ElementsKind kind = KindTraits::Kind;
 if (IsFastPackedElementsKind(kind)) {
 JSObject::TransitionElementsKind(obj, GetHoleyElementsKind(kind));
 }
 if (IsFastSmiOrObjectElementsKind(KindTraits::Kind)) {
 JSObject::EnsureWritableFastElements(obj);
 }
 DeleteCommon(obj, entry, handle(obj->elements()));
 }
