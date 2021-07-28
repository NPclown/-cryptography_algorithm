#include <stdio.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM * a)
{
        /* Use BN_bn2hex(a) for hex string * Use BN_bn2dec(a) for decimal string */
        char * number_str = BN_bn2dec(a);
        printf("%s %s\n", msg, number_str);
        OPENSSL_free(number_str);
}

BIGNUM *XEuclid(BIGNUM *x, BIGNUM *y, const BIGNUM *a, const BIGNUM *b)
{
    BIGNUM * x0 = BN_new();
    BIGNUM * x1 = BN_new();
    BIGNUM * y0 = BN_new();
    BIGNUM * y1 = BN_new();
    
    BIGNUM * dv = BN_new();     //몫
    BIGNUM * rem = BN_new();    //나머지
    BIGNUM * m = BN_new();      // y1 * 몫 곱셈 저장소

    BN_CTX *ctx = BN_CTX_new();

    BIGNUM * aa = BN_new();
    BIGNUM * bb = BN_new();

    if (BN_cmp(a,b) < 0){
        BN_copy(aa,b);
        BN_copy(bb,a);
        BN_dec2bn(&x0, "0");
        BN_dec2bn(&y0, "1");
        BN_dec2bn(&x1, "1");
        BN_dec2bn(&y1, "0");
    }else{
        BN_copy(aa,a);
        BN_copy(bb,b);
        BN_dec2bn(&x0, "1");
        BN_dec2bn(&y0, "0");
        BN_dec2bn(&x1, "0");
        BN_dec2bn(&y1, "1");
    }

    while(!BN_is_zero(bb)){
        BN_div(dv, rem, aa, bb, ctx);  

        if (BN_is_zero(rem))
            break;

        //y계산
        BN_mul(m, y1, dv, ctx);
        BN_sub(y, y0, m);

        //x계산
        BN_mul(m, x1, dv, ctx);
        BN_sub(x, x0, m);

        printf("%s = %s*(%s) + %s\n",BN_bn2dec(aa),BN_bn2dec(bb),BN_bn2dec(dv),BN_bn2dec(rem));
        printf("%s*(%s) + %s*(%s) = %s\n",BN_bn2dec(a),BN_bn2dec(x),BN_bn2dec(b),BN_bn2dec(y),BN_bn2dec(rem));
        printf("----");

        BN_copy(y0, y1);
        BN_copy(y1, y);
        BN_copy(x0, x1);
        BN_copy(x1, x);

        BN_copy(aa, bb);
        BN_copy(bb, rem);

    }


    if(x0 != NULL) BN_free(x0);
    if(x1 != NULL) BN_free(x1);
    if(y0 != NULL) BN_free(y0);
    if(y1 != NULL) BN_free(y1);
    if(dv != NULL) BN_free(dv);
    if(rem != NULL) BN_free(rem);
    if(m != NULL) BN_free(m);
    if(aa != NULL) BN_free(aa);
    if(ctx != NULL) BN_CTX_free(ctx);

    return bb;
err:
    return NULL;
}

int main (int argc, char *argv[])
{
        BIGNUM *a = BN_new();
        BIGNUM *b = BN_new();
        BIGNUM *x = BN_new();
        BIGNUM *y = BN_new();
        BIGNUM *gcd;

        if(argc != 3){
                printf("usage: xeuclid num1 num2");
                return -1;
        }
        BN_dec2bn(&a, argv[1]);
        BN_dec2bn(&b, argv[2]);
        gcd = XEuclid(x,y,a,b);

        printBN("(a,b) = ", gcd);
        printBN("a = ", a);
        printBN("b = ", b);
        printBN("x = ", x);
        printBN("y = ", y);
        printf("%s*(%s) + %s*(%s) = %s\n",BN_bn2dec(a),BN_bn2dec(x),BN_bn2dec(b),BN_bn2dec(y),BN_bn2dec(gcd));

        if(a != NULL) BN_free(a);
        if(b != NULL) BN_free(b);
        if(x != NULL) BN_free(x);
        if(y != NULL) BN_free(y);
        if(gcd != NULL) BN_free(gcd);

        return 0;
}
