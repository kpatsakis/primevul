bool decode(ArgumentDecoder* decoder, RetainPtr<CFStringRef>& result)
{
    CFStringEncoding encoding;
    if (!decoder->decodeEnum(encoding))
        return false;

    if (!CFStringIsEncodingAvailable(encoding))
        return false;
    
    CoreIPC::DataReference dataReference;
    if (!decoder->decode(dataReference))
        return false;

    CFStringRef string = CFStringCreateWithBytes(0, dataReference.data(), dataReference.size(), encoding, false);
    if (!string)
        return false;

    result.adoptCF(string);
    return true;
}
