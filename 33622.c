int OBJ_new_nid(int num)
	{
	int i;

	i=new_nid;
	new_nid+=num;
	return(i);
	}
