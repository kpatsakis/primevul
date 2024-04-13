void find_var_recursive(const TypedValue* tv,
                        const req::ptr<WddxPacket>& wddxPacket) {
  if (tvIsString(tv)) {
    String var_name{tvCastToString(tv)};
    wddxPacket->add_var(var_name, true);
  }
  if (isArrayType(tv->m_type)) {
    for (ArrayIter iter(tv->m_data.parr); iter; ++iter) {
      find_var_recursive(iter.secondRef().asTypedValue(), wddxPacket);
    }
  }
}