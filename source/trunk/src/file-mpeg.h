/******************************************************************************
 * jMList                                                                     *
 * Media file listing generator                                               *
 * http://www.juliano.info/projects/jMList/                                   *
 ******************************************************************************
 *   Author: Juliano Ravasi Ferraz <jferraz at users sourceforge net>         *
 * Language: ANSI C (ISO/IEC 9899:1990)                                       *
 *  License: GNU General Public License                                       *
 ******************************************************************************
 * Copyright (C) 2005 Juliano Ravasi Ferraz                                   *
 ******************************************************************************/

#ifndef FILE_MPEG_H
#define FILE_MPEG_H

#include "file.h"

void mpeg_init(void);

gboolean mpeg_proc(xmlNodePtr file_node, FILE *fp);

#endif	/* FILE_MPEG_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
