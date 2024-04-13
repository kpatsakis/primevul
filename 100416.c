static int compareNumbersForQSort(const void* a, const void* b)
{
    double da = static_cast<const JSValue*>(a)->uncheckedGetNumber();
    double db = static_cast<const JSValue*>(b)->uncheckedGetNumber();
    return (da > db) - (da < db);
}
