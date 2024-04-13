  static void CleanUp() {
    SharedMemory memory;
    memory.Delete(s_test_name_);
  }
