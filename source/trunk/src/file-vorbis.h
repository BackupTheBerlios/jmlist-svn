/******************************************************************************
 * jMList                                                                     *
 * Media file listing generator                                               *
 * http://juliano.info/projects/jMList/                                       *
 ******************************************************************************
 *   Author: Juliano F. Ravasi <dev at juliano info>                          *
 * Language: ANSI C (ISO/IEC 9899:1990)                                       *
 *  License: GNU General Public License                                       *
 ******************************************************************************
 * Copyright (C) 2005 Juliano F. Ravasi                                       *
 ******************************************************************************/

/** \file **/

#ifndef FILE_VORBIS_H
#define FILE_VORBIS_H

#include "file.h"


extern void             vorbis_init(void);
extern gboolean         vorbis_proc(xmlNodePtr file_node, FILE **fp, guint32 *length);


#endif	/* FILE_VORBIS_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
