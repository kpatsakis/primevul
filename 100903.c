bool ScriptLoader::isScriptForEventSupported() const
{
    String eventAttribute = client()->eventAttributeValue();
    String forAttribute = client()->forAttributeValue();
    if (!eventAttribute.isEmpty() && !forAttribute.isEmpty()) {
        forAttribute = forAttribute.stripWhiteSpace();
        if (!equalIgnoringCase(forAttribute, "window"))
            return false;

        eventAttribute = eventAttribute.stripWhiteSpace();
        if (!equalIgnoringCase(eventAttribute, "onload") && !equalIgnoringCase(eventAttribute, "onload()"))
            return false;
    }
    return true;
}
