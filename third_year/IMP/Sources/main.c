/**
 * Autor: Patrik Holop (xholop01)
 * Podiel autorstva: originál
 * Posledná zmena: 15.12.2018
 */

#include <stdio.h>

#include "MK60D10.h"
#include "main.h"


void MCUInit(void) {
	// activate the clock
	MCG->C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);

	// allow the clock for CRC
	SIM->SCGC6 |= SIM_SCGC6_CRC_MASK;
}


uint16_t crc_16(uint32_t *input_data, int length, int polynomial, int seed, int tot, int totr, int fxor) {

	//	1. Clear CTRL[TCRC] to enable 16-bit CRC mode.
	CRC0->CTRL = CRC_CTRL_TCRC(0);
	uint32_t CRC_CTRL_STEP_1 = CRC0->CTRL;

	//	2. Program the transpose and complement options in the CTRL register as required for
	//     the CRC calculation. See Transpose feature and CRC result complement for details.
	// transposition of write data
	// transposition of read data
	// xor the input
	CRC0->CTRL |= CRC_CTRL_TOT(tot) | CRC_CTRL_TOTR(totr) | CRC_CTRL_FXOR(fxor);
	uint32_t CRC_CTRL_STEP_2 = CRC0->CTRL;

	//	3. Write a 16-bit polynomial to the GPOLY[LOW] field. The GPOLY[HIGH] field is
	//     not usable in 16-bit CRC mode.
	CRC0->GPOLY_ACCESS16BIT.GPOLYL = polynomial;
	uint16_t CRC_GPOLY_STEP_3 = CRC0->GPOLY_ACCESS16BIT.GPOLYL;

	//	4. Set CTRL[WAS] to program the seed value.
	CRC0->CTRL |= CRC_CTRL_WAS(1);
	uint32_t CRC_CTRL_STEP_4 = CRC0->CTRL;

	//	5. Write a 16-bit seed to CRC[LU:LL]. CRC[HU:HL] are not used.
	CRC0->ACCESS16BIT.CRCL = seed;
	uint16_t CRC_CRC_STEP_5 = CRC0->ACCESS16BIT.CRCL;

	//	6. Clear CTRL[WAS] to start writing data values.
	CRC0->CTRL = 0;
	CRC0->CTRL = CRC_CTRL_TCRC(0) | CRC_CTRL_TOT(tot) | CRC_CTRL_TOTR(totr) | CRC_CTRL_FXOR(fxor) | CRC_CTRL_WAS(0);

	uint32_t CRC_CTRL_STEP_6 = CRC0->CTRL;

	//	7. Write data values into CRC[HU:HL:LU:LL]. A CRC is computed on every data
	//	   value write, and the intermediate CRC result is stored back into CRC[LU:LL].
	for (int i = 0; i < length; i++) {
		CRC0->CRC = input_data[i];
		//uint32_t CRC_CRC_STEP_7 = CRC0->CRC;
	}

	//	8. When all values have been written, read the final CRC result from CRC[LU:LL].
	uint16_t RESULT_STEP_8 = CRC0->ACCESS16BIT.CRCL;
	return RESULT_STEP_8;
}


short crc_16_cdma2000_calculation(uint32_t *input_data, int length) {

	short crc_result = 0xFFFF; // initial seed
	short polynom = 0xC867;    // polynom

	for (int bcount = 0; bcount < length; bcount++) {             // iterate through integers
		for (int offset = 24; offset >= 0; offset -= 8) {         // iterate through bytes
			short crc_msb = (input_data[bcount] >> offset) << 8;
			crc_result = crc_result ^ crc_msb;                    // xor in the next byte

			for (int i = 0; i < BYTE_LENGTH; i++) {
				short msb = crc_result >> 15;
				crc_result = crc_result << 1;                     // shift the register
				if (msb != 0) {                                   // if shifted bit from register is 1
					crc_result = crc_result ^ polynom;            // XOR with the polynom
				}
			}
		}
	}
	return crc_result;
}


