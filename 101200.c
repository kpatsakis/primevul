bool CSPSourceList::parseSource(const UChar* begin, const UChar* end, String& scheme, String& host, int& port, String& path, CSPSource::WildcardDisposition& hostWildcard, CSPSource::WildcardDisposition& portWildcard)
{
    if (begin == end)
        return false;

    if (equalIgnoringCase("'none'", begin, end - begin))
        return false;

    if (end - begin == 1 && *begin == '*') {
        addSourceStar();
        return true;
    }

    if (equalIgnoringCase("'self'", begin, end - begin)) {
        addSourceSelf();
        return true;
    }

    if (equalIgnoringCase("'unsafe-inline'", begin, end - begin)) {
        addSourceUnsafeInline();
        return true;
    }

    if (equalIgnoringCase("'unsafe-eval'", begin, end - begin)) {
        addSourceUnsafeEval();
        return true;
    }

    String nonce;
    if (!parseNonce(begin, end, nonce))
        return false;

    if (!nonce.isNull()) {
        addSourceNonce(nonce);
        return true;
    }

    DigestValue hash;
    ContentSecurityPolicyHashAlgorithm algorithm = ContentSecurityPolicyHashAlgorithmNone;
    if (!parseHash(begin, end, hash, algorithm))
        return false;

    if (hash.size() > 0) {
        addSourceHash(algorithm, hash);
        return true;
    }

    const UChar* position = begin;
    const UChar* beginHost = begin;
    const UChar* beginPath = end;
    const UChar* beginPort = 0;

    skipWhile<UChar, isNotColonOrSlash>(position, end);

    if (position == end) {
        return parseHost(beginHost, position, host, hostWildcard);
    }

    if (position < end && *position == '/') {
        return parseHost(beginHost, position, host, hostWildcard) && parsePath(position, end, path);
    }

    if (position < end && *position == ':') {
        if (end - position == 1) {
            return parseScheme(begin, position, scheme);
        }

        if (position[1] == '/') {
            if (!parseScheme(begin, position, scheme)
                || !skipExactly<UChar>(position, end, ':')
                || !skipExactly<UChar>(position, end, '/')
                || !skipExactly<UChar>(position, end, '/'))
                return false;
            if (position == end)
                return false;
            beginHost = position;
            skipWhile<UChar, isNotColonOrSlash>(position, end);
        }

        if (position < end && *position == ':') {
            beginPort = position;
            skipUntil<UChar>(position, end, '/');
        }
    }

    if (position < end && *position == '/') {
        if (position == beginHost)
            return false;
        beginPath = position;
    }

    if (!parseHost(beginHost, beginPort ? beginPort : beginPath, host, hostWildcard))
        return false;

    if (beginPort) {
        if (!parsePort(beginPort, beginPath, port, portWildcard))
            return false;
    } else {
        port = 0;
    }

    if (beginPath != end) {
        if (!parsePath(beginPath, end, path))
            return false;
    }

    return true;
}
