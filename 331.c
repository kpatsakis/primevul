bool decode(ArgumentDecoder* decoder, RetainPtr<CFNumberRef>& result)
{
    CFNumberType numberType;
    if (!decoder->decodeEnum(numberType))
        return false;

    CoreIPC::DataReference dataReference;
    if (!decoder->decode(dataReference))
        return false;

    size_t neededBufferSize = sizeForNumberType(numberType);
    if (!neededBufferSize || dataReference.size() != neededBufferSize)
        return false;

    ASSERT(dataReference.data());
    CFNumberRef number = CFNumberCreate(0, numberType, dataReference.data());
    result.adoptCF(number);

    return true;
}
