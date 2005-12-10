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

#ifndef DIRECTORY_H
#define DIRECTORY_H


typedef
struct DirAccumData_struct {
    guint64 size_in_bytes;
    guint32 length_in_ms;
}
DirAccumData;


extern xmlNodePtr       directory_proc(const gchar *real_path, const gchar *dir_name, DirAccumData *parent_accum);


#endif	/* DIRECTORY_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
