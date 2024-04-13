 explicit FastPackedSmiElementsAccessor(const char* name)
 : FastSmiOrObjectElementsAccessor<
 FastPackedSmiElementsAccessor,
 ElementsKindTraits<FAST_SMI_ELEMENTS> >(name) {}
