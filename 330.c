bool decode(ArgumentDecoder* decoder, RetainPtr<CFDateRef>& result)
{
    double absoluteTime;
    if (!decoder->decodeDouble(absoluteTime))
        return false;

    result.adoptCF(CFDateCreate(0, absoluteTime));
    return true;
}
