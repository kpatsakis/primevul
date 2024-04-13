static String valueToDateTimeString(double value, AtomicString type)
{
    DateComponents components;
    if (type == InputTypeNames::date)
        components.setMillisecondsSinceEpochForDate(value);
    else if (type == InputTypeNames::datetime_local)
        components.setMillisecondsSinceEpochForDateTimeLocal(value);
    else if (type == InputTypeNames::month)
        components.setMonthsSinceEpoch(value);
    else if (type == InputTypeNames::time)
        components.setMillisecondsSinceMidnight(value);
    else if (type == InputTypeNames::week)
        components.setMillisecondsSinceEpochForWeek(value);
    else
        ASSERT_NOT_REACHED();
    return components.type() == DateComponents::Invalid ? String() : components.toString();
}
