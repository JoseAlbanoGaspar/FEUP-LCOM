#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
<<<<<<< HEAD
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

//todos os timer usam o mesmo "clock signal", com frequência = 1193181 Hz
#define TIMER_FREQ 1193181 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */




=======
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

>>>>>>> e859c9b01cc41cb400a0531ff5fc07eae080891b
/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

<<<<<<< HEAD
//acho que SPEAKER_CTRL não é usado aqui
#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */




/* Timer control word*/

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */ //ambos os bits 6 e 7 a 0
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */ //bit 6 a 1
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */ //bit 7 a 1

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */ //bit 4 a 1
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */ //bit 5 a 1
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */ //ambos os bits 5 e 4 a 1

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */ //ambos os bits 2 e 1 a 1
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */ //bit 2 a 1

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */ //bit 1 a 1
#define TIMER_BIN 0x00 /**< @brief Count in binary */ //tudo a 0




/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5) //active low
#define TIMER_RB_STATUS_ BIT(4) //active low
#define TIMER_RB_SEL(n)  BIT((n) + 1) //escolha dos timers através da tabela do RB command
//exemplo: timer 0 tem de ter o bit 1 a 1, logo TIMER_RB_SEL(0) -> bit(0+1) = 1 = timer 0 selecionado

#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */ //ambos os bits 6 e 7 a 1

=======
#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)
>>>>>>> e859c9b01cc41cb400a0531ff5fc07eae080891b

/**@}*/

#endif /* _LCOM_I8254_H */
