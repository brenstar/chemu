#include "chemu/decode.h"

#include <stdio.h>
#include <assert.h>

//
// typedef struct {
//     ChipInst instruction;
//     int expectedIndex;
// } IndexTest;

//typedef enum {
//	RANGE_FIXED, RANGE_RANGED
//} RangeType;
//
//struct FixedRange {
//	ChipInst startInst;
//	ChipInst stopInst;
//	ChipInst step;
//	int expectedIndex;
//};
//
//struct RangedRange {
//	struct FixedRange startRange;
//	struct FixedRange endRange;
//	ChipInst step;
//};
//
//typedef struct {
//	union {
//		struct FixedRange fixed;
//		struct RangedRange ranged;
//	} range;
//	RangeType type;
//} RangeTest;

typedef struct {
    ChipInst startInst;
    ChipInst stopInst;
    ChipInst step;
    int expectedIndex;
} RangeTest;

typedef struct IndexTest_s {
	ChipInst base;
	ChipInst mask;
	int expectedIndex;
	const char name[5];
} IndexTest;

typedef struct {
    ChipInst inst;
    ChipInst_AType aExpected;
    ChipInst_IType iExpected;
    ChipInst_RType rExpected;
} DecodeTest;

//#define FRange(start, end, step, expected) {{start, end, step, expected}, RANGE_FIXED}
//#define RRange(start, end, step) {{start, end, step}, RANGE_RANGED}
//
//static const RangeTest RANGE_TESTS[] = {
//	FRange(0x0000, 0x00DF, 1, 0), // sys
//	FRange(0x00E0, 0x00E0, 1, 1), // cls
//	FRange(0x00E1, 0x00ED, 1, 0), // sys
//	FRange(0x00EE, 0x00EE, 1, 2), // ret
//	FRange(0x00EF, 0x0FFF, 1, 0), // sys
//	FRange(0x1000, 0x1FFF, 1, 3), // j
//	FRange(0x2000, 0x2FFF, 1, 4), // call
//	FRange(0x3000, 0x3FFF, 1, 5), // sei
//	FRange(0x4000, 0x4FFF, 1, 6), // sni
//	RRange({0x5000, 0x5FF0, 16, 7}, {0x})
//}
 

static const IndexTest INDEX_TESTS[] = {
	{0x0000, 0xF000, 0, "sys"}, // sys (instructions 0x0000 to 0x0FFF)
	{0x00E0, 0xFFFF, 1, "cls"}, // cls (instruction 0x00E0 only)
	{0x00EE, 0xFFFF, 2, "ret"}, // ret (instruction 0x00EE only)
	{0x1000, 0xF000, 3, "j"}, // j   (instructions 0x1000 to 0x1FFF)
	{0x2000, 0xF000, 4, "call"}, // call
	{0x3000, 0xF000, 5, "sei"},
	{0x4000, 0xF000, 6, "sni"},
	{0x5000, 0xF00F, 7, "se"},
	{0x6000, 0xF000, 8, "li"},
	{0x7000, 0xF000, 9, "addi"},
	{0x8000, 0xF00F, 10, "move"},
	{0x8001, 0xF00F, 11, "or"},
	{0x8002, 0xF00F, 12, "and"},
	{0x8003, 0xF00F, 13, "xor"},
	{0x8004, 0xF00F, 14, "add"},
	{0x8005, 0xF00F, 15, "sub"},
	{0x8006, 0xF00F, 16, "shr"},
	{0x8007, 0xF00F, 17, "subn"},
	{0x800E, 0xF00F, 18, "shl"},
	{0x9000, 0xF00F, 19, "sn"},
	{0xA000, 0xF000, 20, "la"},
	{0xB000, 0xF000, 21, "jo"},
	{0xC000, 0xF000, 22, "rnd"},
	{0xD000, 0xF000, 23, "draw"},
	{0xE09E, 0xF0FF, 24, "sip"},
	{0xE0A1, 0xF0FF, 25, "snip"},
	{0xF007, 0xF0FF, 26, "ld"},
	{0xF00A, 0xF0FF, 27, "lk"},
	{0xF015, 0xF0FF, 28, "del"},
	{0xF018, 0xF0FF, 29, "snd"},
	{0xF01E, 0xF0FF, 30, "ii"},
	{0xF029, 0xF0FF, 31, "font"},
	{0xF033, 0xF0FF, 32, "bcd"},
	{0xF055, 0xF0FF, 33, "save"},
	{0xF065, 0xF0FF, 34, "rest"}

};

