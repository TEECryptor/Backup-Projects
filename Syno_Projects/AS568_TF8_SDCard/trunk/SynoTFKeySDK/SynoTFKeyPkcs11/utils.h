/* -*- c -*- */
/*
 * This file is part of GPKCS11. 
 * (c) 1999,2000 TC TrustCenter for Security in DataNetworks GmbH 
 *
 * GPKCS11 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *  
 * GPKCS11 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with GPKCS11; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA.  
 */
/*
 * RCSID:       $Id: utils.h,v 1.3 2000/03/08 09:59:08 lbe Exp $
 * Source:      $Source: /usr/cvsroot/pkcs11/libgpkcs11/utils.h,v $
 * Last Delta:  $Date: 2000/03/08 09:59:08 $ $Revision: 1.3 $ $Author: lbe $
 * State:	$State: Exp $ $Locker:  $
 * NAME:	utils.c
 * SYNOPSIS:	-
 * DESCRIPTION: -
 * FILES:	-
 * SEE/ALSO:	-
 * AUTHOR:	lin
 * BUGS: *	-
 * HISTORY:	$Log: utils.h,v $
 * HISTORY:	Revision 1.3  2000/03/08 09:59:08  lbe
 * HISTORY:	fix SIGBUS in cryptdb, improve readeability for C_FindObject log output
 * HISTORY:	
 * HISTORY:	Revision 1.2  2000/01/31 18:09:03  lbe
 * HISTORY:	lockdown prior to win_gdbm change
 * HISTORY:	
 * HISTORY:	Revision 1.1  1999/06/16 09:46:12  lbe
 * HISTORY:	reorder files
 * HISTORY:	
 * HISTORY:	Revision 1.3  1999/01/19 12:19:48  lbe
 * HISTORY:	first release lockdown
 * HISTORY:
 * HISTORY:	Revision 1.2  1999/01/18 16:30:09  lbe
 * HISTORY:	util cleanup and package build
 * HISTORY:
 * HISTORY:	Revision 1.1  1999/01/18 13:02:35  lbe
 * HISTORY:	swapped Berkeley DB for gdbm
 * HISTORY:
 */


/* TrustCenter Utilities */

#ifndef _UTILS_H
#define _UTILS_H 1

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(Win32) && !defined(BOOL)
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif
  

extern char achStrUpChar  [];

  /**
    Funktion : Liefert zu einem Klein- den zugeh�rigen Gro�buchstaben.
    Ergebnis : Gro�buchstabe.
    MT_LEVEL : safe
  */
#define StrUpChar(c)	((char) achStrUpChar [(unsigned char) (c)])

/** liest Werte aus einer Konfigurationdatei
    Aufbau:
     [Sektion]
      Name = Wert

   @param ConfigFilename Name der Konfigurationsdatei
   @param Section        Name des relevanten Abschnittes
   @param FieldName      Name des Feldes
   @param Value          (output) Wert
   @param MaxValueLen    maximale Laenge von Value
   @param NormalizeNames true: convert all string to upper case
   @return
    0: OK
    1 oder 2: ConfgiFilename nicht gefunden
    3: Section nicht gefunden
    4: FieldName nicht gefunden 

   @example
     TCU_GetProfileString("tccrypt.cfg", "email-listener", "req_no_id", buf, 255, 0);
*/
int TCU_GetProfileString( const char *ConfigFilename, const char *Section, const char *FieldName,
                          char *Value, int MaxValueLen, BOOL NormalizeNames );


/** wrapper for strtok_r. 
* Unter Solaris muss das Macro _REENTRANT defininiert sein.
* @param s1 list of token, to be NULL for subsequent calls
* @param s2 string of seperation characters
* @param lastst handle to remaining substring of tokens.
* @return token
*/
char *TC_strtok_r(char* s1, const char* s2, char** lasts);


#ifdef __cplusplus
}
#endif

#endif /* _UTILS_H */

