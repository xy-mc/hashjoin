/**
 * @file    types.h
 * @author  Cagri Balkesen <cagri.balkesen@inf.ethz.ch>
 * @date    Tue May 22 16:43:30 2012
 * @version $Id: types.h 4419 2013-10-21 16:24:35Z bcagri $
 * 
 * @brief  Provides general type definitions used by all join algorithms.
 * 
 * 
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#ifdef __cplusplus
#include <span>
#endif // __cplusplus

#ifdef __cplusplus
namespace eth_hashjoin {
#endif // __cplusplus


/**
 * @defgroup Types Common Types
 * Common type definitions used by all join implementations.
 * @{
 */

#ifdef KEY_8B /* 64-bit key/value, 16B tuples */
typedef int64_t intkey_t;
typedef int64_t value_t;
#else /* 32-bit key/value, 8B tuples */
typedef int32_t intkey_t;
typedef int32_t value_t;
#endif

typedef struct tuple_t    tuple_t;
typedef struct relation_t relation_t;

typedef struct result_t result_t;
typedef struct threadresult_t threadresult_t;

/** Type definition for a tuple, depending on KEY_8B a tuple can be 16B or 8B */
struct tuple_t {
    intkey_t key;
    value_t  payload;
};

/**
 * Type definition for a relation. 
 * It consists of an array of tuples and a size of the relation.
 */
struct relation_t {
  tuple_t * tuples;
  uint64_t  num_tuples;

#ifdef __cplusplus
    relation_t() : relation_t(nullptr, 0) {}
    relation_t(tuple_t * tuples, uint64_t num_tuples) : tuples(tuples), num_tuples(num_tuples) {}
    relation_t(const std::span<tuple_t>& span) : tuples(span.data()), num_tuples(span.size()) {}
    operator std::span<tuple_t>() { return {tuples, num_tuples}; }
#endif // __cplusplus
};

/** Holds the join results of a thread */
struct threadresult_t {
    int64_t  nresults;
    void *   results;
    uint32_t threadid;
};

/** Type definition for join results. */
struct result_t {
    int64_t          totalresults;
    threadresult_t * resultlist;
    int              nthreads;
};

/** @} */

#ifdef __cplusplus
} // namespace eth_hashjoin
#endif // __cplusplus
#endif /* TYPES_H */