/*
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
	{0x5001, 0x5FF1, 16, NO_INSTRUCTION}, // illegal
	{0x5002, 0x5FF2, 16, NO_INSTRUCTION}, // illegal
	{0x5003, 0x5FF3, 16, NO_INSTRUCTION}, // illegal
	{0x5004, 0x5FF4, 16, NO_INSTRUCTION}, // illegal
	{0x5005, 0x5FF5, 16, NO_INSTRUCTION}, // illegal
	{0x5006, 0x5FF6, 16, NO_INSTRUCTION}, // illegal
	{0x5007, 0x5FF7, 16, NO_INSTRUCTION}, // illegal
	{0x5008, 0x5FF8, 16, NO_INSTRUCTION}, // illegal
	{0x5009, 0x5FF9, 16, NO_INSTRUCTION}, // illegal
	{0x500A, 0x5FFA, 16, NO_INSTRUCTION}, // illegal
	{0x500B, 0x5FFB, 16, NO_INSTRUCTION}, // illegal
	{0x500C, 0x5FFC, 16, NO_INSTRUCTION}, // illegal
	{0x500D, 0x5FFD, 16, NO_INSTRUCTION}, // illegal
	{0x500E, 0x5FFE, 16, NO_INSTRUCTION}, // illegal
	{0x500F, 0x5FFF, 16, NO_INSTRUCTION}, // illegal
	{0x6000, 0x6FFF, 1, 8}, // li
	{0x7000, 0x7FFF, 1, 9}, // addi
	{0x8000, 0x8FF0, 16, 10}, // move
	{0x8001, 0x8FF1, 16, 11}, // or
	{0x8002, 0x8FF2, 16, 12}, // and
	{0x8003, 0x8FF3, 16, 13}, // xor
	{0x8004, 0x8FF4, 16, 14}, // add
	{0x8005, 0x8FF5, 16, 15}, // sub
	{0x8006, 0x8FF6, 16, 16}, // shr
	{0x8007, 0x8FF7, 16, 17}, // subn
	{0x8008, 0x8FF8, 16, NO_INSTRUCTION}, // illegal
	{0x8009, 0x8FF9, 16, NO_INSTRUCTION}, // illegal
	{0x800A, 0x8FFA, 16, NO_INSTRUCTION}, // illegal
	{0x800B, 0x8FFB, 16, NO_INSTRUCTION}, // illegal
	{0x800C, 0x8FFC, 16, NO_INSTRUCTION}, // illegal
	{0x800D, 0x8FFD, 16, NO_INSTRUCTION}, // illegal
	{0x800E, 0x8FFE, 16, 18}, // shl
	{0x800F, 0x8FFF, 16, NO_INSTRUCTION}, // illegal
	{0x9000, 0x9FF0, 16, 19}, // sn
	{0x9001, 0x9FF1, 16, NO_INSTRUCTION}, // illegal
	{0x9002, 0x9FF2, 16, NO_INSTRUCTION}, // illegal
	{0x9003, 0x9FF3, 16, NO_INSTRUCTION}, // illegal
	{0x9004, 0x9FF4, 16, NO_INSTRUCTION}, // illegal
	{0x9005, 0x9FF5, 16, NO_INSTRUCTION}, // illegal
	{0x9006, 0x9FF6, 16, NO_INSTRUCTION}, // illegal
	{0x9007, 0x9FF7, 16, NO_INSTRUCTION}, // illegal
	{0x9008, 0x9FF8, 16, NO_INSTRUCTION}, // illegal
	{0x9009, 0x9FF9, 16, NO_INSTRUCTION}, // illegal
	{0x900A, 0x9FFA, 16, NO_INSTRUCTION}, // illegal
	{0x900B, 0x9FFB, 16, NO_INSTRUCTION}, // illegal
	{0x900C, 0x9FFC, 16, NO_INSTRUCTION}, // illegal
	{0x900D, 0x9FFD, 16, NO_INSTRUCTION}, // illegal
	{0x900E, 0x9FFE, 16, NO_INSTRUCTION}, // illegal
	{0x900F, 0x9FFF, 16, NO_INSTRUCTION}, // illegal
	{0xA000, 0xAFFF, 1, 20}, // la
	{0xB000, 0xBFFF, 1, 21}, // jo
	{0xC000, 0xCFFF, 1, 22}, // rnd
	{0xD000, 0xDFFF, 1, 23}, // draw


};*/


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

	puts("\nStarting index tests.");
    puts("Note that 'sys' will have two incorrect results, this is expected.\n");

	size_t testCount = sizeof(INDEX_TESTS) / sizeof(IndexTest);
	for (size_t i = 0; i < testCount; ++i) {
		IndexTest test = INDEX_TESTS[i];
		printf("Testing '%s'\n", test.name);
		int hits = 0;
		for (int j = 0; j <= 0xFFFF; ++j) {
			ChipInst inst = (ChipInst)j;
			int index = chemu_dec_index(inst);
			bool match = index == test.expectedIndex;

			if ((inst & test.mask) == test.base) {
				if (!match)
					printf("\t0x%04X. [Incorrect index] Got: %d Expected: %d\n", inst, index, test.expectedIndex);
				else
					++hits;
			} else {
				if (match)
					printf("\t0x%04X. [Mismatch]\n", inst);
			}

		}
		printf("\tHits: %d\n", hits);
	}

    // puts("Testing instructions 0x000-0x00DF expecting 0...");
    // rangeTester(0x0000, 0x00DF, 0);

   /* size_t testCount = sizeof(RANGE_TESTS) / sizeof(RangeTest);
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
    }*/

    puts("\nStarting Decode tests.\n");

    ChipInstDec dec;
    size_t decTestCount = sizeof(DECODE_TESTS) / sizeof(DecodeTest);
    for (size_t i = 0; i < decTestCount; ++i) {
        DecodeTest test = DECODE_TESTS[i];
        printf("Decoding 0x%04X\n", test.inst);
        // A-type
        puts("A-Type");
        dec = chemu_dec_decode(test.inst, INST_CLASS_A);
        printf("\tliteral     = 0x%X    (expected: 0x%X)\n", dec.a.literal, test.aExpected.literal);
        assert(dec.a.literal == test.aExpected.literal);
        printf("\taddr        = 0x%03X  (expected: 0x%03X)\n", dec.a.addr, test.aExpected.addr);
        assert(dec.a.addr == test.aExpected.addr);

        // i-type
        puts("I-Type");
        dec = chemu_dec_decode(test.inst, INST_CLASS_I);
        printf("\tliteral     = 0x%X    (expected: 0x%X)\n", dec.i.literal, test.iExpected.literal);
        assert(dec.i.literal == test.iExpected.literal);
        printf("\trnum        = 0x%X    (expected: 0x%X)\n", dec.i.rnum, test.iExpected.rnum);
        assert(dec.i.rnum == test.iExpected.rnum);
        printf("\timmediate   = 0x%02X   (expected: 0x%02X)\n", dec.i.immediate, test.iExpected.immediate);
        assert(dec.i.immediate == test.iExpected.immediate);

        // R-Type
        puts("R-Type");
        dec = chemu_dec_decode(test.inst, INST_CLASS_R);
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
        dec = chemu_dec_decode(test.inst, INST_CLASS_V);
        printf("\tinstruction = 0x%04X (expected: 0x%04X)\n", dec.instruction, test.inst);
        assert(dec.instruction == test.inst);

        printf("\n");
    }

    // ChipInst testInst = 0xABCD;
    // ChipInstDec dec;
    // printf("Decoding 0x%04X as a-type instruction\n", testInst);
    // dec = chemu_dec_decode(testInst, INST_CLASS_A);
    // printf("literal = 0x%X   (expected: 0xA)\n", dec.a.literal);
    // printf("addr    = 0x%03X (expected: 0xBCD)\n", dec.a.addr);
    // printf("Decoding 0x%04X as i-type instruction\n", testInst);
    // dec = chemu_dec_decode(testInst, INST_CLASS_I);



    // size_t testCount = sizeof(INDEX_TESTS) / sizeof(IndexTest);
    // for (size_t i = 0; i < testCount; ++i) {
    //     IndexTest test = INDEX_TESTS[i];
    //     printf("(%d/%d) Decoding 0x%04X...", i + 1, testCount, test.instruction);
    //     int index = chemu_dec_index(test.instruction);
    //     printf("%d (Expected: %d)\n", index, test.expectedIndex);
    // }


    return 0;
}

static int rangeTester(RangeTest test) {
    int errors = 0;
    for (ChipInst i = test.startInst; i <= test.stopInst; i += test.step) {
        int index = chemu_dec_index(i);
        if (index != test.expectedIndex) {
            printf("Instruction 0x%04X was indexed as %d\n", i, index);
            ++errors;
        }
    }
    return errors;
}
