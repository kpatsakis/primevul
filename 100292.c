bool V8DOMWrapper::domObjectHasJSWrapper(void* object)
{
    return getDOMObjectMap().contains(object) || getActiveDOMObjectMap().contains(object);
}
