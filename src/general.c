#include <stdlib.h>
#include <math.h>

#include "m_bloom_filter.h"

int
m_bloom_filter_init(struct m_bloom_filter* bf, uint32_t n, float err)
{
	float bits;

	if (bf == NULL)
		return M_BLOOM_FILTER_E_NULL;

	if (n == 0)
		return M_BLOOM_FILTER_E_COUNT;

	if (err <= 0.0f || err >= 1.0f)
		return M_BLOOM_FILTER_E_ERROR;

	bits = -n * logf(err) / (float)(M_LN2 * M_LN2);
	m_bit_set_init(&bf->mem, ((uint32_t)bits)+1, NULL);

	bf->scale = bits / (float)UINT32_MAX;
	bf->nhash = (uint32_t)(0.7f*bits/(float)n);
	if (bf->nhash > sizeof(m_bloom_filter_seeds) / sizeof(uint32_t))
		return M_BLOOM_FILTER_E_NHASH;
	bf->hashes = malloc(sizeof(uint32_t) * bf->nhash);

	return M_BLOOM_FILTER_OK;
}

int
m_bloom_filter_dup(struct m_bloom_filter* bf_dup,
                   struct m_bloom_filter* bf_orig)
{
	int ret;

	if (bf_dup == NULL || bf_orig == NULL)
		return M_BLOOM_FILTER_E_NULL;

	ret = m_bit_set_dup(&bf_dup->mem, &bf_orig->mem, NULL);
	if (ret != M_BIT_SET_OK)
		return ret;
	
	bf_dup->scale = bf_orig->scale;
	bf_dup->nhash = bf_orig->nhash;
	bf_dup->hashes = malloc(sizeof(uint32_t) * bf_dup->nhash);

	return M_BLOOM_FILTER_OK;
}

int
m_bloom_filter_free(struct m_bloom_filter* bf)
{
	if (bf == NULL)
		return M_BLOOM_FILTER_E_NULL;

	m_bit_set_free(&bf->mem);
	free(bf->hashes);

	return M_BLOOM_FILTER_OK;
}

int
m_bloom_filter_error_string(int code, char** out_error_string)
{
	static const char* strings[] = {
		"OK",
		"Such element is not present",
		"Such element might be present",
		"Argument is NULL",
		"Size argument is zero",
		"Element count is zero",
		"Invalid error probability",
		"Insufficient hash function count",
		"Unknown code"
	};

	if (out_error_string == NULL)
		return M_BLOOM_FILTER_E_NULL;

	if (code < 0 || code > M_BLOOM_FILTER_E_UNKNOWN_CODE)
		return M_BLOOM_FILTER_E_UNKNOWN_CODE;

	*out_error_string = (char*)strings[code];
	return M_BLOOM_FILTER_OK;
}

