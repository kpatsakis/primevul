static const char *lookup_name(uint32_t index, const char **list, uint32_t llen)
{
    if (index >= llen || list[index] == NULL) {
        return "???";
    }
    return list[index];
}
