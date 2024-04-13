bool decode(ArgumentDecoder* decoder, RetainPtr<SecKeychainItemRef>& result)
{
    RetainPtr<CFDataRef> data;
    if (!CoreIPC::decode(decoder, data))
        return false;

    SecKeychainItemRef item;
    if (SecKeychainItemCopyFromPersistentReference(data.get(), &item) != errSecSuccess || !item)
        return false;
    
    result.adoptCF(item);
    return true;
}
