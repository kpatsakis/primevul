bool ParamTraits<base::TimeDelta>::Read(const Message* m,
                                        PickleIterator* iter,
                                        param_type* r) {
  int64 value;
  bool ret = ParamTraits<int64>::Read(m, iter, &value);
  if (ret)
    *r = base::TimeDelta::FromInternalValue(value);

  return ret;
}
