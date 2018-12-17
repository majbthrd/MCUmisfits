#ifndef __SWM050_H__
#define __SWM050_H__


/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  BOD_IRQ			    	= -14,    /*!< 2 Non Maskable Interrupt                             */
  HardFault_IRQn	       	= -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
  SVCall_IRQn              	= -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
  PendSV_IRQn              	= -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
  SysTick_IRQn             	= -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/******  DEMO Cortex-M0 specific Interrupt Numbers ********************************************/
  TMRSE0_IRQn               = 0,
  TMRSE1_IRQn               = 1,
  WDT_IRQn				    = 2,
  ACMP_IRQn        			= 3,
  GPIO0_IRQn  				= 4,
  GPIO1_IRQn        		= 5,
  GPIO2_IRQn         		= 6,
  GPIO3_IRQn           		= 7,
  GPIO4_IRQn         	  	= 8,
  GPIO5_IRQn         		= 9,
  GPIO6_IRQn         		= 10,
  GPIO7_IRQn	    		= 11,
  GPIO8_IRQn                = 12,
  GPIO9_IRQn                = 13,
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT           0       /*!< UART does not provide a MPU present or not       */
#define __NVIC_PRIO_BITS        2       /*!< UART Supports 2 Bits for the Priority Levels     */
#define __Vendor_SysTickConfig  0       /*!< Set to 1 if different SysTick Config is used         */

#if   defined ( __CC_ARM )
  #pragma anon_unions
#endif

#include <stdio.h>
#include "core_cm0.h"                   /* Cortex-M0 processor and core peripherals               */
#include "system_SWMxxx.h"          	


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

typedef struct {
	union {
		__IO uint32_t u32CLK_CFG;
		struct{
			__IO uint32_t CORE_CLK_DIV  :10;	//=0��2��Ƶ			=����������Ƶ		=ż�� ��Ƶ
			__IO uint32_t 	            : 6;		
			__IO uint32_t TMRSE_CLK_DIV : 6;	//=0��2��Ƶ			=����������Ƶ		=ż�� ��Ƶ��ֱ�Ӷ�ʱ��Դ��Ƶ�����������ں�ʱ��
			__IO uint32_t               :10;
		} CLK_CFG;
	};

	union {
		__IO uint32_t u32PCLK_EN;
		struct{
			__IO uint32_t            : 4;
			__IO uint32_t WDT_CLK    : 1;
			__IO uint32_t            : 1;
            __IO uint32_t TMRSE0_CLK : 1;
			__IO uint32_t            :10;
			__IO uint32_t TMRSE1_CLK : 1;
			__IO uint32_t            :14;
		} PCLK_EN;
	};
    
    union {
        __IO uint32_t u32DBL_IRC;
        struct {
            __IO uint32_t DBL_IRC : 1;      	//0 18MHz       1 36MHz
            __IO uint32_t         :31;
        } DBL_IRC;
    };

	union {
		__IO uint32_t u32OSC_DIS;
		struct {
			__IO uint32_t OSC_DIS : 1;
			__IO uint32_t         : 3;
			__IO uint32_t PWR_DWN : 1;		   //1 Power-down
			__IO uint32_t MOS_DIS : 1;			
			__IO uint32_t         :26;
		} OSC_DIS;
	};
} SYS_T;


