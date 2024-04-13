void Document::processReferrerPolicy(const String& policy)
{
    ReferrerPolicy referrerPolicy;
    if (!SecurityPolicy::referrerPolicyFromString(policy, &referrerPolicy)) {
        addConsoleMessage(ConsoleMessage::create(RenderingMessageSource, ErrorMessageLevel, "Failed to set referrer policy: The value '" + policy + "' is not one of 'always', 'default', 'never', 'no-referrer', 'no-referrer-when-downgrade', 'origin', 'origin-when-crossorigin', or 'unsafe-url'. This document's referrer policy has been left unchanged."));
        return;
    }

    setReferrerPolicy(referrerPolicy);
}
