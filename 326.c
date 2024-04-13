bool decode(ArgumentDecoder* decoder, RetainPtr<CFTypeRef>& result)
{
    CFType type;
    if (!decoder->decodeEnum(type))
        return false;

    switch (type) {
    case CFArray: {
        RetainPtr<CFArrayRef> array;
        if (!decode(decoder, array))
            return false;
        result.adoptCF(array.leakRef());
        return true;
    }
    case CFBoolean: {
        RetainPtr<CFBooleanRef> boolean;
        if (!decode(decoder, boolean))
            return false;
        result.adoptCF(boolean.leakRef());
        return true;
    }
    case CFData: {
        RetainPtr<CFDataRef> data;
        if (!decode(decoder, data))
            return false;
        result.adoptCF(data.leakRef());
        return true;
    }
    case CFDate: {
        RetainPtr<CFDateRef> date;
        if (!decode(decoder, date))
            return false;
        result.adoptCF(date.leakRef());
        return true;
    }
    case CFDictionary: {
        RetainPtr<CFDictionaryRef> dictionary;
        if (!decode(decoder, dictionary))
            return false;
        result.adoptCF(dictionary.leakRef());
        return true;
    }
    case CFNull:
        result.adoptCF(kCFNull);
        return true;
    case CFNumber: {
        RetainPtr<CFNumberRef> number;
        if (!decode(decoder, number))
            return false;
        result.adoptCF(number.leakRef());
        return true;
    }
    case CFString: {
        RetainPtr<CFStringRef> string;
        if (!decode(decoder, string))
            return false;
        result.adoptCF(string.leakRef());
        return true;
    }
    case CFURL: {
        RetainPtr<CFURLRef> url;
        if (!decode(decoder, url))
            return false;
        result.adoptCF(url.leakRef());
        return true;
    }
#if PLATFORM(MAC)
    case SecCertificate: {
        RetainPtr<SecCertificateRef> certificate;
        if (!decode(decoder, certificate))
            return false;
        result.adoptCF(certificate.leakRef());
        return true;
    }
    case SecKeychainItem: {
        RetainPtr<SecKeychainItemRef> keychainItem;
        if (!decode(decoder, keychainItem))
            return false;
        result.adoptCF(keychainItem.leakRef());
        return true;
    }
#endif
    case Null:
        result = tokenNullTypeRef();
        return true;
    case Unknown:
        ASSERT_NOT_REACHED();
        return false;
    }

    return false;
}
