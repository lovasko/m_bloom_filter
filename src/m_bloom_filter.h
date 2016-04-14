#ifndef M_BLOOM_FILTER_H
#define M_BLOOM_FILTER_H

#include <stdlib.h>
#include <stdint.h>
#include <m_bit_set.h>

extern uint32_t m_bloom_filter_seeds[100];

#define M_BLOOM_FILTER_OK             0
#define M_BLOOM_FILTER_FALSE          1
#define M_BLOOM_FILTER_MAYBE          2
#define M_BLOOM_FILTER_E_NULL         3
#define M_BLOOM_FILTER_E_SIZE         4
#define M_BLOOM_FILTER_E_COUNT        5
#define M_BLOOM_FILTER_E_ERROR        6
#define M_BLOOM_FILTER_E_NHASH        7
#define M_BLOOM_FILTER_E_UNKNOWN_CODE 8
#define M_BLOOM_FILTER_E_MAX          8

typedef struct m_bloom_filter {
	struct m_bit_set mem;
	uint32_t* hashes;
	uint32_t nhash;
	float scale;
} m_bloom_filter;

int m_bloom_filter_init(m_bloom_filter* bf, uint32_t n, float err);
int m_bloom_filter_free(m_bloom_filter* bf);
int m_bloom_filter_dup(m_bloom_filter* dup_bf, m_bloom_filter* orig_bf);
int m_bloom_filter_add(m_bloom_filter* bf, void* data, size_t size);
int m_bloom_filter_test(m_bloom_filter* bf, void* data, size_t size);
int m_bloom_filter_error_string(int code, char** out_error_string);

#endif

