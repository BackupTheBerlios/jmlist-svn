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

/*
 * Based on the source code of XMMS <http://www.xmms.org/> and mpg321
 * <http://mpg321.sourceforge.net>, both are distributed under the GNU General
 * Public License.
 *
 * Also: <http://www.dv.co.yu/mpgscript/mpeghdr.htm>
 */

#include "common.h"
#include "file-mpeg.h"
#include "file-mpeg-id3v1.h"
#include "file-mpeg-id3v2.h"

/*
 * MPEG Header (32 bits)
 *
 *   28  24    20  16    12   8     4   0
 *    |   |     |   |     |   |     |   |
 * 11111111  111.....     |   |     |   |       - (21) sync word
 *           ...BB...     |   |     |   |       - (19) version
 *           .....CC.     |   |     |   |       - (17) layer
 *           .......D     |   |     |   |       - (16) protection
 *                     EEEE....     |   |       - (12) bitrate index
 *                     ....FF..     |   |       - (10) sampling rate index
 *                     ......G.     |   |       - ( 9) padding bit
 *                     .......H     |   |       - ( 8) private bit
 *                               II......       - ( 6) mode (stereo/mono)
 *                               ..JJ....       - ( 4) ext.mode (joint)
 *                               ....K...       - ( 3) copyright
 *                               .....L..       - ( 2) original
 *                               ......MM       - ( 0) (obsolete)
 */

typedef
enum MPEGVersion_enum {
    MPEG_V_1_0,
    MPEG_V_2_0,
    MPEG_V_2_5,

    MPEG_N_VERSIONS
}
MPEGVersion;


typedef
enum MPEGLayer_enum {
    MPEG_LAYER_1,
    MPEG_LAYER_2,
    MPEG_LAYER_3,

    MPEG_N_LAYERS
}
MPEGLayer;


typedef
enum MPEGMode_enum {
    MPEG_STEREO,
    MPEG_JOINT_STEREO,
    MPEG_DUAL_CHANNEL,
    MPEG_MONO,

    MPEG_N_MODES
}
MPEGMode;


typedef gint MPEGBitRate;
typedef gint MPEGSampling;


typedef
struct MPEGInfo_struct {
    MPEGVersion         version;
    MPEGLayer           layer;
    MPEGBitRate         bitrate;
    MPEGSampling        sampling;
    MPEGMode            mode;
    gboolean            protection;
    gboolean            padding;
    gboolean            private;
    gboolean            copyright;
    gboolean            original;
    guint               framesize;

    gboolean            vbr;
    guint               frames;
    guint               length;
}
MPEGInfo;


enum { MPEG_N_BITRATES = 16 };
enum { MPEG_N_SAMPLERATES = 3 };
enum { MPEG_MAX_FRAME_SIZE = 1792 };


typedef
enum XingFlags_enum {
    XING_HAS_FRAMES     = BIT(0),
    XING_HAS_BYTES      = BIT(1),
    XING_HAS_TOC        = BIT(2),
    XING_HAS_VBR_SCALE  = BIT(3)
}
XingFlags;


typedef
struct XingInfo_struct {
    guint32             flags;
    guint               frames;
    guint               bytes;
    guint               vbr_scale;
}
XingInfo;


enum { XING_TOC_LENGTH = 100 };


/* bit_rates[version][layer][bitrate_index] */
static const gint bit_rates[MPEG_N_VERSIONS][MPEG_N_LAYERS][MPEG_N_BITRATES] = {
    {
        { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 },
        { 0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384 },
        { 0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320 }
    },
    {
        { 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 },
        { 0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160 },
        { 0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160 }
    },
    {
        { 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 },
        { 0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160 },
        { 0,  8, 16, 24, 32, 40, 48,  56,  64,  80,  96, 112, 128, 144, 160 }
    }
};


/* sample_rates[version][sample_index] */
static const gint sample_rates[MPEG_N_VERSIONS][MPEG_N_SAMPLERATES] = {
    { 44100, 48000, 32000 },
    { 22050, 24000, 16000 },
    { 11025, 12000,  8000 }
};


/* */
static const char *channel_modes[MPEG_N_MODES] = {
    "stereo", "joint", "dual", "mono"
};


/* compute_bpf(ver, layer, sample, rate) {{{ */

static gdouble
compute_bpf(const MPEGInfo *mpeg)
{
    gdouble bpf = mpeg->bitrate;

    if (mpeg->layer == MPEG_LAYER_1)
        bpf *= 48000.0;
    else
        bpf *= 144000.0;

    if (mpeg->version == MPEG_V_1_0)
        bpf /= mpeg->sampling;
    else
        bpf /= 2 * mpeg->sampling;

    return bpf;
}

/* }}} */

/* compute_tpf() {{{ */

static gdouble
compute_tpf(const MPEGInfo *mpeg)
{
    static const double bs[3] = { 384, 1152, 1152 };

    gdouble tpf = bs[mpeg->layer];

    if (mpeg->version == MPEG_V_1_0)
        tpf /= mpeg->sampling;
    else
        tpf /= 2 * mpeg->sampling;

    return tpf;
}

