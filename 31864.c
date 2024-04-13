generate_many_bodies (DBusMessageDataIter   *iter,
                      DBusString            *data,
                      DBusValidity          *expected_validity)
{
  return generate_outer (iter, data, expected_validity,
                         generate_many_bodies_inner);
}
