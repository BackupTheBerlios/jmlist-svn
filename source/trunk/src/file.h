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

#ifndef FILE_H
#define FILE_H

#include "directory.h"


typedef
enum FileType_enum {
    FILE_TYPE_AUDIO,
    FILE_TYPE_VIDEO
}
FileType;


typedef gboolean FileParserFunc(xmlNodePtr file_node, FILE **fp, guint32 *length);


extern xmlNodePtr       file_proc(const gchar *real_path, const gchar *file_name, DirAccumData *accum);
extern gint             file_parser_register(const gchar *name, const gchar **extensions, FileType type, FileParserFunc *func);
extern void             file_parser_init_all(void);
extern const gchar *    filename_get_extension(const gchar *filename);
extern gchar *          filename_change_extension(const char *filename, const char *extension);


#endif	/* FILE_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
