static ALWAYS_INLINE struct module_entry *get_modentry(const char *module)
{
	return helper_get_module(module, 0);
}
