/**
 * Feathery FTP-Server <https://sourceforge.net/projects/feathery>
 * Copyright (C) 2005-2010 Andreas Martin (andreas.martin@linuxmail.org)
 *
 * ftpTypes.h - global type  definitions
 *
 * Definitions of fixed size integers an helper macros.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FTPTYPES_H_
#define FTPTYPES_H_


 // if the compiler is c99 complient, we don't need to define our own types
#if __STDC_VERSION__ == 199901L || (defined(WIN32) && defined(_MSC_VER) && _MSC_VER >= 1600)
#	include <stdint.h>
#else

typedef signed char 	int8_t;
typedef unsigned char 	uint8_t;
typedef signed short 	int16_t;
typedef unsigned short 	uint16_t;
typedef signed int 		int32_t;
typedef unsigned int 	uint32_t;

#endif

typedef int socket_t;
typedef uint32_t ip_t;
typedef uint16_t port_t;

/**
 * @brief returns the number of elements of an array
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

 /**
  * @brief defines functions/variables local to a module
  */
#define LOCAL	static

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#if (defined WIN32) && !(defined snprintf)
#define snprintf _snprintf
#endif


#ifdef	__cplusplus
extern "C" {
#endif

	int VERBOSE_MODE_ENABLED;

	typedef ip_t(*ftpFindExternalFTPServerIpType)(ip_t clientIp);
	typedef void(*ftpAddUPNPPortForwardType)(int internalPort, int externalPort);
	typedef void(*ftpRemoveUPNPPortForwardType)(int internalPort, int externalPort);
	typedef int(*ftpIsValidClientType)(ip_t clientIp);
	typedef int(*ftpIsClientAllowedToGetFileType)(ip_t clientIp, const char *username, const char *filename);

	ftpFindExternalFTPServerIpType	ftpFindExternalFTPServerIp;
	ftpAddUPNPPortForwardType		ftpAddUPNPPortForward;
	ftpRemoveUPNPPortForwardType	ftpRemoveUPNPPortForward;
	ftpIsValidClientType            ftpIsValidClient;
	ftpIsClientAllowedToGetFileType ftpIsClientAllowedToGetFile;

#ifdef	__cplusplus
}
#endif

#endif
