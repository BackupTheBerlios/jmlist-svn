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

#include "common.h"
#include "file.h"
#include "file-mpeg.h"


typedef
struct FileParser_struct {
    const gchar *name;
    const gchar **extensions;
    FileType type;
    FileParserFunc *func;
}
FileParser;

GList *parser_list = NULL;

static FileParser * file_parser_get_by_extension(const gchar *extension);


/* get_file_extension(file_name) {{{ */
/**
 * Return a pointer to the extension part of an UTF-8 enconded filename.
 */
static inline G_CONST_RETURN gchar *
get_file_extension(const gchar *file_name)
{
    const char *p;

    if ((p = strrchr(file_name, '.')) == NULL)
        return NULL;                    /* file has no extension */
    else
        return p + 1;
}

/* }}} */

/* file(real_path, file_name) {{{ */
/**
 * Process a file, checking if it should be listed and retrieving its data in
 * XML format.
 */
xmlNodePtr
file_proc(const gchar *real_path,
          const gchar *file_name)
{
    gchar *file_name_utf8;
    const gchar *file_extension;
    FileParser *parser;
    xmlNodePtr file_node = NULL;

    /* get UTF-8 encoded filename and extension */
    file_name_utf8 = g_filename_to_utf8(file_name, -1, NULL, NULL, NULL);
    file_extension = get_file_extension(file_name_utf8);

    /* check if we support this file */
    if (file_extension == NULL || file_extension[0] == '\0') {
        /* extension-less file, we just ignore */
    }
    else if ((parser = file_parser_get_by_extension(file_extension)) != NULL) {
        /* we have a parser for this file -- good -- go ahead */
        FILE *fp;

        /* create file node */
        file_node = xmlNewNode(NULL, "file");
        xmlNewProp(file_node, "name", file_name_utf8);

        if ((fp = fopen(real_path, "rb")) != NULL) {
            glong file_size;
            gchar size_string[64];

            /* retrieve file size */
            /* @TODO support 64-bit file sizes */
            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);

            g_snprintf(size_string, sizeof size_string, "%ld", file_size);
            xmlNewProp(file_node, "size", size_string);

            /* parse (or at least try to) the file */
            rewind(fp);
            parser->func(file_node, fp);
        }
        else {                          /* fopen() returned NULL */
            xmlNodePtr comment = xmlNewComment("error opening file");
            xmlAddChild(file_node, comment);

            g_printerr("%s: %s\n", real_path, g_strerror(errno));
        }

    }

    g_free(file_name_utf8);             /* free UTF-8 converted filename */
    return file_node;
}

/* }}} */

/* file_parser_register(name, extensions, type, func) {{{ */
/**
 * Register a new file parser plugin.
 *
 * @TODO Use a hash map.
 */
gint
file_parser_register(const gchar *name,
                     const gchar **extensions,
                     FileType type,
                     FileParserFunc *func)
{
    FileParser *parser;

    parser = g_new(FileParser, 1);

    parser->name = name;
    parser->extensions = extensions;
    parser->type = type;
    parser->func = func;

    parser_list = g_list_append(parser_list, parser);
    return 0;
}

/* }}} */

/* file_parser_get_by_extension(extension) {{{ */
/**
 * Return the file parser associated to the given extension, if any.
 *
 * @TODO Use a hash map.
 */
static FileParser *
file_parser_get_by_extension(const gchar *extension)
{
    GList *node;
    int i;

    for (node = g_list_first(parser_list); node; node = g_list_next(node)) {
        FileParser *parser = (FileParser *) node->data;
        for (i = 0; parser->extensions[i]; i++) {
            if (g_strcasecmp(extension, parser->extensions[i]) == 0)
                return parser;
        }
    }

    return NULL;
}

/* }}} */

/* file_parser_init_all() {{{ */
/**
 * Initialize all file parsers.
 */
void
file_parser_init_all(void)
{
    mpeg_init();
}

/* }}} */

/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
