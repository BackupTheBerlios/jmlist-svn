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

#ifndef STYLE_H
#define STYLE_H

#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


/**
 * Style output types.
 */
typedef
enum StyleOutput_enum {
    STYLE_OTHER,                        /**< Generic, other type. */
    STYLE_TEXT,                         /**< Text output. */
    STYLE_HTML,                         /**< HTML output. */
    STYLE_XHTML,                        /**< XHTML output. */
    STYLE_XML                           /**< XML output. */
}
StyleOutput;


typedef
struct StyleParam_struct {
    gchar *             name;
    gchar *             short_descr;
    gchar *             descr;
    gchar *             value;
}
StyleParam;


typedef
struct Style_struct {
    gchar *             name;
    gchar *             title;
    gchar *             version;
    gchar *             author;
    gchar *             descr;
    StyleOutput         output;
    GList *             params;

    xsltStylesheetPtr stylesheet;
}
Style;


extern Style *          style_load(const gchar *filename, gboolean load_style);
extern void             style_destroy(Style *style);
extern struct poptOption *
                        style_get_popt_table(Style *style);
extern Style *          style_find_by_name(const gchar *name);
extern xmlDocPtr        style_apply(Style *style, xmlDocPtr doc);
extern void             style_list(void);


#endif	/* STYLE_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
