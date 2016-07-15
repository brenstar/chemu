#include "chemu/decode.h"

#include <stdio.h>
#include <assert.h>

//
// typedef struct {
//     ChipInst instruction;
//     int expectedIndex;
// } IndexTest;

typedef struct {
    ChipInst startInst;
    ChipInst stopInst;
    ChipInst step;
    int expectedIndex;
} RangeTest;

typedef struct {
    ChipInst inst;
    ChipInst_AType aExpected;
    ChipInst_IType iExpected;
    ChipInst_RType rExpected;
} DecodeTest;

static const RangeTest RANGE_TESTS[] = {
    {0x0000, 0x00DF, 1, 0}, // sys
    {0x00E0, 0x00E0, 1, 1}, // cls
    {0x00E1, 0x00ED, 1, 0}, // sys
    {0x00EE, 0x00EE, 1, 2}, // ret
    {0x00EF, 0x0FFF, 1, 0}, // sys
    {0x1000, 0x1FFF, 1, 3}, // j
    {0x2000, 0x2FFF, 1, 4}, // call
    {0x3000, 0x3FFF, 1, 5}, // sei
    {0x4000, 0x4FFF, 1, 6}, // sni
    {0x5000, 0x5FF0, 16, 7}, // se
};


//#define makeDecTest(n4, n3, n2, n1) { 0x ## n4 ## n3 ## n2 ## n1, {0x ## n4, 0x ## n3 ## n2 ## n1}, {0x}}

