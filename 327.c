bool decode(ArgumentDecoder* decoder, RetainPtr<CFArrayRef>& result)
{
    uint64_t size;
    if (!decoder->decodeUInt64(size))
        return false;

    RetainPtr<CFMutableArrayRef> array(AdoptCF, CFArrayCreateMutable(0, 0, &kCFTypeArrayCallBacks));

    for (size_t i = 0; i < size; ++i) {
        RetainPtr<CFTypeRef> element;
        if (!decode(decoder, element))
            return false;

        CFArrayAppendValue(array.get(), element.get());
    }

    result.adoptCF(array.leakRef());
    return true;
}
