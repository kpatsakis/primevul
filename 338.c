void encode(ArgumentEncoder* encoder, CFDataRef data)
{
    CFIndex length = CFDataGetLength(data);
    const UInt8* bytePtr = CFDataGetBytePtr(data);

    encoder->encodeVariableLengthByteArray(CoreIPC::DataReference(bytePtr, length));
}
