void encode(ArgumentEncoder* encoder, CFDictionaryRef dictionary)
{
    CFIndex size = CFDictionaryGetCount(dictionary);
    Vector<CFTypeRef, 32> keys(size);
    Vector<CFTypeRef, 32> values(size);
    
    CFDictionaryGetKeysAndValues(dictionary, keys.data(), values.data());

    encoder->encodeUInt64(size);

    for (CFIndex i = 0; i < size; ++i) {
        ASSERT(keys[i]);
        ASSERT(CFGetTypeID(keys[i]) == CFStringGetTypeID());
        ASSERT(values[i]);

        if (typeFromCFTypeRef(values[i]) == Unknown)
            continue;

        encode(encoder, static_cast<CFStringRef>(keys[i]));
        encode(encoder, values[i]);
    }
}
