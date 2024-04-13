bool ParamTraits<gfx::SizeF>::Read(const Message* m,
                                   PickleIterator* iter,
                                   gfx::SizeF* p) {
  float w, h;
  if (!ParamTraits<float>::Read(m, iter, &w) ||
      !ParamTraits<float>::Read(m, iter, &h))
    return false;
  p->set_width(w);
  p->set_height(h);
  return true;
}
