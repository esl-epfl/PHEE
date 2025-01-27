#ifndef TWIDDLES_HPP_
#define TWIDDLES_HPP_

/**
 * This file contains precomputed twiddle factors for the FFT
 * computation. Twiddle factors are computed with cos() and sin() on a
 * specific phase angle. Each twiddle factor is stored inside a struct
 * containing cos and sin computation of a given phase. The sine is
 * negated because the FFT algorithm uses the negative sine value.
 *
 * The factors are subdivided in different arrays, each one is very
 * specific for the amount of FFT samples considered:
 * twiddles_128 --> twiddle factors computed for 128-sized fft.
 *
 * There are arrays for the following sizes:
 * - 128
 *
 * Pay attention to the fact that these sizes are very application specific.
 * Here they were considered for the specific application in which this file
 * is used.
*/

#include "arithmetic.h"

typedef struct {
    Real cosine;
    Real sine;
} twiddles_t;

static const twiddles_t twiddles_128[64] = {
    { 0x40000000, 0x00000000 },
    { 0x3FFB10F2, 0xE1B82684 },
    { 0x3FEC46D2, 0xDB742CA2 },
    { 0x3FD3AAC0, 0xD69BF7C8 },
    { 0x3FB14BE8, 0xD383A3E2 },
    { 0x3F853F7E, 0xD0730342 },
    { 0x3F4FA0AB, 0xCEB5FCE8 },
    { 0x3F109082, 0xCD3832C5 },
    { 0x3EC835E8, 0xCBC10EAD },
    { 0x3E76BD7A, 0xCA5177FB },
    { 0x3E1C5979, 0xC8EA5164 },
    { 0x3DB941A3, 0xC7C63C33 },
    { 0x3D4DB314, 0xC71C6263 },
    { 0x3CD9F024, 0xC6780402 },
    { 0x3C5E4035, 0xC5D9866D },
    { 0x3BDAEF92, 0xC5414B66 },
    { 0x3B504F33, 0xC4AFB0CD },
    { 0x3ABEB49A, 0xC425106F },
    { 0x3A267993, 0xC3A1BFCB },
    { 0x3987FBFE, 0xC3260FDC },
    { 0x38E39D9D, 0xC2B24CEB },
    { 0x3839C3CC, 0xC246BE5D },
    { 0x3715AE9C, 0xC1E3A687 },
    { 0x35AE8806, 0xC1894286 },
    { 0x343EF152, 0xC137CA18 },
    { 0x32C7CD3B, 0xC0EF6F7D },
    { 0x314A0318, 0xC0B05F55 },
    { 0x2F8CFCBA, 0xC07AC082 },
    { 0x2C7C5C1D, 0xC04EB418 },
    { 0x29640839, 0xC02C5541 },
    { 0x248BD366, 0xC013B92E },
    { 0x1E47D979, 0xC004EF0E },
    { 0x00610B46, 0xC0000000 },
    { 0xE1B8268B, 0xC004EF0E },
    { 0xDB742C9E, 0xC013B92E },
    { 0xD69BF7C9, 0xC02C5540 },
    { 0xD383A3E5, 0xC04EB418 },
    { 0xD0730340, 0xC07AC082 },
    { 0xCEB5FCE9, 0xC0B05F55 },
    { 0xCD3832C6, 0xC0EF6F7D },
    { 0xCBC10EAB, 0xC137CA19 },
    { 0xCA5177FB, 0xC1894286 },
    { 0xC8EA5162, 0xC1E3A688 },
    { 0xC7C63C34, 0xC246BE5D },
    { 0xC71C6263, 0xC2B24CEC },
    { 0xC6780400, 0xC3260FDD },
    { 0xC5D9866E, 0xC3A1BFCA },
    { 0xC5414B66, 0xC425106F },
    { 0xC4AFB0CC, 0xC4AFB0CE },
    { 0xC425106F, 0xC5414B65 },
    { 0xC3A1BFCA, 0xC5D9866E },
    { 0xC3260FDB, 0xC6780403 },
    { 0xC2B24CEC, 0xC71C6262 },
    { 0xC246BE5D, 0xC7C63C34 },
    { 0xC1E3A686, 0xC8EA5167 },
    { 0xC1894286, 0xCA5177FA },
    { 0xC137CA18, 0xCBC10EAE },
    { 0xC0EF6F7D, 0xCD3832C9 },
    { 0xC0B05F55, 0xCEB5FCE7 },
    { 0xC07AC082, 0xD0730345 },
    { 0xC04EB418, 0xD383A3DA },
    { 0xC02C5541, 0xD69BF7C6 },
    { 0xC013B92E, 0xDB742CA8 },
    { 0xC004EF0E, 0xE1B82675 }
};

#endif  // TWIDDLES_HPP_