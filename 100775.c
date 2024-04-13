bool Element::hasActiveAnimations() const
{
    return hasRareData() && elementRareData()->activeAnimations()
        && elementRareData()->activeAnimations()->size();
}
