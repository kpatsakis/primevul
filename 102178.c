static void ConvertDoubleToSRational(double value, int *numerator,
 int *denominator) {
 int negative = 0;

 if (value < 0) {
        value = -value;
        negative = 1;
 }

 unsigned int n, d;
    float2urat(value, 0x7FFFFFFFU, &n, &d);
 *numerator = (int)n;
 *denominator = (int)d;
 if (negative) {
 *numerator = -*numerator;
 }
}
