void Element::addActiveAnimation(Animation* animation)
{
    ElementRareData* rareData = ensureElementRareData();
    if (!rareData->activeAnimations())
        rareData->setActiveAnimations(adoptPtr(new Vector<Animation*>));
    rareData->activeAnimations()->append(animation);
}
