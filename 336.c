void encode(ArgumentEncoder* encoder, CFArrayRef array)
{
    CFIndex size = CFArrayGetCount(array);
    Vector<CFTypeRef, 32> values(size);

    CFArrayGetValues(array, CFRangeMake(0, size), values.data());

    encoder->encodeUInt64(size);
    for (CFIndex i = 0; i < size; ++i) {
        ASSERT(values[i]);

        encode(encoder, values[i]);
    }
}
