bool decode(ArgumentDecoder* decoder, RetainPtr<CFBooleanRef>& result)
{
    bool boolean;
    if (!decoder->decode(boolean))
        return false;

    result.adoptCF(boolean ? kCFBooleanTrue : kCFBooleanFalse);
    return true;
}
