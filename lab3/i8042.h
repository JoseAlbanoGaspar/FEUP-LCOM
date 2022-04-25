#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

//#define BIT(n) (0x01 << (n)) /**< @brief Sets bit in position n as 1 */ -> já está definido pelo Minix

#define KBC_IRQ 1         /**< @brief Keyboard IRQ line */
#define OUT_BUF 0x60      /**< @brief Keyboard output buffer port */
#define CMD_ARGS_REG 0x60 /**< @brief Keyboard command arguments register */
#define STATUS_REG 0x64   /**< @brief Keyboard status register */
#define CMD_REG 0x64      /**< @brief Keyboard commands register */

#define DELAY_US 20000 /**< @brief Delay until the timer responds to a command */

//Important keys
#define ESC_KEY 0x81            /**< @brief Esc key breakcode */
#define FIRST_OF_TWO_BYTES 0xE000 /**< @brief First byte of two byte scan code */

//KBC Status register
#define KBC_PARE BIT(7)  /**< @brief Status register parity error */ //invalid data
#define KBC_TIMEO BIT(6) /**< @brief Status register timeout error */ //invalid data
#define KBC_AUXB BIT(5)  /**< @brief Status register mouse data */
#define KBC_KEYL BIT(4)  /**< @brief Status register keylock/inhibit flag */ // =0 if keyboard is inhibited
#define KBC_CD BIT(3)    /**< @brief Status register command/A2 input line */ //not relevant for this lab
#define KBC_SYSF BIT(2)  /**< @brief Status register system flag */ //not relevant for this lab
#define KBC_INPB BIT(1)  /**< @brief Status register input buffer flag */ //if is full, don't write commands or arguments
#define KBC_OUTB BIT(0)  /**< @brief Status register output buffer flag*/ //if is full, there is data available for reading

//KBC Commands
#define READ_CMD_B 0x20   /**< @brief Returns command byte */ //returns the command byte
#define WRITE_CMD_B 0x60  /**< @brief Writes command byte (must pass command byte as argument to port 0x60) */
#define CHECK_KBC 0xAA    /**< @brief Check KBC (Self-test): returns 0x55 if OK, returns 0xFC if error */
#define CHECK_KB_IFC 0xAB /**< @brief Check Keyboard interface : returns 0 if OK */
#define DISABLE_KBD 0xAD  /**< @brief Disables Keyboard interface */
#define ENABLE_KBS 0xAE   /**< @brief Enables Keyboard interface */

//Command Byte bits
#define ENABLE_KBD_IR BIT(0) /**< @brief Enables Keyboard interrupts on OBF */

#endif

