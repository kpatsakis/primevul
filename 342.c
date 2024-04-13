void encode(ArgumentEncoder* encoder, CFURLRef url)
{
    CFURLRef baseURL = CFURLGetBaseURL(url);
    encoder->encodeBool(baseURL);
    if (baseURL)
        encode(encoder, baseURL);

    encode(encoder, CFURLGetString(url));
}
