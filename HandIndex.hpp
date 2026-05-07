/**
 * hand_index.h
 *
 * @author Kevin Waugh (waugh@cs.cmu.edu)
 * @date April 13, 2013
 *
 * map poker hands to an index shared by all isomorphic hands, and
 * map an index to a canonical poker hand
 */

#ifndef _HAND_INDEX_H_
#define _HAND_INDEX_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cpluspus */

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
//
#include "Deck.hpp"

#define MAX_ROUNDS 8

typedef uint64_t hand_index_t;
typedef struct hand_indexer_s hand_indexer_t;
typedef struct hand_indexer_state_s hand_indexer_state_t;

#define PRIhand_index PRIu64

//
struct hand_indexer_s {
    uint8_t cards_per_round[MAX_ROUNDS];
    uint8_t round_start[MAX_ROUNDS];
    uint_fast32_t rounds;
    uint_fast32_t configurations[MAX_ROUNDS];
    uint_fast32_t permutations[MAX_ROUNDS];
    hand_index_t round_size[MAX_ROUNDS];
    //
    uint_fast32_t *permutation_to_configuration[MAX_ROUNDS];
    uint_fast32_t *permutation_to_pi[MAX_ROUNDS];
    uint_fast32_t *configuration_to_equal[MAX_ROUNDS];
    uint_fast32_t (* configuration[MAX_ROUNDS])[SUITS];
    uint_fast32_t (* configuration_to_suit_size[MAX_ROUNDS])[SUITS];
    hand_index_t *configuration_to_offset[MAX_ROUNDS];
};

//
struct hand_indexer_state_s {
    uint_fast32_t suit_index[SUITS];
    uint_fast32_t suit_multiplier[SUITS];
    uint_fast32_t round;
    uint_fast32_t permutation_index;
    uint_fast32_t permutation_multiplier;
    uint32_t used_ranks[SUITS];
};


/**
 * Initialize a hand indexer.  This generates a number of lookup tables and is relatively
 * expensive compared to indexing a hand.
 *
 * @param rounds number of rounds
 * @param cards_per_round number of cards in each round
 * @param indexer
 */
bool hand_indexer_init(uint_fast32_t rounds, const uint8_t cards_per_round[], hand_indexer_t *indexer);

/**
 * Free a hand indexer.
 *
 * @param indexer
 */
void hand_indexer_free(hand_indexer_t *indexer);

/**
 * @param indexer
 * @param round
 * @returns size of index for hands on round
 */
hand_index_t hand_indexer_size(const hand_indexer_t *indexer, uint_fast32_t round);

/**
 * Initialize a hand index state.  This is used for incrementally indexing a hand as
 * new rounds are dealt and determining if a hand is canonical.
 *
 * @param indexer
 * @param state
 */
void hand_indexer_state_init(const hand_indexer_t *indexer, hand_indexer_state_t *state);

/**
 * Index a hand on every round.  This is not more expensive than just indexing the last round.
 *
 * @param indexer
 * @param cards
 * @param indices
 * @returns hand's index on the last round
 */
hand_index_t hand_index_all(const hand_indexer_t *indexer, const uint8_t cards[], hand_index_t indices[]);

/**
 * Index a hand on the last round.
 *
 * @param indexer
 * @param cards
 * @returns hand's index on the last round
 */
hand_index_t hand_index_last(const hand_indexer_t *indexer, const uint8_t cards[]);

/**
 * Incrementally index the next round.
 *
 * @param indexer
 * @param cards the cards for the next round only!
 * @param state
 * @returns the hand's index at the latest round
 */
hand_index_t hand_index_next_round(const hand_indexer_t *indexer, const uint8_t cards[], hand_indexer_state_t *state);

/**
 * Recover the canonical hand from a particular index.
 *
 * @param indexer
 * @param round
 * @param index
 * @param cards
 * @returns true if successful
 */
bool hand_unindex(const hand_indexer_t *indexer, uint_fast32_t round, hand_index_t index, uint8_t cards[]);

#ifdef __cplusplus
}
#endif /* __cpluspus */

#endif /* _HAND_INDEX_H_ */
