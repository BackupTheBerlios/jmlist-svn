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
#include "directory.h"

#include "file.h"



xmlNodePtr
directory_proc(const gchar *real_path, const gchar *dir_name)
{
    xmlNodePtr dir_node;
    GDir *dir;
    gchar *dir_name_utf8;

    /* create directory node */
    dir_node = xmlNewNode(NULL, "directory");
    dir_name_utf8 = g_filename_to_utf8(dir_name, -1, NULL, NULL, NULL);
    xmlNewProp(dir_node, "name", dir_name_utf8);

    /* open directory */
    dir = g_dir_open(real_path, 0, NULL);

    if (dir != NULL) {                  /* g_dir_open() succeeded */
        const gchar *entry;

        /* visit each directory entry */
        while ((entry = g_dir_read_name(dir)) != NULL) {
            gchar *full_name = g_build_filename(real_path, entry, NULL);

            if (g_file_test(full_name, G_FILE_TEST_IS_DIR)) {
                xmlNodePtr subdir_node = directory_proc(full_name, entry);

                if (subdir_node != NULL)
                    xmlAddChild(dir_node, subdir_node);
            }
            else if (g_file_test(full_name, G_FILE_TEST_IS_REGULAR)) {
                xmlNodePtr file_node = file_proc(full_name, entry);

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

        g_printerr("%s: %s\n", real_path, g_strerror(errno));
    }

    g_free(dir_name_utf8);              /* free UTF-8 converted filename */
    return dir_node;
}



/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
