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

#ifndef SYSTEM_H
#define SYSTEM_H


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


/* System Dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


/* GLib Dependencies */
#include <glib.h>


/* LibXML2 Dependencies */
#include <libxml/parser.h>
#include <libxml/tree.h>

#if !defined(LIBXML_TREE_ENABLED) || !defined(LIBXML_OUTPUT_ENABLED)
#  error "Tree support not compiled in."
#endif


#endif	/* SYSTEM_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
