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

#ifndef STYLE_H
#define STYLE_H

#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


typedef
enum StyleOutput_enum {
    STYLE_OTHER,
    STYLE_TEXT,
    STYLE_HTML,
    STYLE_XHTML,
    STYLE_XML
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