typedef struct {
	union {
		__IO uint32_t u32SWD_SEL;
		struct {
			__IO uint32_t EN : 1;		//0 A1/A2������PORT_SEL����		1 A1/A2���ӵ�SWD���ܣ��ֱ�ΪSWDIO/SWCLK����
			__IO uint32_t    :31;
		} SWD_EN;
	};
	
	__I  uint32_t RESERVE0[19];
	
    union {
        __IO uint32_t u32PORTA_SEL; 
        struct {
            __IO uint32_t PA00: 2;     //00 GPIO       01 TMRSE0_IN     10/11 
            __IO uint32_t PA01: 2;     //00 GPIO       01 TMRSE0_OUT    10/11 
            __IO uint32_t PA02: 2;     //00 GPIO       01 TMRSE1_IN     10/11 
            __IO uint32_t PA03: 2;     //00 GPIO       01 		        10/11
            __IO uint32_t PA04: 2;     //00 GPIO       01 		        10/11
            __IO uint32_t PA05: 2;     //00 GPIO       01 		        10/11
            __IO uint32_t PA06: 2;     //00 GPIO       01      			10/11
            __IO uint32_t PA07: 2;     //00 GPIO       01 TMRSE1OUT     10/11
			__IO uint32_t PA08: 2;     //00 GPIO       01               10/11
			__IO uint32_t PA09: 2;     //00 GPIO       01               10/11
            __IO uint32_t 	  :12;
        } PORTA_SEL;
    };

	__I  uint32_t RESERVE1[3];

    union {								//��������ģʽ�л��������ʱ���ܹ������ã���ʱʱ�������96uS
        __IO uint32_t u32PORTA_PULLU; 
        struct {
            __IO uint32_t PA00: 1;     //0 �����ر�        1 ��������
            __IO uint32_t PA01: 1;
            __IO uint32_t PA02: 1;
            __IO uint32_t PA03: 1;
            __IO uint32_t PA04: 1;
            __IO uint32_t PA05: 1;
            __IO uint32_t PA06: 1;
            __IO uint32_t PA07: 1;
			__IO uint32_t PA08: 1;
			__IO uint32_t PA09: 1;
            __I  uint32_t 	  :22;
        } PORTA_PULLU;  
    };
    
	__I  uint32_t RESERVE2[19];

    union {
		__IO uint32_t u32PORTA_INDIS;
		struct {
			__IO uint32_t PA00: 1;     //0 GPIOA.0���빦�ܲ���ֹ	1 GPIOA.0���빦�ܽ�ֹ���Խ��͹���
            __IO uint32_t PA01: 1;
            __IO uint32_t PA02: 1;
            __IO uint32_t PA03: 1;
            __IO uint32_t PA04: 1;
            __IO uint32_t PA05: 1;
            __IO uint32_t PA06: 1;
            __IO uint32_t PA07: 1;
			__IO uint32_t PA08: 1;
			__IO uint32_t PA09: 1;
            __I  uint32_t 	  :22;
		} PORTA_INDIS;
	};		
} PORT_T;

typedef struct 
{
	union {
		__IO uint32_t u32DAT;
		struct {
			__IO uint32_t DAT_0  : 1;
			__IO uint32_t DAT_1  : 1;
			__IO uint32_t DAT_2  : 1;
			__IO uint32_t DAT_3  : 1;
			__IO uint32_t DAT_4  : 1;
			__IO uint32_t DAT_5  : 1;
			__IO uint32_t DAT_6  : 1;
			__IO uint32_t DAT_7  : 1;
			__IO uint32_t DAT_8  : 1;
			__IO uint32_t DAT_9  : 1;
			__IO uint32_t 	     :22;
		} DAT;
	};

	union {
		__IO uint32_t u32DIR;
		struct {
			__IO uint32_t DIR_0  : 1;
			__IO uint32_t DIR_1  : 1;
			__IO uint32_t DIR_2  : 1;
			__IO uint32_t DIR_3  : 1;
			__IO uint32_t DIR_4  : 1;
			__IO uint32_t DIR_5  : 1;
			__IO uint32_t DIR_6  : 1;
			__IO uint32_t DIR_7  : 1;
			__IO uint32_t DIR_8  : 1;
			__IO uint32_t DIR_9  : 1;
			__IO uint32_t 	     :22;
		} DIR;
	};
} GPIO_T;

