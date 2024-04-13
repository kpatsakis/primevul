static CFType typeFromCFTypeRef(CFTypeRef type)
{
    ASSERT(type);

    if (type == tokenNullTypeRef())
        return Null;

    CFTypeID typeID = CFGetTypeID(type);
    if (typeID == CFArrayGetTypeID())
        return CFArray;
    if (typeID == CFBooleanGetTypeID())
        return CFBoolean;
    if (typeID == CFDataGetTypeID())
        return CFData;
    if (typeID == CFDateGetTypeID())
        return CFDate;
    if (typeID == CFDictionaryGetTypeID())
        return CFDictionary;
    if (typeID == CFNullGetTypeID())
        return CFNull;
    if (typeID == CFNumberGetTypeID())
        return CFNumber;
    if (typeID == CFStringGetTypeID())
        return CFString;
    if (typeID == CFURLGetTypeID())
        return CFURL;
#if PLATFORM(MAC)
    if (typeID == SecCertificateGetTypeID())
        return SecCertificate;
    if (typeID == SecKeychainItemGetTypeID())
        return SecKeychainItem;
#endif

    ASSERT_NOT_REACHED();
    return Unknown;
}
