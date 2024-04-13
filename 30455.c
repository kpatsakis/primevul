static const EVP_MD *rsa_mgf1_to_md(X509_ALGOR *alg, X509_ALGOR *maskHash)
{
    const EVP_MD *md;
    if (!alg)
        return EVP_sha1();
    /* Check mask and lookup mask hash algorithm */
    if (OBJ_obj2nid(alg->algorithm) != NID_mgf1) {
        RSAerr(RSA_F_RSA_MGF1_TO_MD, RSA_R_UNSUPPORTED_MASK_ALGORITHM);
        return NULL;
    }
    if (!maskHash) {
        RSAerr(RSA_F_RSA_MGF1_TO_MD, RSA_R_UNSUPPORTED_MASK_PARAMETER);
        return NULL;
    }
    md = EVP_get_digestbyobj(maskHash->algorithm);
    if (md == NULL) {
        RSAerr(RSA_F_RSA_MGF1_TO_MD, RSA_R_UNKNOWN_MASK_DIGEST);
        return NULL;
    }
    return md;
}
