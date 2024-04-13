isdst_differ (int a, int b)
{
  return !a != !b && 0 <= a && 0 <= b;
}