/* }}} */

/* mpeg_init() {{{ */
/**
 * Initialize MPEG Audio parser.
 */

void
mpeg_init(void)
{
    static const gchar *module_name = "MPEG Audio parser";
    static const gchar *module_extensions[] = { "mpg", "mp2", "mp3", NULL };

    file_parser_register(module_name, module_extensions, FILE_TYPE_AUDIO, mpeg_proc);
}

/* }}} */

/* decode_header(hdr, mpeg) {{{ */

static inline gboolean
decode_header(guint32 hdr, MPEGInfo *mpeg)
{
    int tmp_version, tmp_layer, tmp_bitrate, tmp_sampling;

    if ((hdr & 0xFFE00000) != 0xFFE00000)
        return FALSE;

    if ((hdr & 0xFFFF0000) == 0xFFFE0000)
        return FALSE;

    tmp_version = (hdr >> 19) & 0x3;
    tmp_layer = (hdr >> 17) & 0x3;
    tmp_bitrate = (hdr >> 12) & 0xF;
    tmp_sampling = (hdr >> 10) & 0x3;

    if (tmp_version == 0x1 || tmp_layer == 0x0)
        return FALSE;

    if (tmp_bitrate == 0x0 || tmp_bitrate == 0xF)
        return FALSE;

    if (tmp_sampling == 0x3)
        return FALSE;

    /* mpeg version and layer */
    mpeg->version =
        tmp_version == 0x3 ? MPEG_V_1_0 :
        tmp_version == 0x2 ? MPEG_V_2_0 : MPEG_V_2_5;

    mpeg->layer =
        tmp_layer == 0x3 ? MPEG_LAYER_1 :
        tmp_layer == 0x2 ? MPEG_LAYER_2 : MPEG_LAYER_3;

    /* bitrate, sampling and channels */
    mpeg->bitrate = bit_rates[mpeg->version][mpeg->layer][tmp_bitrate];
    mpeg->sampling = sample_rates[mpeg->version][tmp_sampling];
    mpeg->mode = (hdr >> 6) & 0x3;

    /* flags */
    mpeg->protection = !(hdr & (1 << 16));
    mpeg->padding = 1 && (hdr & (1 << 9));
    mpeg->private = 1 && (hdr & (1 << 8));
    mpeg->copyright = 1 && (hdr & (1 << 3));
    mpeg->original = 1 && (hdr & (1 << 2));

    /* frame size */
    switch (mpeg->layer) {
    case MPEG_LAYER_1:
        mpeg->framesize =
            (mpeg->bitrate * 12000 / mpeg->sampling + mpeg->padding) * 4 - 4;
        break;

    case MPEG_LAYER_2:
        mpeg->framesize =
            mpeg->bitrate * 144000 / mpeg->sampling + mpeg->padding - 4;
        break;

    case MPEG_LAYER_3:
        mpeg->framesize = mpeg->bitrate * 144000 /
            (mpeg->version == MPEG_V_1_0 ? mpeg->sampling : 2 * mpeg->sampling) +
            mpeg->padding - 4;
        break;

    default: break;     /* avoid GCC warnings */
    }

    if (mpeg->framesize > MPEG_MAX_FRAME_SIZE)
        return FALSE;

    return TRUE;
}

/* }}} */

/* xing_decode(hdr, xing) {{{ */

static inline gboolean
xing_decode(const guchar *fhdr, const MPEGInfo *mpeg, XingInfo *xing)
{
    static const gint offset[MPEG_N_VERSIONS][MPEG_N_MODES] = {
        { 32, 32, 32, 17 },     /* MPEG 1.0 */
        { 17, 17, 17,  9 },     /* MPEG 2.0 */
        { 17, 17, 17,  9 }      /* MPEG 2.5 */
       /* St, JS, DC, Mn */
    };

    fhdr += offset[mpeg->version][mpeg->mode];

    if (fhdr[0] != 'X' || fhdr[1] != 'i' || fhdr[2] != 'n' || fhdr[3] != 'g')
        return FALSE;

    fhdr += 4;

    xing->flags = GUINT32_FROM_BE(*(guint32*)fhdr);
    fhdr += sizeof(guint32);

    if (xing->flags & XING_HAS_FRAMES) {
        xing->frames = GUINT32_FROM_BE(*(guint32*)fhdr);
        fhdr += sizeof(guint32);
    }

    if (xing->flags & XING_HAS_BYTES) {
        xing->bytes = GUINT32_FROM_BE(*(guint32*)fhdr);
        fhdr += sizeof(guint32);
    }

    if (xing->flags & XING_HAS_TOC) {
        fhdr += XING_TOC_LENGTH;
    }

    if (xing->flags & XING_HAS_VBR_SCALE) {
        xing->vbr_scale = GUINT32_FROM_BE(*(guint32*)fhdr);
        fhdr += sizeof(guint32);
    }

    return TRUE;
}


