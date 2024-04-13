String base64Encode(const char* data, unsigned length, Base64EncodePolicy policy)
{
    Vector<char> result;
    base64Encode(data, length, result, policy);
    return String(result.data(), result.size());
}
