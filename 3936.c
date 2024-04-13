static TypedValue* add_vars_helper(ActRec* ar) {
  int start_index = 1;
  Resource packet_id{getArg<KindOfResource>(ar, 0)};
  auto wddxPacket = cast<WddxPacket>(packet_id);

  for (int i = start_index; i < ar->numArgs(); i++) {
    auto const tv = getArg(ar, i);
    find_var_recursive(tv, wddxPacket);
  }
  return arReturn(ar, true);
}