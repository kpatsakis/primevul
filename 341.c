void encode(ArgumentEncoder* encoder, CFNumberRef number)
{
    CFNumberType numberType = CFNumberGetType(number);

    Vector<uint8_t> buffer(CFNumberGetByteSize(number));
    bool result = CFNumberGetValue(number, numberType, buffer.data());
    ASSERT_UNUSED(result, result);

    encoder->encodeEnum(numberType);
    encoder->encodeVariableLengthByteArray(buffer);
}
