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
#include "main.h"

#include "directory.h"
#include "file.h"

#include <locale.h>
#include <popt.h>


int
main(int argc, char **argv)
{
    char *opt_output = NULL;
    const char **args;
    poptContext optcon;
    int opt;

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;


    enum { OPT_NONE, OPT_VERSION };

    struct poptOption options_output[] = {
        {
            "output", 'o', POPT_ARG_STRING, &opt_output, 0,
            "File where the output will be written to", "FILE"
        },
        POPT_TABLEEND
    };

    struct poptOption options[] = {
        {
            "version", 0, POPT_ARG_NONE, NULL, OPT_VERSION,
            "Display version information"
        },
        { NULL, 0, POPT_ARG_INCLUDE_TABLE, &options_output, 0, "Output options:" },
        POPT_AUTOHELP
        POPT_TABLEEND
    };


    /* Internal initialization. */
    LIBXML_TEST_VERSION;
    setlocale(LC_ALL, "");
    g_set_application_name("jMList");
    g_set_prgname(g_path_get_basename(argv[0]));
    file_parser_init_all();


    /* Parse command-line arguments. */
    optcon = poptGetContext(NULL, argc, (const char**) argv, options, 0);
    poptSetOtherOptionHelp(optcon, "[OPTION...] [DIRECTORY...]");

    while ((opt = poptGetNextOpt(optcon)) >= 0) {
        switch (opt) {
        case OPT_VERSION:
            g_print(PACKAGE_STRING "\n");
            g_print(PACKAGE_URL "\n");
            g_print("Copyright (C) 2004 - Juliano Ravasi Ferraz\n");
            exit(EXIT_SUCCESS);
        }
    }

    if (opt < -1) {
        g_printerr("%s: %s\n",
                   poptBadOption(optcon, POPT_BADOPTION_NOALIAS),
                   poptStrerror(opt));
        poptPrintUsage(optcon, stderr, 0);
        exit(EXIT_FAILURE);
    }


    /* Create a new XML document and its root node. */
    doc = xmlNewDoc("1.0");
    root_node = xmlNewNode(NULL, "jmlist");
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, "version", "0 (beta)");
    xmlNewProp(root_node, "generated-by", PACKAGE_STRING);
    xmlNewProp(root_node, "generator-url", PACKAGE_URL);


    /* Scan the given directories, and generate the output file. */
    args = poptGetArgs(optcon);

    if (args == NULL) {                 /* Run just for the current directory */
        char *current_dir, *dir_name;
        xmlNodePtr dir;

        current_dir = g_get_current_dir();
        dir_name = g_path_get_basename(current_dir);

        dir = directory_proc(".", dir_name);
        xmlAddChild(root_node, dir);

        g_free(dir_name);
        g_free(current_dir);
    }
    else {                              /* Run for each given directory */
        const char **arg;

        for (arg = args; *arg != NULL; arg++) {
            char *dir_name;
            xmlNodePtr dir;

            dir_name = g_path_get_basename(*arg);

            dir = directory_proc(*arg, dir_name);
            xmlAddChild(root_node, dir);

            g_free(dir_name);
        }
    }


    /* Output the resulting XML document. */
    xmlSaveFormatFileEnc(opt_output ? opt_output : "-", doc, "UTF-8", 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();

    return EXIT_SUCCESS;
}



/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
