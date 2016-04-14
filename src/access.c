#include <stdlib.h>
#include <math.h>
#include <murmur3.h>

#include "m_bloom_filter.h"

int
m_bloom_filter_add(m_bloom_filter* bf, void* data, size_t size)
{
	uint32_t i;

	if (bf == NULL || data == NULL)
		return M_BLOOM_FILTER_E_NULL;

	if (size == 0)
		return M_BLOOM_FILTER_E_SIZE;

	murmur3n(data, size, bf->nhash, m_bloom_filter_seeds, &bf->hashes);

	for (i = 0; i < bf->nhash; i++)
		m_bit_set_add(&bf->mem, (uint32_t)(bf->hashes[i] * bf->scale));

	return M_BLOOM_FILTER_OK;
}

int
m_bloom_filter_test(m_bloom_filter* bf, void* data, size_t size)
{
	uint32_t i;

	if (bf == NULL || data == NULL)
		return M_BLOOM_FILTER_E_NULL;

	if (size == 0)
		return M_BLOOM_FILTER_E_SIZE;

	murmur3n(data, size, bf->nhash, m_bloom_filter_seeds, &bf->hashes);

	for (i = 0; i < bf->nhash; i++)
		if (m_bit_set_test(&bf->mem, (uint32_t)(bf->hashes[i] * bf->scale))
		  == M_BIT_SET_FALSE)
			return M_BLOOM_FILTER_FALSE;

	return M_BLOOM_FILTER_MAYBE;
}

