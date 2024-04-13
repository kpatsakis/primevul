static inline bool xchg_paths(struct rtrs_clt_path __rcu **rcu_ppcpu_path,
			      struct rtrs_clt_path *clt_path,
			      struct rtrs_clt_path *next)
{
	struct rtrs_clt_path **ppcpu_path;

	/* Call cmpxchg() without sparse warnings */
	ppcpu_path = (typeof(ppcpu_path))rcu_ppcpu_path;
	return clt_path == cmpxchg(ppcpu_path, clt_path, next);
}