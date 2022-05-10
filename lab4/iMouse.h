//
// Created by barba on 10/05/2022.
//

#ifndef LAB4_IMOUSE_H
#define LAB4_IMOUSE_H

/*Mouse Macros */

#define MOUSE_IRQ 12

/* Parse the mouse packets */
#define MOUSE_LB_PRESSED BIT(0)
#define MOUSE_RB_PRESSED BIT(1)
#define MOUSE_MB_PRESSED BIT(2)
#define MOUSE_BIT_ID BIT(3) // este bit é sempre 1
#define MOUSE_X_DELTA_SIGN BIT(4)
#define MOUSE_Y_DELTA_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)

/* PS/2 Mouse commands */
#define MOUSE_CMD 0xD4 //Write Byte to Mouse
#define MOUSE_ENABLE_CMD 0xF4 //Enable Data Reporting, in stream mode only
#define MOUSE_DISABLE_CMD 0xF5 //Disable Data Reporting

/* PS/2 Mouse commands - responses */
#define MOUSE_ACK_ERROR 0xFC //error - second consecutive invalid byte
#define MOUSE_NACK 0xFE //Resend, for communication errors
#define MOUSE_STREAM 0xEA //Set Stream Mode
#define MOUSE_READ 0xEB //Read Data - When in remote mode, request the packet.

/* Mouse status register */

#define MOUSE_PARE BIT(7)  /**< @brief Status register parity error */
#define MOUSE_TIMEO BIT(6) /**< @brief Status register timeout error */
#define MOUSE_AUXB BIT(5)  /**< @brief Status register mouse data */
#define MOUSE_KEYL BIT(4)  /**< @brief Status register keylock/inhibit flag */
#define MOUSE_CD BIT(3)    /**< @brief Status register command/A2 input line */
#define MOUSE_SYSF BIT(2)  /**< @brief Status register system flag */
#define MOUSE_INPB BIT(1)  /**< @brief Status register input buffer flag */
#define MOUSE_OUTB BIT(0)  /**< @brief Status register output buffer flag*/

/* Mouse buffer - PORTS (same as kbd buffer)*/
#define MOUSE_STATUS_REG 0x64
#define MOUSE_OUT_BUF 0x60
#define MOUSE_IRQ 12

#endif // LAB4_IMOUSE_H
