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

/** \file **/

#include "common.h"
#include "file-mpeg-id3v1.h"

#include <id3.h>


typedef
struct ID3v1Tag_struct {
    gchar       tag[3];
    gchar       title[30];
    gchar       artist[30];
    gchar       album[30];
    gchar       year[4];
    gchar       comment[30];
    guchar      genre;
}
ID3v1Tag;


/* parse_field() {{{ */
/**
 * Parse data from an ID3v1 field.
 * Data is converted to UTF-8 if it is not already.
 *
 * \param field Tag field text.
 * \param sz Size of this field.
 * \returns A newly allocated string containing the text from this field in
 *      UTF-8 encoding.
 */

static inline gchar *
parse_field(gchar *field, gsize sz)
{
    gchar buffer[256] = "";
    gchar *p, *r;

    /* fix blank space at the end of the field */
    for (p = field + sz - 1; p >= field && g_ascii_isspace(*p); p--)
        *p = '\0';

    strncpy(buffer, field, sz);

    /* check for empty field */
    if (buffer[0] == '\0')
        return NULL;

    if (g_utf8_validate(buffer, -1, NULL)) {
        /* valid utf-8 string */
        r = g_strchomp(g_strdup(buffer));
    }
    else {
        /* try to convert from foreign charset */
        /* TODO: the list of charsets to try should be customizable */
        r = g_convert(buffer, -1, "UTF-8", "ISO8859-1", NULL, NULL, NULL);

        if (r == NULL)
            return NULL;

        r = g_strchomp(r);
    }

    if (r[0] == '\0') {
        g_free(r);
        return NULL;
    }

    return r;
}

/* }}} */

/* id3v1_parse() {{{ */
/**
 * Parse an ID3v1 tag.
 *
 * \param fp File which will be parsed.
 * \returns A XML tree representing the tag data.
 */

xmlNodePtr
id3v1_parse(FILE *fp)
{
    xmlNodePtr tag_node;
    ID3v1Tag tag;
    gchar *tmp;

    rewind(fp);

    if (fseek(fp, -ID3V1_TAG_LENGTH, SEEK_END) != 0 ||
            fread(&tag, sizeof tag, 1, fp) != 1 ||
            strncmp(tag.tag, "TAG", 3) != 0)
        return NULL;

    tag_node = xmlNewNode(NULL, "tag");
    xmlNewProp(tag_node, "format", "ID3");

    if ((tmp = parse_field(tag.title, sizeof tag.title)) != NULL) {
        xmlNewTextChild(tag_node, NULL, "title", tmp);
        g_free(tmp);
    }

    if ((tmp = parse_field(tag.artist, sizeof tag.artist)) != NULL) {
        xmlNewTextChild(tag_node, NULL, "artist", tmp);
        g_free(tmp);
    }

    if ((tmp = parse_field(tag.album, sizeof tag.album)) != NULL) {
        xmlNewTextChild(tag_node, NULL, "album", tmp);
        g_free(tmp);
    }

    if ((tmp = parse_field(tag.year, sizeof tag.year)) != NULL) {
        xmlNewTextChild(tag_node, NULL, "year", tmp);
        g_free(tmp);
    }

    if (tag.genre < ID3_NR_OF_V1_GENRES)
        xmlNewTextChild(tag_node, NULL, "genre",
                        ID3_v1_genre_description[tag.genre]);

    if ((tmp = parse_field(tag.comment, sizeof tag.comment)) != NULL) {
        xmlNewTextChild(tag_node, NULL, "comment", tmp);
        g_free(tmp);
    }

    return tag_node;
}

/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
