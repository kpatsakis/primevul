bool ParamTraits<std::vector<char> >::Read(const Message* m,
                                           PickleIterator* iter,
                                           param_type* r) {
  const char *data;
  int data_size = 0;
  if (!m->ReadData(iter, &data, &data_size) || data_size < 0)
    return false;
  r->resize(data_size);
  if (data_size)
    memcpy(&r->front(), data, data_size);
  return true;
}
