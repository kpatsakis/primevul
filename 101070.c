HTMLBodyElement::HTMLBodyElement(Document& document)
    : HTMLElement(bodyTag, document)
{
    ScriptWrappable::init(this);
}
