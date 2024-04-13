void __init sk_init(void)
{
	if (totalram_pages <= 4096) {
		sysctl_wmem_max = 32767;
		sysctl_rmem_max = 32767;
		sysctl_wmem_default = 32767;
		sysctl_rmem_default = 32767;
	} else if (totalram_pages >= 131072) {
		sysctl_wmem_max = 131071;
		sysctl_rmem_max = 131071;
	}
}
