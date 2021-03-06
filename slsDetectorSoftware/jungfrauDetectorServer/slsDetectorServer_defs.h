#ifndef SLSDETECTORSERVER_DEFS_H
#define SLSDETECTORSERVER_DEFS_H

#include "sls_detector_defs.h" 	//default dynamicgain in settings
#include "RegisterDefs.h"
#include <stdint.h>



#define GOODBYE 					(-200)
#define MIN_REQRD_VRSN_T_RD_API     0x171113
#define REQRD_FRMWR_VRSN            0x171113


/* Struct Definitions */
typedef struct ip_header_struct {
	uint16_t     ip_len;
	uint8_t      ip_tos;
	uint8_t      ip_ihl:4 ,ip_ver:4;
	uint16_t     ip_offset:13,ip_flag:3;
	uint16_t     ip_ident;
	uint16_t     ip_chksum;
	uint8_t      ip_protocol;
	uint8_t      ip_ttl;
	uint32_t     ip_sourceip;
	uint32_t     ip_destip;
} ip_header;

/* Enums */
enum CLK_SPEED_INDEX		{FULL_SPEED, HALF_SPEED, QUARTER_SPEED};
enum ADCINDEX				{TEMP_FPGA, TEMP_ADC};
enum DACINDEX				{VB_COMP, VDD_PROT, VIN_COM, VREF_PRECH, VB_PIXBUF, VB_DS, VREF_DS, VREF_COMP };
#define DEFAULT_DAC_VALS   	{ 	1220,	/* VB_COMP */		\
								3000,	/* VDD_PROT */		\
								1053,	/* VIN_COM */		\
								1450,	/* VREF_PRECH */	\
								750,	/* VB_PIXBUF */		\
								1000,	/* VB_DS */			\
								480,	/* VREF_DS */		\
								420		/* VREF_COMP */		\
							};

#define NUM_SETTINGS		6
#define DEFAULT_SETT_INDX	{DYNAMICGAIN, DYNAMICHG0, FIXGAIN1, FIXGAIN2, FORCESWITCHG1, FORCESWITCHG2};
#define DEFAULT_SETT_VALS	{	0x0f00,		/* DYNAMICGAIN	 	*/	\
 	  	  	  	  	  	  	  	0x0f01,		/* DYNAMICHG0		*/	\
  	  	  	  	  	  	  	  	0x0f02,		/* FIXGAIN1			*/	\
  	  	  	  	  	  	  	  	0x0f06,		/* FIXGAIN2			*/	\
  	  	  	  	  	  	  	  	0x1f00,		/* FORCESWITCHG1	*/	\
 	  	  	  	  	  	  	  	0x3f00		/* FORCESWITCHG2	*/	\
 	  	  	  	  	  	  	 };
#define DEFAULT_SETT_NAMES  {	"Dynamic Gain", 		/* DYNAMICGAIN	*/	\
								"Dynamic High Gain 0",	/* DYNAMICHG0	*/	\
								"Fix Gain 1",			/* FIXGAIN1		*/	\
								"Fix Gain 2",			/* FIXGAIN2		*/	\
								"Force Switch Gain 1",	/* FORCESWITCHG1*/	\
								"Force Switch Gain 2"	/* FORCESWITCHG2*/	\
							};
enum NETWORKINDEX           { TXN_FRAME };



/* Hardware Definitions */
#define NMAXMOD 					(1)
#define NMOD 						(1)
#define NCHAN 						(256 * 256)
#define NCHIP 						(8)
#define NADC						(0)
#define NDAC 						(8)
#define NDAC_OLDBOARD				(16)
#define DYNAMIC_RANGE				(16)
#define NUM_BITS_PER_PIXEL			(DYNAMIC_RANGE / 8)
#define DATA_BYTES					(NCHIP * NCHAN * NUM_BITS_PER_PIXEL)
#define IP_PACKETSIZE				(0x2052)
#define CLK_RUN						(40)	/* MHz */
#define CLK_SYNC					(20)	/* MHz */


/** Default Parameters */
#define DEFAULT_NUM_FRAMES			(100*1000*1000)
#define DEFAULT_NUM_CYCLES			(1)
#define DEFAULT_EXPTIME				(10*1000)		//ns
#define DEFAULT_PERIOD				(2*1000*1000)	//ns
#define DEFAULT_DELAY				(0)
#define DEFAULT_HIGH_VOLTAGE		(0)
#define DEFAULT_TIMING_MODE			(AUTO_TIMING)
#define DEFAULT_SETTINGS			(DYNAMICGAIN)
#define DEFAULT_TX_UDP_PORT			(0x7e9a)
#define DEFAULT_TMP_THRSHLD         (65*1000) //milli degree Celsius

/* Defines in the Firmware */
#define FIX_PATT_VAL    			(0xACDC2014)
#define ADC_PORT_INVERT_VAL   		(0x453b2a9c)
#define MAX_TIMESLOT_VAL            (0x1F)
#define MAX_THRESHOLD_TEMP_VAL      (127999) //millidegrees


