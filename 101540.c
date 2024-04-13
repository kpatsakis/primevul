void TearDownV8() {
  g_isolate_holder->isolate()->Exit();
  delete g_isolate_holder;
  g_isolate_holder = nullptr;
}
