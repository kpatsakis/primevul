void encode(ArgumentEncoder* encoder, SecCertificateRef certificate)
{
    RetainPtr<CFDataRef> data(AdoptCF, SecCertificateCopyData(certificate));
    encode(encoder, data.get());
}