short crc16_cdma2000_table(uint32_t *input_data, int length) {

	short crc = 0xFFFF;
	uint32_t input = 0x0000;

	for (int bcount = 0; bcount < length; bcount++) {             // iterate through integers
		for (int i = 24; i >= 0; i-=8) {                          // iterate through bytes
			// find position in table based on MSB value of temporary CRC
			// XOR the found value to shifted CRC
			crc = (crc << 8) ^ lookup_table_crc_16[(unsigned char) ((crc >> 8) ^ (input_data[bcount] >> i))];
		}
	}
	return crc;
}


uint32_t crc_32(uint32_t *input_data, int length, int polynomial, int seed, int tot, int totr, int fxor) {

	//	1. Set CTRL[TCRC] to enable 32-bit CRC mode.
	CRC0->CTRL = CRC_CTRL_TCRC(1);
	uint32_t CRC_CTRL_STEP_1 = CRC0->CTRL;

	//	2. Program the transpose and complement options in the CTRL register as required for
	//	   the CRC calculation. See Transpose feature and CRC result complement for details.
	CRC0->CTRL |= CRC_CTRL_TOT(tot) | CRC_CTRL_TOTR(totr) | CRC_CTRL_FXOR(fxor);
	uint32_t CRC_CTRL_STEP_2 = CRC0->CTRL;

	//	3. Write a 32-bit polynomial to GPOLY[HIGH:LOW].
	CRC0->GPOLY = polynomial;
	uint32_t CRC_GPOLY_STEP_3 = CRC0->GPOLY;

	//	4. Set CTRL[WAS] to program the seed value.
	CRC0->CTRL |= CRC_CTRL_WAS(1);
	uint32_t CRC_CTRL_STEP_4 = CRC0->CTRL;

	//	5. Write a 32-bit seed to CRC[HU:HL:LU:LL].
	CRC0->CRC = seed;
	uint32_t CRC_CRC_STEP_5 = CRC0->CRC;

	//	6. Clear CTRL[WAS] to start writing data values.
	CRC0->CTRL = 0;
	CRC0->CTRL = CRC_CTRL_TCRC(1) | CRC_CTRL_TOT(tot) | CRC_CTRL_TOTR(totr) | CRC_CTRL_FXOR(fxor) | CRC_CTRL_WAS(0);

	uint32_t CRC_CTRL_STEP_6 = CRC0->CTRL;

	//	7. Write data values into CRC[HU:HL:LU:LL]. A CRC is computed on every data
	//	   value write, and the intermediate CRC result is stored back into
	//	   CRC[HU:HL:LU:LL].
	for (int i = 0; i < length; i++) {
			CRC0->CRC = input_data[i];
			//uint32_t CRC_CRC_STEP_7 = CRC0->CRC;
	}

	//	8. When all values have been written, read the final CRC result from
	//	   CRC[HU:HL:LU:LL]. The CRC is calculated bytewise, and two clocks are required
	//	   to complete one CRC calculation.
	uint32_t RESULT_STEP_8 = CRC0->CRC;

	return RESULT_STEP_8;
}


int crc_32_mpg2_calculation(uint32_t *input_data, int length) {

	int crc_result = 0xFFFFFFFF; // initial seed
	int polynom = 0x04C11DB7;        // polynom

	for (int bcount = 0; bcount < length; bcount++) {        // iterate through integers
		for (int i = 24; i >= 0; i -= 8) {					 // iterate through bytes
			int crc_msb = (input_data[bcount] >> i) << 24;
			crc_result = crc_result ^ crc_msb;				 // XOR in the next byte

			for (int i = 0; i < BYTE_LENGTH; i++) {
				short msb = crc_result >> 31;
				crc_result = crc_result << 1;				// shift the register
				if (msb != 0) {								// if the shifted value is 1
					crc_result = crc_result ^ polynom;		// XOR with the polynom
				}
			}
		}
	}
	return crc_result;
}


int crc_32_mpg2_table(uint32_t *input_data, int length) {

	int polynom = 0x04C11DB7;        // polynom
	int crc = 0xFFFFFFFF;			 // seed

	for (int bcount = 0; bcount < length; bcount++) {	// iterate through integers
		for (int i = 24; i >= 0; i-=8) {				// iterate through bytes
			// find position in table based on MSB value of temporary CRC
			// XOR the found value to shifted CRC
			crc = (crc << 8) ^ lookup_table_crc_32[(unsigned char) ((crc >> 24) ^ (input_data[bcount] >> i))];
		}
	}
	return crc;
}