typedef struct
{
	union {
		__I  uint32_t u32EXT;
		struct {
			__I  uint32_t EXT0  : 1;
			__I  uint32_t EXT1  : 1;
			__I  uint32_t EXT2  : 1;
			__I  uint32_t EXT3  : 1;
			__I  uint32_t EXT4  : 1;
			__I  uint32_t EXT5  : 1;
			__I  uint32_t EXT6  : 1;
			__I  uint32_t EXT7  : 1;
			__I  uint32_t EXT8  : 1;
			__I  uint32_t EXT9  : 1;
			__I  uint32_t 	    :22;
		} EXT;
	};
} GPIO_EXT_T;

typedef struct 
{
	union {
		__IO uint32_t u32CPEN;
		struct {
			__I  uint32_t		 : 4;
			__IO uint32_t EN 	 : 1;
			__I  uint32_t 	     :27; 
		} CPEN;
	};

	union {
		__IO uint32_t u32CPLEVEL;
		struct {
			__IO uint32_t LEVEL   : 8;
			__I  uint32_t 	      :24; 
		} CPLEVEL;
	};

	__I  uint32_t RESERVE[2];

	union {
		__IO uint32_t u32CPOUTPUT;		
		struct {
			__O  uint32_t CPOUTRAW : 1;
			__O  uint32_t CPOUTMOS : 1;
			__O  uint32_t CPOUTCEL : 1;
			__I  uint32_t 	       :29; 
		} CPOUTPUT;
	};
	
	__I  uint32_t RESERVE1[3];

	union {
		__IO uint32_t u32CPMOD;			
		struct {
			__IO uint32_t WORKSMOD : 1;
			__I  uint32_t 	        :31; 
		} CPMOD;
	};
	
	union {
		__IO uint32_t u32CPCTL;
		struct {
			__I  uint32_t           : 1;
			__IO uint32_t MOS_LEVEL : 1;
			__IO uint32_t CEL_LEVEL : 1;
			__IO uint32_t A7M_LEVEL : 1;
			__IO uint32_t A7C_LEVEL : 1;
			__I  uint32_t           : 3;
			__IO uint32_t MOS_DELAY : 6;
			__IO uint32_t           : 2;
			__IO uint32_t CEL_DELAY : 6;
			__I  uint32_t 	        :10; 
		} CPCTL;
	};

	union {
		__IO uint32_t u32CLRLOCK;
		struct {
			__IO uint32_t CLR 	   : 1;
			__I  uint32_t 		   :31; 
		} CLRLOCK;
	};

	union {
		__IO uint32_t u32LOCKCON;
		struct {
			__IO uint32_t LOCKEN 	: 1;
			__IO uint32_t LEVELCON 	: 1;
			__IO uint32_t CLRCON 	: 1;
			__O  uint32_t LOCKSTAT 	: 1;
			__I  uint32_t 		    :28; 
		} LOCKCON;
	};

	__I  uint32_t RESERVE3[4];

	union {
		__IO uint32_t u32CPINTCON;
		struct {
			__IO uint32_t INTMASKn 	 : 1;
			__I  uint32_t            : 1;
			__IO uint32_t INTUP 	 : 1;
			__IO uint32_t INTDOWN  	 : 1;
			__IO uint32_t MOSINTMASKn : 1;
			__IO uint32_t MOSINTEN   : 1;
			__IO uint32_t MOSINTUP   : 1;
			__IO uint32_t MOSINTDOWN : 1;
			__I  uint32_t 		  	 :24; 
		} CPINTCON;
	};

	union {
		__IO uint32_t u32CPINTRAW;
		struct {
			__O  uint32_t CPRAW 	: 1;
			__O  uint32_t CPMOS     : 1;
			__O  uint32_t CPCEL 	: 1;
			__I  uint32_t 		  	:29; 
		} CPINTRAW;
	};

	union {
		__IO uint32_t u32CPINT;
		struct {
			__O  uint32_t CPRAW 	: 1;
			__O  uint32_t CPMOS     : 1;
			__O  uint32_t CPCEL 	: 1;
			__I  uint32_t 		  	:29; 
		} CPINTINT;
	};

	union {
		__IO uint32_t u32CPINTOF;		  //overflow
		struct {
			__O  uint32_t CPRAW 	: 1;
			__O  uint32_t CPMOS     : 1;
			__O  uint32_t CPCEL 	: 1;
			__I  uint32_t 		  	:29; 
		} CPINTOF;
	};

	union {
		__IO uint32_t u32CPINTCLR;
		struct {
			__I  uint32_t CPRAW 	: 1;
			__I  uint32_t CPMOS     : 1;
			__I  uint32_t CPCEL 	: 1;
			__I  uint32_t 		  	:29; 
		} CPINTCLR;
	};
} CP_T;

