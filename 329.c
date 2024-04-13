bool decode(ArgumentDecoder* decoder, RetainPtr<CFDataRef>& result)
{
    CoreIPC::DataReference dataReference;
    if (!decoder->decode(dataReference))
        return false;

    result.adoptCF(CFDataCreate(0, dataReference.data(), dataReference.size()));
    return true;
}
