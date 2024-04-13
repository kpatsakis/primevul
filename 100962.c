void ParamTraits<int>::Log(const param_type& p, std::string* l) {
  l->append(base::IntToString(p));
}
