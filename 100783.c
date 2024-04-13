void Element::removeActiveAnimation(Animation* animation)
{
    ElementRareData* rareData = elementRareData();
    ASSERT(rareData);
    size_t position = rareData->activeAnimations()->find(animation);
    ASSERT(position != notFound);
    rareData->activeAnimations()->remove(position);
}
