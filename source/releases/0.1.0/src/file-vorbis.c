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
#include "file-vorbis.h"

#include <vorbis/vorbisfile.h>


static const
struct {
    const gchar *name;                  /* field name */
    const gchar *field;                 /* field name for vorbis */
}
vc_fields[] = {
    { "title",          "TITLE"                                         },
    { "artist",         "ARTIST"                                        },
    { "performer",      "PERFORMER"                                     },
    { "album",          "ALBUM"                                         },
    { "year",           "DATE"                                          },
    { "track",          "TRACKNUMBER"                                   },
    { "genre",          "GENRE"                                         },
    { "composer",       "COMPOSER"                                      },
  /*{ "orig-artist",    "ORIGINAL-ARTIST"                               },*/
    { "copyright",      "COPYRIGHT"                                     },
    { "license",        "LICENSE"                                       },
  /*{ "url",            "URL"                                           },*/
  /*{ "encoded-by",     "ENCODED-BY"                                    },*/
    { "comment",        "DESCRIPTION"                                   },
    { "comment",        "COMMENT"               /* non-standard! */     },
    { NULL }
};


/* vorbis_init() {{{ */
/**
 * Initialize OGG Container parser.
 */

void
vorbis_init(void)
{
    static const gchar *module_name = "Vorbis Audio parser";
    static const gchar *module_extensions[] = { "ogg", NULL };

    file_parser_register(module_name, module_extensions, FILE_TYPE_AUDIO, vorbis_proc);
}

/* }}} */

/* vorbis_proc(file_node, fp) {{{ */
/**
 * Process an OGG Vorbis Audio file.
 *
 * \param file_node XML node where stream information should be inserted.
 * \param fp Pointer to an open file.
 * \returns TRUE if successful.
 */

gboolean
vorbis_proc(xmlNodePtr file_node,
            FILE *fp)
{
    OggVorbis_File file;
    int link, i;

    if (ov_open(fp, &file, NULL, 0) != 0)
        return FALSE;

    for (link = 0; link < file.links; link++) {
        xmlNodePtr audio_stream_node, tag_node;
        gchar tmp[64];

        vorbis_info *vi = ov_info(&file, link);
        vorbis_comment *vc = ov_comment(&file, link);

        /* add information about the audio stream */
        audio_stream_node = xmlNewChild(file_node, NULL, "audio-stream", NULL);
        xmlNewProp(audio_stream_node, "format", "vorbis");

        g_snprintf(tmp, sizeof tmp, "%d", vi->version);
        xmlNewProp(audio_stream_node, "version", tmp);

#if 0
        /* XXX: For some reason, this gives wrong results */
        g_snprintf(tmp, sizeof tmp, "%s%ld",
                   vi->bitrate_lower != vi->bitrate_upper ? "~" : "",
                   vi->bitrate_nominal / 1000);
#endif
        g_snprintf(tmp, sizeof tmp, "~%ld", ov_bitrate(&file, link) / 1000);
        xmlNewProp(audio_stream_node, "bitrate", tmp);

        g_snprintf(tmp, sizeof tmp, "%ld", vi->rate);
        xmlNewProp(audio_stream_node, "sample", tmp);

        g_snprintf(tmp, sizeof tmp, "%d", vi->channels);
        xmlNewProp(audio_stream_node, "channels", tmp);

        xmlNewProp(audio_stream_node, "mode",
                   vi->channels == 1 ? "mono" :
                   vi->channels == 2 ? "stereo" : "stereo+" );

        g_snprintf(tmp, sizeof tmp, "%d",
                   (int)(ov_time_total(&file, link) * 1000));
        xmlNewProp(audio_stream_node, "length", tmp);

        /* add information of vorbis comment (in other words, "tag") */
        tag_node = xmlNewChild(audio_stream_node, NULL, "tag", NULL);
        xmlNewProp(tag_node, "format", "vorbis");

        for (i = 0; vc_fields[i].name != NULL; i++) {
            int j = 0;
            gchar *str;

            while ((str = vorbis_comment_query(vc, (gchar *)vc_fields[i].field, j++)) != NULL)
                xmlNewTextChild(tag_node, NULL, vc_fields[i].name, str);
        }
    }

    ov_clear(&file);
    return FALSE;
}

/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