#define SAMPLE_ADC_HALF_SPEED	 	(SAMPLE_DECMT_FACTOR_2_VAL + SAMPLE_DGTL_SAMPLE_0_VAL + SAMPLE_ADC_DECMT_FACTOR_0_VAL + SAMPLE_ADC_SAMPLE_0_VAL)	/* 0x1000 */
#define SAMPLE_ADC_QUARTER_SPEED 	(SAMPLE_DECMT_FACTOR_4_VAL + SAMPLE_DGTL_SAMPLE_8_VAL + SAMPLE_ADC_DECMT_FACTOR_1_VAL + SAMPLE_ADC_SAMPLE_0_VAL)	/* 0x2810 */
#define CONFIG_HALF_SPEED			(CONFIG_TDMA_DISABLE_VAL + CONFIG_HALF_SPEED_20MHZ_VAL + CONFIG_MODE_1_X_10GBE_VAL)
#define CONFIG_QUARTER_SPEED		(CONFIG_TDMA_DISABLE_VAL + CONFIG_QUARTER_SPEED_10MHZ_VAL + CONFIG_MODE_1_X_10GBE_VAL)
#define ADC_OFST_HALF_SPEED_VAL		(0x20) //adc pipeline
#define ADC_OFST_QUARTER_SPEED_VAL	(0x0f)
#define ADC_PHASE_HALF_SPEED 		(0x48) //72
#define ADC_PHASE_QUARTER_SPEED 	(0x48) //72

/* Maybe not required for jungfrau */
#define NTRIMBITS 					(6)
#define NCOUNTBITS 					(24)
#define NCHIPS_PER_ADC				(2)
#define TRIM_DR 					(((int)pow(2,NTRIMBITS))-1)
#define COUNT_DR 					(((int)pow(2,NCOUNTBITS))-1)
#define ALLMOD 						(0xffff)
#define ALLFIFO 					(0xffff)

/* MSB & LSB DEFINES */
#define MSB_OF_64_BIT_REG_OFST		(32)
#define LSB_OF_64_BIT_REG_OFST		(0)
#define BIT_32_MSK					(0xFFFFFFFF)

/* LTC2620 DAC DEFINES */
#define LTC2620_DAC_CMD_OFST		(20)
#define LTC2620_DAC_CMD_MSK			(0x0000000F << LTC2620_DAC_CMD_OFST)
#define LTC2620_DAC_ADDR_OFST		(16)
#define LTC2620_DAC_ADDR_MSK		(0x0000000F << LTC2620_DAC_ADDR_OFST)
#define LTC2620_DAC_DATA_OFST		(4)
#define LTC2620_DAC_DATA_MSK		(0x00000FFF << LTC2620_DAC_DATA_OFST)

#define LTC2620_DAC_CMD_WRITE		(0x00000000 << LTC2620_DAC_CMD_OFST)
#define LTC2620_DAC_CMD_SET			(0x00000003 << LTC2620_DAC_CMD_OFST)
#define LTC2620_DAC_CMD_POWER_DOWN	(0x00000004 << LTC2620_DAC_CMD_OFST)
#define LTC2620_DAC_NUMBITS			(24)


/* MAX1932 HV DEFINES */
#define MAX1932_HV_NUMBITS			(8)
#define MAX1932_HV_DATA_OFST		(0)
#define MAX1932_HV_DATA_MSK			(0x000000FF << MAX1932_HV_DATA_OFST)



/** PLL Reconfiguration Registers */
//https://www.altera.com/documentation/mcn1424769382940.html
#define PLL_MODE_REG 				(0x00)
#define PLL_STATUS_REG 				(0x01)
#define PLL_START_REG 				(0x02)
#define PLL_N_COUNTER_REG 			(0x03)
#define PLL_M_COUNTER_REG 			(0x04)
#define PLL_C_COUNTER_REG 			(0x05)
#define PLL_PHASE_SHIFT_REG			(0x06)

#define PLL_SHIFT_NUM_SHIFTS_OFST	(0)
#define PLL_SHIFT_NUM_SHIFTS_MSK	(0x0000FFFF << PLL_SHIFT_NUM_SHIFTS_OFST)

#define PLL_SHIFT_CNT_SELECT_OFST	(16)
#define PLL_SHIFT_CNT_SELECT_MSK	(0x0000001F << PLL_SHIFT_CNT_SELECT_OFST)
#define PLL_SHIFT_CNT_SLCT_C0_VAL	((0x0 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C1_VAL	((0x1 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C2_VAL	((0x2 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C3_VAL	((0x3 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C4_VAL	((0x4 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C5_VAL	((0x5 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C6_VAL	((0x6 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C7_VAL	((0x7 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C8_VAL	((0x8 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C9_VAL	((0x9 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C10_VAL	((0x10 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C11_VAL	((0x11 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C12_VAL	((0x12 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C13_VAL	((0x13 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C14_VAL	((0x14 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C15_VAL	((0x15 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C16_VAL	((0x16 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)
#define PLL_SHIFT_CNT_SLCT_C17_VAL	((0x17 << PLL_SHIFT_CNT_SELECT_OFST) & PLL_SHIFT_CNT_SELECT_MSK)

#define PLL_SHIFT_UP_DOWN_OFST		(21)
#define PLL_SHIFT_UP_DOWN_MSK		(0x00000001 << PLL_SHIFT_UP_DOWN_OFST)
#define PLL_SHIFT_UP_DOWN_NEG_VAL	((0x0 << PLL_SHIFT_UP_DOWN_OFST) & PLL_SHIFT_UP_DOWN_MSK)
#define PLL_SHIFT_UP_DOWN_POS_VAL	((0x1 << PLL_SHIFT_UP_DOWN_OFST) & PLL_SHIFT_UP_DOWN_MSK)

#define PLL_K_COUNTER_REG 			(0x07)
#define PLL_BANDWIDTH_REG 			(0x08)
#define PLL_CHARGEPUMP_REG 			(0x09)
#define PLL_VCO_DIV_REG 			(0x1c)
#define PLL_MIF_REG 				(0x1f)


#endif /* SLSDETECTORSERVER_DEFS_H */
