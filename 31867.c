generate_trivial (DBusMessageDataIter   *iter,
                  DBusString            *data,
                  DBusValidity          *expected_validity)
{
  return generate_outer (iter, data, expected_validity,
                         generate_trivial_inner);
}
