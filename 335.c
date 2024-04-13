void encode(ArgumentEncoder* encoder, CFTypeRef typeRef)
{
    CFType type = typeFromCFTypeRef(typeRef);
    encoder->encodeEnum(type);

    switch (type) {
    case CFArray:
        encode(encoder, static_cast<CFArrayRef>(typeRef));
        return;
    case CFBoolean:
        encode(encoder, static_cast<CFBooleanRef>(typeRef));
        return;
    case CFData:
        encode(encoder, static_cast<CFDataRef>(typeRef));
        return;
    case CFDate:
        encode(encoder, static_cast<CFDateRef>(typeRef));
        return;
    case CFDictionary:
        encode(encoder, static_cast<CFDictionaryRef>(typeRef));
        return;
    case CFNull:
        return;
    case CFNumber:
        encode(encoder, static_cast<CFNumberRef>(typeRef));
        return;
    case CFString:
        encode(encoder, static_cast<CFStringRef>(typeRef));
        return;
    case CFURL:
        encode(encoder, static_cast<CFURLRef>(typeRef));
        return;
#if PLATFORM(MAC)
    case SecCertificate:
        encode(encoder, (SecCertificateRef)typeRef);
        return;
    case SecKeychainItem:
        encode(encoder, (SecKeychainItemRef)typeRef);
        return;
#endif
    case Null:
        return;
    case Unknown:
        break;
    }

    ASSERT_NOT_REACHED();
}