void test_and_verify(uint32_t *input_data, int length) {

	// HW CRC MODULE
	printf("************* CRC 16 RESULTS *************\n");
	printf("*                                        *\n");

	uint16_t crc_16_cdma2000_hwmodule_RESULT = crc_16(input_data, length, configurations[3][0], configurations[3][1], configurations[3][2], configurations[3][3], configurations[3][4]);
	printf("*   CRC-16 HW module:          %#04x    *\n", crc_16_cdma2000_hwmodule_RESULT);

	// DIRECT CALCULATION
	short crc_16_cdma2000_calculation_result = crc_16_cdma2000_calculation(input_data, length);
	printf("*   CRC-16 direct calculation: %#04x    *\n", crc_16_cdma2000_calculation_result);

	// LOOKUP TABLE
	short crc16_cdma2000_table_result = crc16_cdma2000_table(input_data, length);
	printf("*   CRC-16 lookup table:       %#04x    *\n", crc16_cdma2000_table_result);

	printf("*                                        *\n");
	printf("******** CRC 16 VERIFICATION OK **********\n");
	printf("*                                        *\n");

	// prepare encoded data
	uint32_t encoded_data[length+1];
	for (int i = 0; i < length; i++) encoded_data[i] = input_data[i];

	// hw module verification
	encoded_data[length] = crc_16_cdma2000_hwmodule_RESULT << 16;
	uint16_t crc_16_cdma2000_hwmodule_VERIFICATION_OK = crc_16(encoded_data, length+1, configurations[3][0], configurations[3][1], configurations[3][2], configurations[3][3], configurations[3][4]);
	printf("*   CRC-16 HW module:          %#04x      *\n", crc_16_cdma2000_hwmodule_VERIFICATION_OK);

	// direct calculation
	encoded_data[length] = crc_16_cdma2000_calculation_result << 16;
	short crc_16_cdma2000_calculation_VERIFICATION_OK = crc_16_cdma2000_calculation(encoded_data, length+1);
	printf("*   CRC-16 direct calculation: %#04x      *\n", crc_16_cdma2000_calculation_VERIFICATION_OK);

	// lookup table
	encoded_data[length] = crc16_cdma2000_table_result << 16;
	short crc_16_cdma2000_table_VERIFICATION_OK = crc16_cdma2000_table(encoded_data, length+1);
	printf("*   CRC-16 lookup table:       %#04x      *\n", crc_16_cdma2000_table_VERIFICATION_OK);

	printf("*                                        *\n");
	printf("******* CRC 16 VERIFICATION ERROR ********\n");
	printf("*                                        *\n");

	// simulate transmission error
	encoded_data[0] = ~(encoded_data[0]);

	// hw module
	encoded_data[length] = crc_16_cdma2000_hwmodule_RESULT << 16;
	uint16_t crc_16_cdma2000_hwmodule_VERIFICATION_FAIL = crc_16(encoded_data, length+1, configurations[3][0], configurations[3][1], configurations[3][2], configurations[3][3], configurations[3][4]);
	printf("*   CRC-16 HW module:           %#04x   *\n", crc_16_cdma2000_hwmodule_VERIFICATION_FAIL);

	// direct calculation
	encoded_data[length] = crc_16_cdma2000_calculation_result << 16;
	short crc_16_cdma2000_calculation_VERIFICATION_FAIL = crc_16_cdma2000_calculation(encoded_data, length+1);
	printf("*   CRC-16 direct calculation:  %#04x   *\n", crc_16_cdma2000_calculation_VERIFICATION_FAIL);

	// lookup table
	encoded_data[length] = crc16_cdma2000_table_result << 16;
	short crc_16_cdma2000_table_VERIFICATION_FAIL = crc16_cdma2000_table(encoded_data, length+1);
	printf("*   CRC-16 lookup table:        %#04x   *\n", crc_16_cdma2000_table_VERIFICATION_FAIL);


	// CRC 32 verification
	printf("*                                        *\n");
	printf("************* CRC 32 RESULTS *************\n");
	printf("*                                        *\n");

	// HW CRC MODULE
	uint32_t crc_32_mpeg2_hwmodule_RESULT = crc_32(input_data, length, configurations[1][0], configurations[1][1], configurations[1][2], configurations[1][3], configurations[1][4]);
	printf("*  CRC-32 HW module:          %#08x *\n", crc_32_mpeg2_hwmodule_RESULT);

	uint32_t crc_32_mpeg2_calculation_RESULT = crc_32_mpg2_calculation(input_data, length);
	printf("*  CRC-32 direct calculation: %#08x *\n", crc_32_mpeg2_calculation_RESULT);

	uint32_t crc_32_mpeg2_table_RESULT = crc_32_mpg2_table(input_data, length);
	printf("*  CRC-32 lookup table:       %#08x *\n", crc_32_mpeg2_table_RESULT);

	printf("*                                        *\n");
	printf("******** CRC 32 VERIFICATION OK **********\n");
	printf("*                                        *\n");

	for (int i = 0; i < length; i++) encoded_data[i] = input_data[i];

	// hw module
	encoded_data[length] = crc_32_mpeg2_hwmodule_RESULT;
	uint32_t crc_32_mpeg2_hwmodule_VERIFICATION_OK = crc_32(encoded_data, length+1, configurations[1][0], configurations[1][1], configurations[1][2], configurations[1][3], configurations[1][4]);
	printf("*  CRC-32 HW module:          %#08x   *\n", crc_32_mpeg2_hwmodule_VERIFICATION_OK);

	// direct calculation
	encoded_data[length] = crc_32_mpeg2_calculation_RESULT;
	uint32_t crc_32_mpeg2_calculation_VERIFICATION_OK = crc_32_mpg2_calculation(encoded_data, length+1);
	printf("*  CRC-32 direct calculation: %#08x   *\n", crc_32_mpeg2_calculation_VERIFICATION_OK);

	// lookup table
	encoded_data[length] = crc_32_mpeg2_table_RESULT;
	uint32_t crc_32_mpeg2_table_VERIFICATION_OK = crc_32_mpg2_table(encoded_data, length+1);
	printf("*  CRC-32 lookup table:       %#08x   *\n", crc_32_mpeg2_table_VERIFICATION_OK);

	printf("*                                        *\n");
	printf("******** CRC 32 VERIFICATION ERROR *******\n");
	printf("*                                        *\n");
	// simulate transmission error
	encoded_data[0] = ~(encoded_data[0]);

	// hw module
	encoded_data[length] = crc_32_mpeg2_hwmodule_RESULT;
	uint32_t crc_32_mpeg2_hwmodule_VERIFICATION_FAIL = crc_32(encoded_data, length+1, configurations[1][0], configurations[1][1], configurations[1][2], configurations[1][3], configurations[1][4]);
	printf("*  CRC-32 HW module:          %#08x  *\n", crc_32_mpeg2_hwmodule_VERIFICATION_FAIL);

	// direct calculation
	encoded_data[length] = crc_32_mpeg2_calculation_RESULT;
	uint32_t crc_32_mpeg2_calculation_VERIFICATION_FAIL = crc_32_mpg2_calculation(encoded_data, length+1);
	printf("*  CRC-32 direct calculation: %#08x  *\n", crc_32_mpeg2_calculation_VERIFICATION_FAIL);

	// lookup table
	encoded_data[length] = crc_32_mpeg2_table_RESULT;
	uint32_t crc_32_mpeg2_table_VERIFICATION_FAIL = crc_32_mpg2_table(encoded_data, length+1);
	printf("*  CRC-32 lookup table:       %#08x  *\n", crc_32_mpeg2_table_VERIFICATION_FAIL);
	printf("*                                        *\n");
	printf("*************** Finished *****************\n");
}


int main(void)
{
	printf("* Project: CRC-16 & CRC-32 *\n");
	MCUInit();

	int length = 2;
	uint32_t input_data[2] = {0x12345678, 0x87654321};

	// test CRC results
	test_and_verify(input_data, length);

	while (1) {
	}

    return 0;
}