typedef struct 
{
	union {
		__IO uint32_t u32INTEN;
		struct {
			__IO uint32_t INTEN0 : 1;
			__IO uint32_t INTEN1 : 1;
			__IO uint32_t INTEN2 : 1;
			__IO uint32_t INTEN3 : 1;
			__IO uint32_t INTEN4 : 1;
			__IO uint32_t INTEN5 : 1;
			__IO uint32_t INTEN6 : 1;
			__IO uint32_t INTEN7 : 1;
			__IO uint32_t INTEN8 : 1;
			__IO uint32_t INTEN9 : 1;
			__I  uint32_t 	     :22; 
		} INTEN;
	};

	union {
		__IO uint32_t u32INTMSK;
		struct {
			__IO uint32_t INTMSK0 : 1;
			__IO uint32_t INTMSK1 : 1;
			__IO uint32_t INTMSK2 : 1;
			__IO uint32_t INTMSK3 : 1;
			__IO uint32_t INTMSK4 : 1;
			__IO uint32_t INTMSK5 : 1;
			__IO uint32_t INTMSK6 : 1;
			__IO uint32_t INTMSK7 : 1;
			__IO uint32_t INTMSK8 : 1;
			__IO uint32_t INTMSK9 : 1;
			__I  uint32_t 	      :22; 
		} INTMSK;
	};

	union {
		__IO uint32_t u32INTMODE;			//0 ��ƽ����	1 ���ش���
		struct {
			__IO uint32_t INTMODE0 : 1;
			__IO uint32_t INTMODE1 : 1;
			__IO uint32_t INTMODE2 : 1;
			__IO uint32_t INTMODE3 : 1;
			__IO uint32_t INTMODE4 : 1;
			__IO uint32_t INTMODE5 : 1;
			__IO uint32_t INTMODE6 : 1;
			__IO uint32_t INTMODE7 : 1;
			__IO uint32_t INTMODE8 : 1;
			__IO uint32_t INTMODE9 : 1;
			__I  uint32_t 	       :22; 
		} INTMODE;
	};
	
	union {
		__IO uint32_t u32INTLEVEL;			//0 �͵�ƽ/�½��ش���		1 �ߵ�ƽ/�����ش���
		struct {
			__IO uint32_t INTLEVEL0 : 1;
			__IO uint32_t INTLEVEL1 : 1;
			__IO uint32_t INTLEVEL2 : 1;
			__IO uint32_t INTLEVEL3 : 1;
			__IO uint32_t INTLEVEL4 : 1;
			__IO uint32_t INTLEVEL5 : 1;
			__IO uint32_t INTLEVEL6 : 1;
			__IO uint32_t INTLEVEL7 : 1;
			__IO uint32_t INTLEVEL8 : 1;
			__IO uint32_t INTLEVEL9 : 1;
			__I  uint32_t 	        :22; 
		} INTLEVEL;
	};
	
	union {
		__I  uint32_t u32INTSTAT;
		struct {
			__I  uint32_t INTSTAT0 : 1;
			__I  uint32_t INTSTAT1 : 1;
			__I  uint32_t INTSTAT2 : 1;
			__I  uint32_t INTSTAT3 : 1;
			__I  uint32_t INTSTAT4 : 1;
			__I  uint32_t INTSTAT5 : 1;
			__I  uint32_t INTSTAT6 : 1;
			__I  uint32_t INTSTAT7 : 1;
			__I  uint32_t INTSTAT8 : 1;
			__I  uint32_t INTSTAT9 : 1;
			__I  uint32_t 	       :22; 
		} INTSTAT;
	};

	union {
		__I  uint32_t u32INTRAWS;
		struct {
			__I  uint32_t INTRAWS0 : 1;
			__I  uint32_t INTRAWS1 : 1;
			__I  uint32_t INTRAWS2 : 1;
			__I  uint32_t INTRAWS3 : 1;
			__I  uint32_t INTRAWS4 : 1;
			__I  uint32_t INTRAWS5 : 1;
			__I  uint32_t INTRAWS6 : 1;
			__I  uint32_t INTRAWS7 : 1;
			__I  uint32_t INTRAWS8 : 1;
			__I  uint32_t INTRAWS9 : 1;
			__I  uint32_t 		   :22; 
		} INTRAWS;
	};

	__I  uint32_t RESERVE3;

	union {
		__IO uint32_t u32INTCLR;
		struct {
			__IO uint32_t INTCLR0 : 1;
			__IO uint32_t INTCLR1 : 1;
			__IO uint32_t INTCLR2 : 1;
			__IO uint32_t INTCLR3 : 1;
			__IO uint32_t INTCLR4 : 1;
			__IO uint32_t INTCLR5 : 1;
			__IO uint32_t INTCLR6 : 1;
			__IO uint32_t INTCLR7 : 1;
			__IO uint32_t INTCLR8 : 1;
			__IO uint32_t INTCLR9 : 1;
			__I  uint32_t 		  :22; 
		} INTCLR;
	};
} EXTI_T;


