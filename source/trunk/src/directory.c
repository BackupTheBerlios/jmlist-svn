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
#include "directory.h"
#include "main.h"
#include "file.h"


/* directory_proc() {{{ */
/**
 * Process a directory, listing each file and descending recursively into its
 * subdirectories.
 *
 * \param real_path Real path to the directory.
 * \param dir_name Directory name, the last component of its path.
 * \param parent_accum Parameter to add accumulated size/length data.
 * \returns A newly allocated xmlNodePtr.
 */
xmlNodePtr
directory_proc(const gchar *real_path,
               const gchar *dir_name,
               DirAccumData *parent_accum)
{
    DirAccumData accum = { 0, 0 };
    xmlNodePtr dir_node;
    GDir *dir;
    gchar *dir_name_utf8, tmp[64];

    /* create directory node */
    dir_node = xmlNewNode(NULL, "directory");
    dir_name_utf8 = g_filename_to_utf8(dir_name, -1, NULL, NULL, NULL);
    xmlNewProp(dir_node, "name", dir_name_utf8);

    /* open directory */
    dir = g_dir_open(real_path, 0, NULL);

    if (dir != NULL) {                  /* g_dir_open() succeeded */
        const gchar *entry;

        log_info("Processing \"%s\"...", real_path);

        /* visit each directory entry */
        while ((entry = g_dir_read_name(dir)) != NULL) {
            gchar *full_name = g_build_filename(real_path, entry, NULL);

            if (g_file_test(full_name, G_FILE_TEST_IS_DIR)) {
                xmlNodePtr subdir_node = directory_proc(full_name, entry,
                                                        &accum);

                if (subdir_node != NULL)
                    xmlAddChild(dir_node, subdir_node);
            }
            else if (g_file_test(full_name, G_FILE_TEST_IS_REGULAR)) {
                xmlNodePtr file_node = file_proc(full_name, entry, &accum);

                if (file_node != NULL)
                    xmlAddChild(dir_node, file_node);
            }

            g_free(full_name);
        }

        g_dir_close(dir);
    }
    else {                              /* g_dir_open() returned NULL */
        xmlNodePtr comment = xmlNewComment("error opening directory");
        xmlAddChild(dir_node, comment);

        g_warning("%s: %s", real_path, g_strerror(errno));
    }

    if (parent_accum != NULL) {
        parent_accum->size_in_bytes += accum.size_in_bytes;
        parent_accum->length_in_ms += accum.length_in_ms;
    }

    g_snprintf(tmp, sizeof tmp, "%" G_GUINT32_FORMAT, accum.length_in_ms);
    xmlNewProp(dir_node, "length", tmp);

    g_snprintf(tmp, sizeof tmp, "%" G_GUINT64_FORMAT, accum.size_in_bytes);
    xmlNewProp(dir_node, "size", tmp);

    g_free(dir_name_utf8);              /* free UTF-8 converted filename */
    return dir_node;
}

/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
