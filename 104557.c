XFreeModifiermap(XModifierKeymap *map)
{
    if (map) {
        Xfree(map->modifiermap);
	Xfree(map);
    }
    return 1;
}
