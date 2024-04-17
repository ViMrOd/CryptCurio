#include "bn.h"

bn bn_init()
{
    bn b = (bn)malloc(sizeof(bn_struct));
    if (b == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    b->size = 1;
    b->cap = SIZE_BN;
    b->sgn = 0;
    b->digits = (uint8_t*)calloc(b->cap, 1);

    if (b->digits == NULL) {
        free(b);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    return b;
}

bn bn_init_set_ui(uint64_t n)
{
    bn b = bn_init();

    if (n >= 10) {
        --b->size;
        size_t i = 0;
        while (n) {
            b->digits[i++] = n % 10;
            n /= 10;
            ++b->size;
        }
    } else {
        b->digits[0] = n;
    }
    
    return b;
}

bn bn_init_set_si(int64_t n)
{
    bn b = bn_init();

    if (n < 0) {
        b->sgn = 1;
        n *= -1;
    }

    size_t i = 0;
    while (1) {
        if (n == 0)
            break;
        b->digits[i++] = n % 10;
        n /= 10;
        ++b->size;
    }

    return b;
}

bn bn_init_set_str(const char *str)
{
    bn b = bn_init();

    b->size = strlen(str);
    int str_stop = b->size - 1;
    int str_start = 0;
    if (str[0] == '-') {
        b->sgn = 1;
        str_start = 1;
        --b->size;
    }
    
    for (size_t i = 0; str_stop >= str_start; str_stop--, i++)
        b->digits[i] = str[str_stop] - '0';
    
    return b;
}

void bn_set(bn rop, const bn a)
{
    bn_reset(rop);

    rop->cap = a->cap;
    rop->size = a->size;
    rop->sgn = a->sgn;

    for (int i = 0; i < rop->cap; i++) {
        rop->digits[i] = a->digits[i];
    }
}

void bn_clear(bn b)
{
    free(b->digits);
    free(b);
}

int bn_cmp(const bn a, const bn b)
{
    if (a->sgn == 0 && b->sgn == 1) {
        // a positive, b is negative //
        return 1;
    } else if (a->sgn == 1 && b->sgn == 0) {
        // a negative, b is positive //
        return -1;
    } else if (a->sgn == 0 && b->sgn == 0) {
        // both positive //
        return bn_cmp_abs(a, b);
    } else if (a->sgn == 1 && b->sgn == 1) {
        // both negative //
        return -1 * bn_cmp_abs(a, b);
    } else {
        return EXIT_FAILURE;
    }
}

int bn_cmp_abs(const bn a, const bn b)
{
    int max_size = a->size > b->size ? a->size - 1 : b->size - 1;
    for (int i = max_size; i >= 0; i--) {
        if (a->digits[i] > b->digits[i])
            return 1;
        if (a->digits[i] < b->digits[i])
            return -1;
    }
    return 0;
}

int bn_cmp_ui(const bn a, uint64_t b)
{
    bn B = bn_init_set_ui(b);
    return bn_cmp(a, B);

    bn_clear(B);
}

void bn_reset(bn b)
{
    b->cap = SIZE_BN;
    b->size = 1;
    b->sgn = 0;

    for (size_t i = 0; i < b->cap; i++) {
        b->digits[i] = 0;
    }
}

size_t get_size(const bn b)
{
    size_t leading_zeros = 0;
    for (int i = b->cap - 1; i > 0; i--) {
        if (b->digits[i] != 0)
            break;

        ++leading_zeros;
    }

    return b->cap - leading_zeros;

}

char* bn_string(const bn b)
{
    size_t size = (b->sgn) ? b->size + 1 : b->size;
    char *str = (char*)calloc(size, 1);

    for (int i = 0; i < b->size; i++)
        str[i + b->sgn] = b->digits[b->size - 1 - i] + '0';

    if (b->sgn)
        str[0] = '-';

    return str;
}

static inline void _u_bn_add(bn rop, const bn a, const bn b)
{
    bn tmp = bn_init();

    size_t max_size = a->size > b->size ? a->size + 1 : b->size + 1;
    int carry = 0;

    for (size_t i = 0; i < max_size; i++) {
        tmp->digits[i] = (a->digits[i] + b->digits[i] + carry) % 10;
        carry = (a->digits[i] + b->digits[i] + carry) / 10;
    }

    tmp->size = get_size(tmp);
    bn_set(rop, tmp);

    bn_clear(tmp);
}

static inline void _u_bn_sub(bn rop, const bn a, const bn b)
{
    bn tmp = bn_init();

    size_t max_size = a->size > b->size ? a->size : b->size;
    int carry = 0;
    int t = 0;

    for (size_t i = 0; i < max_size; i++) {
        t = a->digits[i] - b->digits[i] + carry;
        if (t < 0) {
            carry = -1;
            t += 10;
        } else {
            carry = 0;
        }
        tmp->digits[i] = t;
    }

    tmp->size = get_size(tmp);
    bn_set(rop, tmp);

    bn_clear(tmp);
}

int bn_add(bn rop, const bn a, const bn b)
{
    bn sum = bn_init();

    if (a->sgn == b->sgn) {
        // Positive + Positive or Negative + Negative //
        // a + b with sgn 0 or -(a + b) //
        _u_bn_add(sum, a, b);
        sum->sgn = a->sgn | b->sgn;
    } else if (a->sgn == 0 && b->sgn == 1) {
        // Positive + Negative //
        if (bn_cmp_abs(a, b) >= 0) {
            // |a| >= |b| ? |a| - |b| with sgn 0 //
            _u_bn_sub(sum, a, b);
            sum->sgn = 0;
        } else {
            // |b| - |a| with sgn 1 //
            _u_bn_sub(sum, b, a);
            sum->sgn = 1;
        }
    } else if (a->sgn == 1 && b->sgn == 0) {
        // Negative + Positive //
        if (bn_cmp_abs(a, b) == 1) {
            // |a| > b ? |a| - b with sgn 1 //
            _u_bn_sub(sum, a, b);
            sum->sgn = 1;
        } else {
            // b - |a| with sgn 0 //
            _u_bn_sub(sum, b, a);
            sum->sgn = 0;
        }
    } else {
        // Error Path //
        bn_clear(sum);
        return -1;
    }

    bn_set(rop, sum);

    bn_clear(sum);
    return 1;
}

int bn_add_ui(bn rop, const bn a, uint64_t b)
{
    bn B = bn_init_set_ui(b);
    bn_add(rop, a, B);
    bn_clear(B);
    return 1;
}

int bn_sub(bn rop, const bn a, const bn b)
{
    bn diff = bn_init();

    if (a->sgn ^ b->sgn) {
        // Positive - Negative or Negative - Positive //
        // a + |b| with sgn 0 or |a| + b with sgn 1 //
        _u_bn_add(diff, a, b);
        diff->sgn = a->sgn;
    } else if (a->sgn == 0 && b->sgn == 0) {
        // Positive - Positive //
        if (bn_cmp(a, b) >= 0) {
            // a - b with sgn 0 //
            _u_bn_sub(diff, a, b);
            diff->sgn = 0;
        } else {
            // |b - a| with sgn 1 //
            _u_bn_sub(diff, b, a);
            diff->sgn = 1;
        }
    } else if (a->sgn == 1 && b->sgn == 1) {
        // Negative - Negative //
        if (bn_cmp_abs(a, b) == 1) {
            // |a| - |b| with sgn 1 //
            _u_bn_sub(diff, a, b);
            diff->sgn = 1;                
        } else {
            // |b| - |a| with sgn 0 //
            _u_bn_sub(diff, b, a);
            diff->sgn = 0;
        }
    } else {
        // Error Occured //
        bn_clear(diff);
        return -1;
    }

    diff->size = get_size(diff);
    bn_set(rop, diff);

    bn_clear(diff);
    return 1;
}

int bn_sub_ui(bn rop, const bn a, uint64_t b)
{
    bn B = bn_init_set_ui(b);
    bn_sub(rop, a, B);
    bn_clear(B);
    return 1;
}

static inline void bn_mult_10(bn rop, const bn a, const size_t n)
{
    bn tmp = bn_init();

    if (a->size != 1 || a->digits[0] != 0) {
        tmp->size = a->size + n;

        for (int i = n, j = 0; i < tmp->size && j < a->size; i++, j++)
            tmp->digits[i] = a->digits[j];
        for (int i = 0; i < n; i++)
            tmp->digits[i] = 0;
    }

    bn_set(rop, tmp);
    rop->sgn = a->sgn;

    bn_clear(tmp);
}

static inline void bn_mul_single_dig(bn rop, const bn a, const int b)
{
    // Assert b < 10 //

    bn prod = bn_init();

    int exp = 0;
    int carry = 0;

    for (int i = 0; i < a->size + 1; i++) {
        bn t = bn_init_set_ui((b * a->digits[i] + carry) % 10);
        carry = (b * a->digits[i] + carry) / 10;

        bn_mult_10(t, t, exp++);
        _u_bn_add(prod, prod, t);
    }

    prod->size = get_size(prod);

    bn_set(rop, prod);
    bn_clear(prod);
}

void bn_mul(bn rop, const bn a, const bn b)
{
    bn prod = bn_init();

    size_t max_size = a->size + b->size;
    int exp = 0;
    
    for (int i = 0; i < max_size; i++) {
        bn sum = bn_init();
        int dig = b->digits[i];

        bn_mul_single_dig(sum, a, dig);
        bn_mult_10(sum, sum, exp++);

        _u_bn_add(prod, prod, sum);

        bn_clear(sum);
    }

    prod->size = get_size(prod);
    prod->sgn = a->sgn ^ b->sgn;
    bn_set(rop, prod);

    bn_clear(prod);
}

int bn_powm(bn rop, const bn base, const bn exp, const bn mod)
{
    bn _rop = bn_init_set_ui(1);
    bn _exp = bn_init(); bn_set(_exp, exp);
    bn _mod = bn_init(); bn_set(_mod, mod);

    bn ZERO = bn_init();
    bn ONE = bn_init_set_ui(1);

    while (bn_cmp(_exp, ZERO) > 0) {
        bn_mul(_rop, _rop, base);
        bn_div(_mod, _rop, _rop, mod);

        bn_sub(_exp, _exp, ONE);
    }

    bn_set(rop, _rop);

    bn_clear(ONE);
    bn_clear(ZERO);
    bn_clear(_exp);
    bn_clear(_rop);
    bn_clear(_mod);

    return 1;
}

int bn_pow(bn rop, const bn base, const bn exp)
{
    bn _rop = bn_init_set_ui(1);
    bn _exp = bn_init(); bn_set(_exp, exp);

    bn ZERO = bn_init();
    bn ONE = bn_init_set_ui(1);

    while (bn_cmp(_exp, ZERO) > 0) {
        bn_mul(_rop, _rop, base);
        bn_sub(_exp, _exp, ONE);
    }

    bn_set(rop, _rop);

    bn_clear(ONE);
    bn_clear(ZERO);
    bn_clear(_exp);
    bn_clear(_rop);

    return 1;
}

void bn_pow_ui(bn rop, const bn base, int64_t exp)
{
    bn tmp = bn_init_set_ui(1);

    while(exp--)
        bn_mul(tmp, tmp, base);

    tmp->size = get_size(tmp);

    bn_set(rop, tmp);
    bn_clear(tmp);
}

static inline int get_quot(const bn divd, const bn divr)
{
    bn _divd = bn_init();
    bn_set(_divd, divd);

    int q = 0;
    while (bn_cmp_abs(_divd, divr) > -1) {
        ++q;
        _u_bn_sub(_divd, _divd, divr);
    }

    bn_clear(_divd);

    return q;
}

static inline void next_partial_rem(bn rem, const bn divr, const bn divd, 
                                    int q, int i)
{
    bn _rem = bn_init();
    bn tmp = bn_init();

    bn_mul_single_dig(tmp, divr, q);

    bn_sub(_rem, rem, tmp);
    bn_mult_10(_rem, _rem, 1);

    bn _tmp = bn_init_set_ui(divd->digits[i - 1]);
    bn_add(_rem, _rem, _tmp);

    bn_set(rem, _rem);

    bn_clear(_rem);
    bn_clear(tmp);
    bn_clear(_tmp);
}

int bn_div(bn quot, bn mod, const bn divd, const bn divr)
{
    // Check divr not 0 return -1 or error //

    bn _quot = bn_init();
    bn _mod = bn_init();

    bn _divr = bn_init();
    bn_set(_divr, divr);
    _divr->sgn = 0;

    bn partial_rem = bn_init_set_ui(divd->digits[divd->size - 1]);
    bn tmp = bn_init();

    for (int i = divd->size - 1; i > 0; i--) {
        int q = get_quot(partial_rem, _divr);
        _quot->digits[i] = q;
        next_partial_rem(partial_rem, _divr, divd, q, i);
    }
    int q = get_quot(partial_rem, _divr);
    _quot->digits[0] = q;

    bn_mul_single_dig(tmp, _divr, q);
    _u_bn_sub(partial_rem, partial_rem, tmp);

    _quot->size = get_size(_quot);
    _quot->sgn = divd->sgn ^ divr->sgn;

    partial_rem->size = get_size(partial_rem);

    bn_set(quot, _quot);
    bn_set(mod, partial_rem);

    bn_clear(_divr);
    bn_clear(_quot);
    bn_clear(tmp);
    bn_clear(partial_rem);

    return 1;
}

int bn_mod(bn rop, const bn a, const bn b)
{
    bn _rop = bn_init(); 

    bn _dummy = bn_init();

    bn_div(_dummy, _rop, a, b);
    
    bn_set(rop, _rop);

    bn_clear(_dummy);
    bn_clear(_rop);

    return 1;
}

int bn_is_odd(const bn a)
{
    return a->digits[0] == 1 || a->digits[0] == 3 || a->digits[0] == 5 || 
           a->digits[0] == 7 || a->digits[0] == 9;
}

void bn_printf(const bn b)
{
    /*
     *Need to reimplement to start from first non-zero digit
     *Insread of going through entire number
     *Low priority
     */
    if (b->sgn == 1)
        putchar('-');

    int leading_zero = 1;

    for (int i = SIZE_BN - 1; i >= 0; i--) {
        if (b->digits[i] != 0)
            leading_zero = 0;
        if (leading_zero == 0)
            printf("%d", b->digits[i]);
    }
    if (leading_zero)
        printf("%d", 0);

    putchar('\n');
}