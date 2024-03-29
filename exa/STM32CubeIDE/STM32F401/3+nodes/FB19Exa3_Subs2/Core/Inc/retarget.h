/**
 *  Filename:       retarget.h
 *  Platform(s):    STM32F401
 *  Project:
 *  Created:        May 26, 2023
 *  Description:    This file publishes some low-level C library and console
 *                  UART functions.
 *
 *  Notes:          This module is specific to the STM32CubeMX and STM32CubeIDE
 *                  development environment.
 *                  The 'dedicated console UART' (example: huart2) was
 *                  generated by STM32CubeMX and is defined in main.c.
 *
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#ifndef RETARGET_H_
#define RETARGET_H_

/**
 * Low-level function used by the standard I/O library \c stdio.h for reading
 * a character from the standard input, which is in this case the 'console
 * UART'.
 * The function blocks until the UART's Receive Register Not Empty flag has
 * been set by the UART (i.e. has received a character).
 */
int __io_getchar(void);

/**
 * Low-level function used by the standard I/O library \c stdio.h for writing
 * a character to the standard output, which is in this case the 'console
 * UART'.
 * The function blocks until the UART's Transmit Register Empty flag has been
 * set by the UART (i.e. the character could be copied into the UART's
 * Transmit register).
 */
int __io_putchar(int ch);

/**
 * Reads user input from the dedicated console UART and returns it as null-
 * terminated C-string.
 *
 * Wrapper for platform specific implementation.<br>
 * Assumes that the UART doesn't store more than 1 character in its input
 * buffer.<br>
 * If the buffer is too small to hold the user input string, the input
 * string is truncated to the size of the buffer - 1 character (for the
 * terminating '\0' character). The rest of the user input characters are
 * lost.
 * Expects the end-of-line character to be the 'newline' character
 * '\n' = 0x0A.<br>
 * The end-of-line character is NOT part of the returned string.<br>
 * Terminates the string inside the buffer pointed to by \c str with a '\0'
 * character.<br>
 * The length of the buffer shall be at least 2 characters: one for a
 * payload character from the UART, and one for the terminating '\0'
 * character.
 *
 * @param[out] *str points to the buffer that will hold the null-terminated
 * string when the function has returned. In case of an error, it won't be
 * changed.
 * @param[in] bufLen is the length of the buffer in characters. If it is < 2,
 * \c -1 is returned to indicate this error.
 * @return The length of the string excluding its '\0' character (as strlen()
 * would return it), or \c -1 if the buffer is less than 2 characters long.
 */
int readConsoleInput(char* str, int bufLen);

/**
 * Writes the null-terminated C-string as output to the dedicated console UART.
 *
 * Wrapper for platform specific implementation.<br>
 * @param[in] str points to the null terminated string that shall be written
 * to the console.
 * @return According to www.cplusplus.com on fputs():<br>
 * On success, a non-negative value is returned. On error, the function returns
 * \c EOF (typically \c -1) and sets the error indicator (\c ferror).
 */
int writeConsoleOutput(const char* str);

#endif /* RETARGET_H_ */
