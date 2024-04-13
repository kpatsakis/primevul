bool base64Decode(const UChar* data, unsigned length, Vector<char>& out, CharacterMatchFunctionPtr shouldIgnoreCharacter, Base64DecodePolicy policy)
{
    return base64DecodeInternal<UChar>(data, length, out, shouldIgnoreCharacter, policy);
}
