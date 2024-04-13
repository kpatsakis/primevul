void ParamTraits<std::vector<unsigned char> >::Write(Message* m,
                                                     const param_type& p) {
  if (p.empty()) {
    m->WriteData(NULL, 0);
  } else {
    m->WriteData(reinterpret_cast<const char*>(&p.front()),
                 static_cast<int>(p.size()));
  }
}
