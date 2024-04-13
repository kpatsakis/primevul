static float uif(unsigned int ui)
{
   union { float f; unsigned int ui; } myuif;
   myuif.ui = ui;
   return myuif.f;
}
