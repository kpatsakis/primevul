int Get16u(void * Short)
{
 if (MotorolaOrder){
 return (((uchar *)Short)[0] << 8) | ((uchar *)Short)[1];
 }else{
 return (((uchar *)Short)[1] << 8) | ((uchar *)Short)[0];
 }
}
