/*
 *
 *
 *
 *
 */

#ifndef BN_H
#define BN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SIZE_BN 2048

typedef struct bn_struct
{
    uint8_t *digits;
    size_t size;
    size_t sgn;
    size_t cap;
} bn_struct;

typedef bn_struct *bn;

// Initialization //

bn bn_init();
bn bn_init_set_ui(uint64_t);
bn bn_init_set_si(int64_t);
bn bn_init_set_str(const char*);
bn bn_init_set(const bn); //<-

void bn_set(bn, const bn);
void bn_set_ui(bn, const uint64_t);//<-
void bn_set_si(bn, const int64_t); //<-
void bn_set_str(bn, const char*);  //<-
void bn_reset(bn);

void bn_clear(bn);

// Arithmitic //

int bn_add(bn, const bn, const bn);
int bn_sub_ui(bn, const bn, uint64_t);

int bn_sub(bn, const bn, const bn);
int bn_sub_ui(bn, const bn, uint64_t);

void bn_mul(bn, const bn, const bn);

// Exponentiation //

int bn_powm(bn, const bn, const bn, const bn);
void bn_pow_ui(bn, const bn, int64_t);
int bn_pow(bn, const bn, const bn);

void bn_pow_ui(bn, const bn, int64_t);
void bn_ui_pow_ui(bn, uint64_t, uint64_t);

// Division //

int bn_div(bn, bn, const bn, const bn);
int bn_mod(bn, const bn, const bn);

// Compare //

int bn_cmp(const bn, const bn);
int bn_cmp_abs(const bn, const bn);

// Print //

void bn_printf(const bn);

// Misc. //

size_t get_size(const bn);
char* bn_string(const bn);
int bn_is_odd(const bn);

#endif // BN_H //