#define makeDecTest(n4, n3, n2, n1) { 0x ## n4 ## n3 ## n2 ## n1, \
    {0x ## n4, 0x ## n3 ## n2 ## n1}, \
    {0x ## n4, 0x ## n3, 0x ## n2 ## n1}, \
    {0x ## n4, 0x ## n3, 0x ## n2, 0x ## n1}}



static const DecodeTest DECODE_TESTS[] = {
//    { 0xABCD, {0xA, 0xBCD}, {0xA, 0xB, 0xCD}, {0xA, 0xB, 0xC, 0xD} },
    makeDecTest(A, B, C, D),
    makeDecTest(F, 0, F, 0),
    makeDecTest(A, E, E, A),
    makeDecTest(5, 5, 5, F)
};

// static const IndexTest INDEX_TESTS[] = {
//     {0x0000, 0},    // sys
//     {0x00F0, 0},    // sys
//     {0x00E0, 1},    // cls
//     {0x00EE, 2},    // ret
//     {0x10ee, 3},    // j
//     {0x20ea, 4},    //call
//     {0x3f02, 5},    //sei
//     {0x4a10, 6},    //sni
//     {0x5000, 7},    // se
//     {0x5002, -1},   // illegal
//     {0x6890, 8},    // li
//
// };

static int rangeTester(RangeTest test);


int main() {

    puts("\nStarting index tests.\n");

    // puts("Testing instructions 0x000-0x00DF expecting 0...");
    // rangeTester(0x0000, 0x00DF, 0);

    size_t testCount = sizeof(RANGE_TESTS) / sizeof(RangeTest);
    for (size_t i = 0; i < testCount; ++i) {
        RangeTest test = RANGE_TESTS[i];
        printf("Testing instruction");
        if (test.startInst == test.stopInst)
            printf("  0x%04X\t", test.startInst);
        else
            printf("s 0x%04X-0x%04X", test.startInst, test.stopInst);
        printf("\texpecting %d\n", test.expectedIndex);
        int errors = rangeTester(test);
        if (errors > 0)
            printf("Errors: %d/%d\n", errors, test.stopInst - test.startInst + 1);
    }

    puts("\nStarting Decode tests.\n");

    ChipInstDec dec;
    size_t decTestCount = sizeof(DECODE_TESTS) / sizeof(DecodeTest);
    for (size_t i = 0; i < decTestCount; ++i) {
        DecodeTest test = DECODE_TESTS[i];
        printf("Decoding 0x%04X\n", test.inst);
        // A-type
        puts("A-Type");
        dec = chipdec_decode(test.inst, INST_CLASS_A);
        printf("\tliteral     = 0x%X    (expected: 0x%X)\n", dec.a.literal, test.aExpected.literal);
        assert(dec.a.literal == test.aExpected.literal);
        printf("\taddr        = 0x%03X  (expected: 0x%03X)\n", dec.a.addr, test.aExpected.addr);
        assert(dec.a.addr == test.aExpected.addr);

        // i-type
        puts("I-Type");
        dec = chipdec_decode(test.inst, INST_CLASS_I);
        printf("\tliteral     = 0x%X    (expected: 0x%X)\n", dec.i.literal, test.iExpected.literal);
        assert(dec.i.literal == test.iExpected.literal);
        printf("\trnum        = 0x%X    (expected: 0x%X)\n", dec.i.rnum, test.iExpected.rnum);
        assert(dec.i.rnum == test.iExpected.rnum);
        printf("\timmediate   = 0x%02X   (expected: 0x%02X)\n", dec.i.immediate, test.iExpected.immediate);
        assert(dec.i.immediate == test.iExpected.immediate);

        // R-Type
        puts("R-Type");
        dec = chipdec_decode(test.inst, INST_CLASS_R);
        printf("\tliteral_hi  = 0x%X    (expected: 0x%X)\n", dec.r.literal_hi, test.rExpected.literal_hi);
        assert(dec.r.literal_hi == test.rExpected.literal_hi);
        printf("\trnum_dest   = 0x%X    (expected: 0x%X)\n", dec.r.rnum_dest, test.rExpected.rnum_dest);
        assert(dec.r.rnum_dest == test.rExpected.rnum_dest);
        printf("\trnum_src    = 0x%X    (expected: 0x%X)\n", dec.r.rnum_src, test.rExpected.rnum_src);
        assert(dec.r.rnum_src == test.rExpected.rnum_src);
        printf("\tliteral_lo  = 0x%X    (expected: 0x%X)\n", dec.r.literal_lo, test.rExpected.literal_lo);
        assert(dec.r.literal_lo == test.rExpected.literal_lo);

        // V-Type
        puts("V-Type");
        dec = chipdec_decode(test.inst, INST_CLASS_V);
        printf("\tinstruction = 0x%04X (expected: 0x%04X)\n", dec.instruction, test.inst);
        assert(dec.instruction == test.inst);

        printf("\n");
    }

    // ChipInst testInst = 0xABCD;
    // ChipInstDec dec;
    // printf("Decoding 0x%04X as a-type instruction\n", testInst);
    // dec = chipdec_decode(testInst, INST_CLASS_A);
    // printf("literal = 0x%X   (expected: 0xA)\n", dec.a.literal);
    // printf("addr    = 0x%03X (expected: 0xBCD)\n", dec.a.addr);
    // printf("Decoding 0x%04X as i-type instruction\n", testInst);
    // dec = chipdec_decode(testInst, INST_CLASS_I);



    // size_t testCount = sizeof(INDEX_TESTS) / sizeof(IndexTest);
    // for (size_t i = 0; i < testCount; ++i) {
    //     IndexTest test = INDEX_TESTS[i];
    //     printf("(%d/%d) Decoding 0x%04X...", i + 1, testCount, test.instruction);
    //     int index = chipdec_index(test.instruction);
    //     printf("%d (Expected: %d)\n", index, test.expectedIndex);
    // }


    return 0;
}

static int rangeTester(RangeTest test) {
    int errors = 0;
    for (ChipInst i = test.startInst; i <= test.stopInst; i += test.step) {
        int index = chipdec_index(i);
        if (index != test.expectedIndex) {
            printf("Instruction 0x%04X was indexed as %d\n", i, index);
            ++errors;
        }
    }
    return errors;
}
