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

#ifndef COMMON_H
#define COMMON_H


#include "system.h"


#define BIT(n)  ((guint32)(1UL << (n)))



static inline xmlNodePtr
xml_get_element_by_name(xmlNodePtr current, const xmlChar *name)
{
    for (; current; current = current->next)
        if (current->type == XML_ELEMENT_NODE &&
                strcmp(current->name, name) == 0)
            return current;

    return NULL;
}



#if defined(G_HAVE_ISO_VARARGS)

# define log_info(...) g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, __VA_ARGS__)
# ifdef DEBUG
#  define log_debug(...) g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, __VA_ARGS__)
# else
#  define log_debug(...) ((void) 0)
# endif

#elif defined(G_HAVE_GNUC_VARARGS)

# define log_info(args...) g_log(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, args)
# ifdef DEBUG
#  define log_debug(args...) g_log(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, args)
# else
#  define log_debug(args...) ((void) 0)
# endif

#else           /* user need to get a better compiler */

static void log_info(const gchar *format, ...) {
  va_list args;
  va_start(args, format);
  g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_INFO, format, args);
  va_end(args);
}
static void log_debug(const gchar *format, ...) {
# ifdef DEBUG
  va_list args;
  va_start(args, format);
  g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, args);
  va_end(args);
# endif
}

#endif



#endif	/* COMMON_H */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
