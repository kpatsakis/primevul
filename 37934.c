static bool ima_match_rules(struct ima_measure_rule_entry *rule,
			    struct inode *inode, enum ima_hooks func, int mask)
{
	struct task_struct *tsk = current;
	int i;

	if ((rule->flags & IMA_FUNC) && rule->func != func)
		return false;
	if ((rule->flags & IMA_MASK) && rule->mask != mask)
		return false;
	if ((rule->flags & IMA_FSMAGIC)
	    && rule->fsmagic != inode->i_sb->s_magic)
		return false;
	if ((rule->flags & IMA_UID) && rule->uid != tsk->cred->uid)
		return false;
	for (i = 0; i < MAX_LSM_RULES; i++) {
		int rc = 0;
		u32 osid, sid;

		if (!rule->lsm[i].rule)
			continue;

		switch (i) {
		case LSM_OBJ_USER:
		case LSM_OBJ_ROLE:
		case LSM_OBJ_TYPE:
			security_inode_getsecid(inode, &osid);
			rc = security_filter_rule_match(osid,
							rule->lsm[i].type,
							Audit_equal,
							rule->lsm[i].rule,
							NULL);
			break;
		case LSM_SUBJ_USER:
		case LSM_SUBJ_ROLE:
		case LSM_SUBJ_TYPE:
			security_task_getsecid(tsk, &sid);
			rc = security_filter_rule_match(sid,
							rule->lsm[i].type,
							Audit_equal,
							rule->lsm[i].rule,
							NULL);
		default:
			break;
		}
		if (!rc)
			return false;
	}
	return true;
}
