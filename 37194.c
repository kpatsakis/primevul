static void __init init_alloc_cpu(void)
{
	int cpu;

	for_each_online_cpu(cpu)
		init_alloc_cpu_cpu(cpu);
  }
