int test_64()
{
    long long int a = 0;// 0x8000000000000000;
    a = (~a)<<63;
    long long int b = 0 - a;
    long long int c = 0x8000000000000000;
    long long int zero = 1;
    long long int d = zero - c;
    long long int e = 2 * a;
    printf("sizeof(a)=%zu sizeof(long int) = %zu\n",sizeof(a),sizeof(long int));
    printf("a=%lld b=%lld\n", a, b);
    printf("c=%lld d=%lld\n", c, d);
    printf("e=%lld\n", e);
    if (b < 0) {
        printf("b < 0\n");
    }
    else if (b == 0) {
        printf("b = 0\n");
    }
    else {
        printf("b > 0\n");
    }
    return 0;
}
