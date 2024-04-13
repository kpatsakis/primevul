int EVP_PKEY_id(const EVP_PKEY *pkey)
{
	return pkey->type;
}
