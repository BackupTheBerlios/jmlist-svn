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

#include "common.h"
#include "file-mpeg-id3v2.h"

#include <id3.h>


typedef
struct ID3v2Header_struct {
    gchar  id[3];                       /* ID3v2 file identifier */
    guchar version[2];                  /* version */
    guchar flags;                       /* flags */
    guchar size[4];                     /* size */
}
ID3v2Header;


static const
struct {
    const gchar *name;                  /* field name */
    ID3_FrameID frame;                  /* field identifier */
    ID3_FieldID field;                  /* field type */
}
id3v2_fields[] = {
    { "title",          ID3FID_TITLE,           ID3FN_TEXT              },
    { "artist",         ID3FID_LEADARTIST,      ID3FN_TEXT              },
    { "album",          ID3FID_ALBUM,           ID3FN_TEXT              },
    { "year",           ID3FID_YEAR,            ID3FN_TEXT              },
    { "track",          ID3FID_TRACKNUM,        ID3FN_TEXT              },
    { "genre",          ID3FID_CONTENTTYPE,     ID3FN_TEXT              },
    { "composer",       ID3FID_COMPOSER,        ID3FN_TEXT              },
    { "orig-artist",    ID3FID_ORIGARTIST,      ID3FN_TEXT              },
    { "copyright",      ID3FID_COPYRIGHT,       ID3FN_TEXT              },
    { "url",            ID3FID_WWWUSER,         ID3FN_URL               },
    { "encoded-by",     ID3FID_ENCODEDBY,       ID3FN_TEXT              },
    { "comment",        ID3FID_COMMENT,         ID3FN_TEXT              },
    { NULL }
};


/* check_header() {{{ */

/**
 * Checks if this is a well-formed ID3v2 tag header.
 *
 * \param header The header to check.
 * \returns The size of the tag.
 */
static inline glong
check_header(ID3v2Header header)
{
    if (header.id[0] != 'I' || header.id[1] != 'D' || header.id[2] != '3')
        return 0;

    if (header.version[0] == 0xFF || header.version[1] == 0xFF)
        return 0;

    if ((header.size[0] | header.size[1] | header.size[2] | header.size[3]) & 0x80)
        return 0;

    return (header.size[0] << 21) | (header.size[1] << 14) |
        (header.size[2] << 7) | header.size[3];
}

/* }}} */

/* parse_genre() {{{ */
/**
 * Parse the genre of an ID3v2 tag.
 *
 * \param str String stored on tag.
 * \returns Human-readable genre specification.
 */
static inline const gchar *
parse_genre(const gchar *str)
{
    const gchar *s;
    gint genre_id;

    if (str[0] == '(') {
        genre_id = strtol(str+1, (gchar**)&s, 10);

        if (s != NULL && *s == ')') {
            if (*++s != '\0')
                return s;
            else if (genre_id < ID3_NR_OF_V1_GENRES)
                return ID3_v1_genre_description[genre_id];
            else
                return str;
        }
        else {
            return str;
        }
    }
    else {
        return str;
    }
}

/* }}} */

/* id3v2_parse() {{{ */
/**
 * Parse an ID3v2 tag.
 * After parsing, the file pointer is left after the end of the tag, where
 * the actual audio data should start.
 *
 * \param fp File which will be parsed.
 * \returns A XML tree representing the tag data.
 */
xmlNodePtr
id3v2_parse(FILE *fp)
{
    ID3v2Header header;
    ID3Tag *tag;

    glong header_size;
    xmlNodePtr tag_node = NULL;
    guchar *buffer;

    rewind(fp);

    if (fread(&header, sizeof header, 1, fp) != 1) {
        rewind(fp);
        return NULL;
    }

    if ((header_size = check_header(header)) == 0) {
        rewind(fp);
        return NULL;
    }

    /* retrieve body of tag */
    buffer = g_malloc(header_size);

    if (fread(buffer, header_size, 1, fp) != 1) {
        g_free(buffer);
        rewind(fp);
        return NULL;
    }

    /* parse tag with ID3Tag */
    tag = ID3Tag_New();

    if (ID3Tag_Parse(tag, (guchar *) &header, buffer) == ID3E_NoError) {
        ID3Frame *frame;
        ID3Field *field;
        gchar tmp[1024];
        int i;

        tag_node = xmlNewNode(NULL, "tag");
        xmlNewProp(tag_node, "format", "ID3v2");

        g_snprintf(tmp, sizeof tmp, "2.%d.%d", header.version[0], header.version[1]);
        xmlNewProp(tag_node, "version", tmp);

        for (i = 0; id3v2_fields[i].name != NULL; i++) {
            if ((frame = ID3Tag_FindFrameWithID(tag, id3v2_fields[i].frame)) != NULL) {
                if ((field = ID3Frame_GetField(frame, id3v2_fields[i].field)) != NULL) {
                    if (ID3Field_GetASCII(field, tmp, sizeof tmp)) {
                        if (id3v2_fields[i].frame == ID3FID_CONTENTTYPE) {
                            xmlNewTextChild(tag_node, NULL,
                                            id3v2_fields[i].name,
                                            parse_genre(tmp));
                        }
                        else {
                            if (g_utf8_validate(tmp, -1, NULL)) {
                                xmlNewTextChild(tag_node, NULL,
                                                id3v2_fields[i].name, tmp);
                            }
                            else {
                                gchar *r = g_convert(tmp, -1, "UTF-8",
                                                     "ISO8859-1", NULL,
                                                     NULL, NULL);

                                if (r != NULL) {
                                    xmlNewTextChild(tag_node, NULL,
                                                    id3v2_fields[i].name, r);
                                    g_free(r);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    g_free(buffer);
    return tag_node;
}


/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
