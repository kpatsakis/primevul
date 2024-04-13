void encode(ArgumentEncoder* encoder, SecKeychainItemRef keychainItem)
{
    CFDataRef data;
    if (SecKeychainItemCreatePersistentReference(keychainItem, &data) == errSecSuccess) {
        encode(encoder, data);
        CFRelease(data);
    }
}
