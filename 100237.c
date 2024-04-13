static String& pageCacheLogPrefix(int indentLevel)
{
    static int previousIndent = -1;
    DEFINE_STATIC_LOCAL(String, prefix, ());
    
    if (indentLevel != previousIndent) {    
        previousIndent = indentLevel;
        prefix.truncate(0);
        for (int i = 0; i < previousIndent; ++i)
            prefix += "    ";
    }
    
    return prefix;
}
