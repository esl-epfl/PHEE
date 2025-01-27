#ifndef FFT_HPP_
#define FFT_HPP_

#include <stdio.h>

#include "arithmetic.h"
#include "twiddles.h"

struct complex {
    Real Re, Im;
};

// Returns 1 (true) if N is a power of 2
// M is number of stages to perform. 2^M = N
int isPwrTwo(int N, int *M) {
    // Check if N is positive and has only one set bit
    if (N <= 0 || (N & (N - 1)) != 0) {
        return 0;
    }

    // Calculate log2(N) and store the result in M
    *M = 0;
    while (N > 1) {
        N >>= 1;
        (*M)++;
    }

    return 1;
}

void rad2FFT(int N, struct complex *x, struct complex *DFT) {
    int M;

    // Check if power of two. If not, exit
    if (!isPwrTwo(N, &M)) {
        printf("Rad2FFT(): N must be a power of 2 for Radix FFT\n");
        return;
    }

    // Wn is the exponential weighting function in the form a + jb
    // complex<Real> WN;
    struct complex TEMP;  // TEMP is used to save computation in the butterfly calc
    struct complex *pDFT = DFT;  // Pointer to first elements in DFT array
    struct complex *pLo;         // Pointer for lo / hi value of butterfly calcs
    struct complex *pHi;
    struct complex *pX;  // Pointer to x[n]

    // Decimation In Time - x[n] sample sorting
    int MM1 = M - 1;
    for (unsigned int i = 0; i < (unsigned int)N; i++, DFT++) {
        pX = x + i;  // Calculate current x[n] from base address *x and index i.
        int ii = 0;  // Reset new address for DFT[n]
        unsigned int iaddr = i;  // Copy i for manipulations

        // Bit reverse i and store in ii...
        for (int l = 0; l < M; l++) {
            if (iaddr & 0x01) {          // Detemine least significant bit
                ii += (1 << (MM1 - l));  // Increment ii by 2^(M-1-l) if lsb was 1
            }
            iaddr >>= 1;  // right shift iaddr to test next bit. Use logical operations
                          // for speed increase
            if (!iaddr) {
                break;
            }
        }
        DFT = pDFT + ii;   // Calculate current DFT[n] from base address *pDFT and bit
                           // reversed index ii
        DFT->Re = pX->Re;  // Update the complex array with address sorted time domain
                           // signal x[n]
        DFT->Im = pX->Im;  // NB: Imaginary is always zero
    }

    // FFT Computation by butterfly calculation
    // Loop for M stages, where 2^M = N
    for (int stage = 1; stage <= M; stage++) {
        int BSep = 1 << stage;  // Separation between butterflies = 2^stage
        int P = N / BSep;       // Number of similar Wn's in this stage
        // Butterfly width (spacing between opposite points) = Separation / 2.
        int BWidth = BSep >> 1;

        // Loop for j calculations per butterfly
        // Separate j == 0 case from other cases
        // Loop for HiIndex Step BSep butterflies per stage
        // HiIndex is the index of the DFT array for the top value of each butterfly
        // calc
        for (int HiIndex = 0; HiIndex < N; HiIndex += BSep) {
            pHi = pDFT + HiIndex;  // Point to higher value
            pLo = pHi + BWidth;    // Point to lower value (Note VC++ adjusts for
                                   // spacing between elements)

            TEMP.Re = pLo->Re;
            TEMP.Im = pLo->Im;

            // CSub (pHi, &TEMP, pLo);
            // Find new Lovalue (complex subtraction)
            // pLo->Re = pHi->Re - TEMP.Re;
            // pLo->Im = pHi->Im - TEMP.Im;

            // CAdd (pHi, &TEMP, pHi);
            // Find new Hivalue (complex addition)
            // pHi->Re = pHi->Re + TEMP.Re;
            // pHi->Im = pHi->Im + TEMP.Im;

            asm volatile(
                "plw     pt0,0(%4)       \n\t"  // Load pHi->Re
                "plw     pt1,0(%5)       \n\t"  // Load pHi->Im
                "plw     pt2,0(%6)       \n\t"  // Load TEMP.Re
                "plw     pt3,0(%7)       \n\t"  // Load TEMP.Im

                "psub.s  pt4,pt0,pt2     \n\t"  // pLo->Re = pHi->Re - TEMP.Re
                "psw     pt4,0(%8)       \n\t"  // Store pLo->Re
                "psub.s  pt5,pt1,pt3     \n\t"  // pLo->Im = pHi->Im - TEMP.Im
                "psw     pt5,0(%9)       \n\t"  // Store pLo->Im
                "padd.s  pt6,pt0,pt2     \n\t"  // pHi->Re = pHi->Re + TEMP.Re
                "psw     pt6,0(%4)       \n\t"  // Store pHi->Re
                "padd.s  pt7,pt1,pt3     \n\t"  // pHi->Im = pHi->Im + TEMP.Im
                "psw     pt7,0(%5)       \n\t"  // Store pHi->Im
                : "=rm"(pLo->Re), "=rm"(pLo->Im), "=rm"(pHi->Re), "=rm"(pHi->Im)
                : "r"(&pHi->Re), "r"(&pHi->Im), "r"(&TEMP.Re), "r"(&TEMP.Im),
                  "r"(&pLo->Re), "r"(&pLo->Im)
                :);
        }
        for (int j = 1; j < BWidth; j++) {
            // Only when j > 0 to save on calculation if R = 0, as WN^0 = (1 + j0)
            // Calculate Wn (Real and Imaginary)
            // WN.Re = twiddles_128[P * j].cosine;
            // WN.Im = twiddles_128[P * j].sine;

            // Preload twiddles
            // WN.Re will be in pt8 and WN.Im will be in pt9
            asm volatile(
                "plw     pt8,0(%0)       \n\t"  // Load twiddles_128[P * j].cosine
                "plw     pt9,0(%1)       \n\t"  // Load twiddles_128[P * j].sine
                :
                : "r"(&twiddles_128[P * j].cosine), "r"(&twiddles_128[P * j].sine)
                :);

            // Loop for HiIndex Step BSep butterflies per stage
            for (int HiIndex = j; HiIndex < N; HiIndex += BSep) {
                pHi = pDFT + HiIndex;  // Point to higher value
                pLo = pHi + BWidth;    // Point to lower value (Note VC++ adjusts for
                                       // spacing between elements)

                // CMult(pLo, &WN, &TEMP);
                // Perform complex multiplication of Lovalue with Wn
                // TEMP.Re = (pLo->Re * WN.Re) - (pLo->Im * WN.Im);
                // TEMP.Im = (pLo->Re * WN.Im) + (pLo->Im * WN.Re);

                asm volatile(
                    "plw     pt0,0(%2)       \n\t"  // Load pLo->Re
                    "plw     pt1,0(%3)       \n\t"  // Load pLo->Im
                    "pmul.s  pt2,pt0,pt8     \n\t"  // pLo->Re * WN.Re
                    "pmul.s  pt3,pt1,pt9     \n\t"  // pLo->Im * WN.Im
                    "pmul.s  pt4,pt0,pt9     \n\t"  // pLo->Re * WN.Im
                    "pmul.s  pt5,pt1,pt8     \n\t"  // pLo->Im * WN.Re
                    "psub.s  pt6,pt2,pt3     \n\t"  // Compute TEMP.Re
                    "psw     pt6,0(%4)       \n\t"  // Store TEMP.Re
                    "padd.s  pt7,pt4,pt5     \n\t"  // Compute TEMP.Im
                    "psw     pt7,0(%5)       \n\t"  // Store TEMP.Im
                    : "=rm"(TEMP.Re), "=rm"(TEMP.Im)
                    : "r"(&pLo->Re), "r"(&pLo->Im), "r"(&TEMP.Re), "r"(&TEMP.Im)
                    :);

                // CSub (pHi, &TEMP, pLo);
                // Find new Lovalue (complex subtraction)
                // pLo->Re = pHi->Re - TEMP.Re;
                // pLo->Im = pHi->Im - TEMP.Im;

                // CAdd (pHi, &TEMP, pHi);
                // Find new Hivalue (complex addition)
                // pHi->Re = pHi->Re + TEMP.Re;
                // pHi->Im = pHi->Im + TEMP.Im;

                asm volatile(
                    "plw     pt0,0(%4)       \n\t"  // Load pHi->Re
                    "plw     pt1,0(%5)       \n\t"  // Load pHi->Im
                    "plw     pt2,0(%6)       \n\t"  // Load TEMP.Re
                    "plw     pt3,0(%7)       \n\t"  // Load TEMP.Im

                    "psub.s  pt4,pt0,pt2     \n\t"  // pLo->Re = pHi->Re - TEMP.Re
                    "psw     pt4,0(%8)       \n\t"  // Store pLo->Re
                    "psub.s  pt5,pt1,pt3     \n\t"  // pLo->Im = pHi->Im - TEMP.Im
                    "psw     pt5,0(%9)       \n\t"  // Store pLo->Im
                    "padd.s  pt6,pt0,pt2     \n\t"  // pHi->Re = pHi->Re + TEMP.Re
                    "psw     pt6,0(%4)       \n\t"  // Store pHi->Re
                    "padd.s  pt7,pt1,pt3     \n\t"  // pHi->Im = pHi->Im + TEMP.Im
                    "psw     pt7,0(%5)       \n\t"  // Store pHi->Im
                    : "=rm"(pLo->Re), "=rm"(pLo->Im), "=rm"(pHi->Re), "=rm"(pHi->Im)
                    : "r"(&pHi->Re), "r"(&pHi->Im), "r"(&TEMP.Re), "r"(&TEMP.Im),
                        "r"(&pLo->Re), "r"(&pLo->Im)
                    :);
            }
        }
    }

    pLo = 0;  // Null all pointers
    pHi = 0;
    pDFT = 0;
    DFT = 0;
    pX = 0;
}

#endif  // FFT_HPP_