typedef struct{
	union {
		__IO uint32_t u32CR;
		struct {
			__IO uint32_t EN : 1;
			__IO uint32_t RMOD : 1;		//0 ��ʱ����λ		1 ��һ�γ�ʱ�����жϣ��������ڶ��γ�ʱʱ��û������жϣ���ϵͳ��λ
			__IO uint32_t RESERVE : 30;
		} CR;
	};
	
	union { 						
		__IO uint32_t u32TORR;
		struct {
			__IO uint32_t TOP : 4;		//��ʼֵ����WDTʹ��֮ǰд��ֵ��	2^(8+TOP)����24λ
			__IO uint32_t TOP_INIT : 4;	//��ʱ��Ҫ��װ��ֵ
			__IO uint32_t RESERVE  :24;
		} TORR; 
	};
	            		
	__I  uint32_t CCVR;					//24λ             		
	__O  uint32_t CRR;					//д0x76����������						
	__I  uint32_t STAT;             		
	__I  uint32_t ICLR;							
} WDT_T;


typedef struct {				
	union {
		__IO uint32_t u32CTRL;
		struct {
			__IO uint32_t ENA : 1;				//ģ��ʹ��
			__IO uint32_t : 3;
			__IO uint32_t WMOD : 2;				//00 ��ʱ��/������ģʽ	01 PWMģʽ	10 ����׽ģʽ		11 ռ�ձȲ�׽ģʽ
			__IO uint32_t : 2;
			__IO uint32_t OSCMOD : 1;			//0 �ڲ�	1 �ⲿ
			__IO uint32_t : 3;
			__IO uint32_t OUTMOD : 2;			//00 �����		01 ȡ��		10 �ø�		11 �õ�
												//��������ʱ������ʱ��/��������Ŀ��ֵ��PWMģʽ��ǰ����ǰ����/������ڽ�����
												//����׽ģʽ�ڶ����أ���ֹͣ�أ���ռ�ձȲ�׽ģʽ�������أ���ֹͣ�أ�
			__IO uint32_t : 2;
			__IO uint32_t EDGE_F : 1;			//0 ������		1 �½���
			__IO uint32_t : 7;
			__IO uint32_t VALSAVE: 1;			//��������ֵ
			__IO uint32_t : 3;
			__IO uint32_t SINGLE : 1;			//1 ����ģʽ 0 ѭ��ģʽ
		} CTRL;
	};
	
	__IO uint32_t TARVAL;						//Ŀ��ֵ����PWMģʽʱ����16λ�͸�16λ�ֱ�ΪPWM��ǰ�����ںͺ������
	
	__IO uint32_t CURVAL;						//��ǰֵ
	
	__IO uint32_t CAPVAL;						//������ģʽ��Ϊ����ֵ��ռ�ձȲ���ģʽ��Ϊ����ֵ
	__IO uint32_t CAPHALF;						//ռ�ձȲ���ģʽ��Ϊǰ������ֵ
	
	__IO uint32_t STATE_P;						//[0] ��ǰλ��״̬������PWMģʽ��ռ�ձ�ģʽ���жϵ�ǰ����ǰ�����ڻ��Ǻ������
	
	__I  uint32_t RESERVE[26];
	
	__IO uint32_t OUT_LVL;						//[0] ����������ϵĵ�ƽֵ
	
	union {
		__IO uint32_t u32INTCTRL;
		struct {
			__IO uint32_t ENA : 1;				//1 ʹ��	to observe sate, you should set this bit 1
			__IO uint32_t MASKn : 1;			//0 ����
												//�жϴ���ʱ������ʱ��/��������Ŀ��ֵ��PWMģʽ��ǰ����ǰ����/������ڽ�����
												//����׽ģʽ�ڶ����أ���ֹͣ�أ���ռ�ձȲ�׽ģʽ�������أ���ֹͣ�أ�
			__IO uint32_t : 30;
		} INTCTRL;
	};
	
	__IO uint32_t INTRSTAT;						//[0] �ж�ԭʼ״̬
	
	__IO uint32_t INTSTAT;						//[0] �ж�״̬
	
	__IO uint32_t INTOFLAG;						//[0] interrupt overflow flag �ж������־
} TMRSE_T;



