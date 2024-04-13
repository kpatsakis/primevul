static const char *find_word_break(const char *string)
{
    /* Skip over the actual word */
    while (string && *string && !isspace(*string))
        string++;

    return string;
}