/* }}} */

/* mpeg_proc(file_node, fp) {{{ */
/**
 * Process a MPEG Audio file.
 *
 * \param file_node XML node where stream information should be inserted.
 * \param fp Pointer to an open file.
 * \returns TRUE if successful.
 */

gboolean
mpeg_proc(xmlNodePtr file_node,
          FILE *fp)
{
    gint skid = 1048576;

    xmlNodePtr audio_stream_node, id3_node = NULL, id3v2_node = NULL;
    MPEGInfo mpeg;
    guint64 file_size, stream_size;
    guint32 hdr;
    guchar fhdr[MPEG_MAX_FRAME_SIZE], tmp_char;
    gchar tmp[64];

    /* find file size */
#if HAVE_FSEEKO
    fseeko(fp, 0, SEEK_END);
    file_size = ftello(fp);
#else
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
#endif


    /* check for a possible ID3v2 tag */
    id3v2_node = id3v2_parse(fp);

    /* get stream size (discount id3 tags) */
#if HAVE_FSEEKO
    stream_size = file_size - ftello(fp);
#else
    stream_size = file_size - ftell(fp);
#endif

    /* read and parse the MPEG header */
    if (fread(&hdr, sizeof hdr, 1, fp) != 1) {
        if (id3v2_node != NULL)
            xmlFreeNode(id3v2_node);

        return FALSE;
    }

    hdr = GUINT32_FROM_BE(hdr);

    while (!decode_header(hdr, &mpeg)) {
        /* header not found, shift and try next byte */
        /*
         * `skid´ is used to improve performance if you have a big non-mpeg
         * file with a mp3 extension. We stop searching for mpeg header if
         * we skid for more than 1MB.
         */
        if (--skid <= 0 || fread(&tmp_char, 1, 1, fp) != 1) {
            if (id3v2_node != NULL)
                xmlFreeNode(id3v2_node);

            return FALSE;
        }

        hdr = (hdr << CHAR_BIT) | tmp_char;
    }

    /* create and initialize <audio-stream /> element */
    audio_stream_node = xmlNewChild(file_node, NULL, "audio-stream", NULL);
    xmlNewProp(audio_stream_node, "format", "mpeg");

    g_snprintf(tmp, sizeof tmp, "%d", mpeg.version + 1);
    xmlNewProp(audio_stream_node, "mpeg", tmp);

    g_snprintf(tmp, sizeof tmp, "%d", mpeg.layer + 1);
    xmlNewProp(audio_stream_node, "layer", tmp);

    if (id3v2_node != NULL)
        xmlAddChild(audio_stream_node, id3v2_node);

    /* compute song length */
    mpeg.vbr = FALSE;
    mpeg.frames = 0;
    mpeg.length = 0;

    if (fread(fhdr, mpeg.framesize, 1, fp) == 1) {
        /* check for a Xing header (used on VBR streams) */
        XingInfo xing;
        gdouble tpf = compute_tpf(&mpeg);

        if (xing_decode(fhdr, &mpeg, &xing)) {
            /* Xing header found, presumably this is a VBR stream */
            mpeg.vbr = TRUE;
            mpeg.bitrate = 0;

            if (xing.flags & XING_HAS_FRAMES) {
                mpeg.frames = xing.frames;

                if (xing.flags & XING_HAS_BYTES)
                    mpeg.bitrate = xing.bytes * 8 / (xing.frames * tpf * 1000);
            }
        }

        /* parse ID3v1 tag */
        if ((id3_node = id3v1_parse(fp)) != NULL) {
            xmlAddChild(audio_stream_node, id3_node);
            stream_size -= ID3V1_TAG_LENGTH;
        }

        if (mpeg.frames == 0) {
            /* Xing header not found or didn't contain frames, estimate */
            mpeg.frames = stream_size / compute_bpf(&mpeg);
        }

        /* compute song length, in milliseconds */
        mpeg.length = mpeg.frames * tpf * 1000;
    }

    /* add information about the audio stream */
    if (mpeg.bitrate != 0) {
        g_snprintf(tmp, sizeof tmp, "%s%d", mpeg.vbr ? "~" : "", mpeg.bitrate);
        xmlNewProp(audio_stream_node, "bitrate", tmp);
    }

    g_snprintf(tmp, sizeof tmp, "%d", mpeg.sampling);
    xmlNewProp(audio_stream_node, "sample", tmp);

    xmlNewProp(audio_stream_node, "mode", channel_modes[mpeg.mode]);

    if (mpeg.frames != 0) {
        g_snprintf(tmp, sizeof tmp, "%u", mpeg.frames);
        xmlNewProp(audio_stream_node, "frames", tmp);
    }

    if (mpeg.length != 0) {
        g_snprintf(tmp, sizeof tmp, "%u", mpeg.length);
        xmlNewProp(audio_stream_node, "length", tmp);
    }

    return TRUE;
}

/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
