bool decode(ArgumentDecoder* decoder, RetainPtr<SecCertificateRef>& result)
{
    RetainPtr<CFDataRef> data;
    if (!decode(decoder, data))
        return false;

    result.adoptCF(SecCertificateCreateWithData(0, data.get()));
    return true;
}