/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
#define SRAM_BASE				((	uint32_t) 0x20000000)
#define PERIPH_BASE				((  uint32_t) 0x40000000)
#define SYSCTL_BASE				((	uint32_t) 0x400F0000)

#define PORT_BASE               (PERIPH_BASE	+ 0x0030) /* ��0x0000�޸�,��2014.04.30 */
#define CP_BASE               	(PERIPH_BASE	+ 0x0800)
#define GPIO_BASE				(PERIPH_BASE	+ 0x1000)
#define TMRSE0_BASE				(PERIPH_BASE	+ 0x2000)
#define TMRSE1_BASE				(PERIPH_BASE	+ 0x2400)
#define WDT_BASE				(PERIPH_BASE   	+0x19000)

#define GPIOA_BASE				(GPIO_BASE		+ 0x0000)
#define GPIO_EXTA_BASE			(GPIO_BASE		+ 0x0050)

#define EXTIA_BASE				(GPIO_BASE		+ 0x0030)


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define SYS							((SYS_T *)  SYSCTL_BASE)

#define PORT                        ((PORT_T *) PORT_BASE  )

#define GPIOA				  	  	((GPIO_T *) GPIOA_BASE )
#define GPIO_EXTA					((GPIO_EXT_T *) GPIO_EXTA_BASE)
									
#define EXTIA						((EXTI_T *) EXTIA_BASE )

#define TMRSE0						((TMRSE_T*) TMRSE0_BASE)
#define TMRSE1						((TMRSE_T*) TMRSE1_BASE)

#define CP							((CP_T  *) CP_BASE   )

#define WDT							((WDT_T  *) WDT_BASE   )


#define __STOP()  SYS->u32OSC_DIS = 0x30

#endif //__SWM050_H__
