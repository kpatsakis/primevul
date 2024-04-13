ScriptLoader* toScriptLoaderIfPossible(Element* element)
{
    if (isHTMLScriptLoader(element))
        return toHTMLScriptElement(element)->loader();

    if (isSVGScriptLoader(element))
        return toSVGScriptElement(element)->loader();

    return 0;
}
