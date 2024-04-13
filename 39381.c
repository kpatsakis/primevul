static int br_mdb_add_group(struct net_bridge *br, struct net_bridge_port *port,
			    struct br_ip *group, unsigned char state)
{
	struct net_bridge_mdb_entry *mp;
	struct net_bridge_port_group *p;
	struct net_bridge_port_group __rcu **pp;
	struct net_bridge_mdb_htable *mdb;
	int err;

	mdb = mlock_dereference(br->mdb, br);
	mp = br_mdb_ip_get(mdb, group);
	if (!mp) {
		mp = br_multicast_new_group(br, port, group);
		err = PTR_ERR(mp);
		if (IS_ERR(mp))
			return err;
	}

	for (pp = &mp->ports;
	     (p = mlock_dereference(*pp, br)) != NULL;
	     pp = &p->next) {
		if (p->port == port)
			return -EEXIST;
		if ((unsigned long)p->port < (unsigned long)port)
			break;
	}

	p = br_multicast_new_port_group(port, group, *pp, state);
	if (unlikely(!p))
		return -ENOMEM;
	rcu_assign_pointer(*pp, p);

	br_mdb_notify(br->dev, port, group, RTM_NEWMDB);
	return 0;
}
