void BindColor(Model* model, V* view, C color, S setter) {
  view->AddBinding(base::MakeUnique<Binding<SkColor>>(
      base::Bind([](Model* m, C c) { return (m->color_scheme()).*c; },
                 base::Unretained(model), color),
      base::Bind([](V* v, S s, const SkColor& value) { (v->*s)(value); },
                 base::Unretained(view), setter)));
}
