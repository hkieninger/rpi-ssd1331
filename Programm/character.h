#ifndef CHARACTER_H_
#define CHARACTER_H_ 1

#include <stdint.h> //uint64_t

/*
 * this header contains "bitmaps" in hex form to describe the characters,
 * the characters have a width of 5 pixel and a height of 7 pixel
 * each pixel is either set (1) or not set (0)
 * all pixel together give a binary number with 35 digits, wich corresponds to a hex number with max 9 digits
 * example for the description of the character A
 *
 *                           43210
 * most significant bit -> 6 ..$..
 *                         5 .$.$.
 *                         4 $...$
 *                         3 $...$
 *                         2 $$$$$
 *                         1 $...$
 *                         0 $...$ <- least significant bit
 *
 * binary number: 00100'01010'10001'1001'11111'10001'10001
 * hex number: 11518FE31
 */

 #define CHARACTER_WIDTH 5
 #define CHARACTER_HEIGHT 7

extern const uint64_t LETTER_CHARACTER[] = {
    0x11518FE31, // A
    0x7A31F463E, // B
    0x1D1084107, // C
    0x72518C65C, // D
    0x7E10F421F, // E
    0x7E10F4210, // F
    0x1D109C526, // G
    0x4631FC631, // H
    0x108421084, // I
    0x04210C544, // J
    0x4654C5251, // K
    0x42108421F, // L
    0x463BAC631, // M
    0x4639ACE31, // N
    0x3A318C62E, // O
    0x7A31F4210, // P
    0x3A318D5C2, // Q
    0x7A31F7251, // R
    0x3E107043E, // S
    0x7C8421084, // T
    0x46318C62E, // U
    0x46318A944, // V
    0x4631AEE31, // W
    0x462A22A31, // X
	0x463151084, // Y
	0x7C222221F  // Z
};

extern const uint64_t DIGIT_CHARACTER[] = {
    0x3A318C62E, // 0
    0x0CA988421, // 1
    0x3A222221F, // 2
    0x78217043E, // 3
    0x0CA98FC21, // 4
    0x7E107043E, // 5
    0x3A10F462E, // 6
    0x7C2222210, // 7
    0x3A317462E, // 8
    0x3A317842E  // 9
};

extern const uint64_t PUNCTUATION_CHARACTER[] = {
    0x000000000, // "Space"
    0x108421004, // !
    0x782111004  // ?
};

extern const uint64_t OPERATOR_CHARACTER[] = {
    0x0C8882083, // <
    0x001F07C00, // =
    0x608208898, // >
    0x0084F9080, // +
    0x0000F8000, // -
    0x022A22A20, // *
    0x044222110  // /
};

extern const uint64_t UNKNOWN_CHARACTER = 0x7FFFFFFFF;

#endif /* CHARACTER_H_ */
