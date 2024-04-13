FontFileNameCompare(const void* a, const void* b)
{
    FontEntryPtr    a_name = (FontEntryPtr) a,
		    b_name = (FontEntryPtr) b;

    return strcmpn(a_name->name.name, b_name->name.name);
}
