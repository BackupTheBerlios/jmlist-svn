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
#include "main.h"
#include "directory.h"
#include "file.h"
#include "style.h"

#include <locale.h>
#include <popt.h>


int opt_verbose = FALSE;


static void
log_handler(const gchar *log_domain, GLogLevelFlags log_level,
            const gchar *message, gpointer user_data);


/* main() {{{ */
/**
 * Main program function.
 *
 * \param argc Number of command-line arguments.
 * \param argv Array of command-line arguments.
 * \returns 0 on successful program completion.
 */

int
main(int argc, char **argv)
{
    char *opt_output = NULL;
    char *opt_style = NULL;
    const char **args;
    poptContext optcon;
    int opt, i;

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    Style *style = NULL;


    enum {
        OPT_NONE,
        OPT_VERSION,
        OPT_STYLE,
        OPT_LIST_STYLES
    };

    struct poptOption options_output[] = {
        {
            "output", 'o', POPT_ARG_STRING, &opt_output, 0,
            "File where the output will be written to", "FILE"
        },
        {
            "style", 's', POPT_ARG_STRING, &opt_style, OPT_STYLE,
            "Stylesheet used to format the output", "STYLE"
        },
        POPT_TABLEEND
    };

    struct poptOption options_style_placeholder[] = { POPT_TABLEEND };
    struct poptOption *options_style = NULL;

    struct poptOption options[] = {
        {
            "list-styles", 0, POPT_ARG_NONE, NULL, OPT_LIST_STYLES,
            "List available styles"
        },
        {
            "verbose", 'v', POPT_ARG_NONE, &opt_verbose, 0,
            "Display progress while scanning directories"
        },
        {
            "version", 0, POPT_ARG_NONE, NULL, OPT_VERSION,
            "Display version information"
        },
        { NULL, 0, POPT_ARG_INCLUDE_TABLE, &options_output, 0, "Output options:" },
        { NULL, 0, POPT_ARG_INCLUDE_TABLE, &options_style_placeholder, 0, "Style options:" },
        POPT_AUTOHELP
        POPT_TABLEEND
    };


    /* Internal initialization. */
    LIBXML_TEST_VERSION;

    setlocale(LC_ALL, "");

    g_set_application_name("jMList");
    g_set_prgname(g_path_get_basename(argv[0]));
    g_log_set_handler(NULL, G_LOG_LEVEL_MASK, log_handler, NULL);

    xmlSubstituteEntitiesDefault(1);

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

        case OPT_STYLE:
            if (style != NULL) {
                g_critical("--style option specified more than once.");
                exit(EXIT_FAILURE);
            }
            if ((style = style_find_by_name(opt_style)) == NULL) {
                g_critical("There is no style named '%s'.", opt_style);
                g_critical("Try `%s --list-styles' for a list of available styles.",
                           g_get_prgname());
                exit(EXIT_FAILURE);
            }

            options_style = style_get_popt_table(style);

            for (i = 0; (options[i].longName != NULL ||
                         options[i].shortName != '\0' ||
                         options[i].arg != NULL); i++)
                if (options[i].arg == &options_style_placeholder)
                    options[i].arg = options_style;

            break;

        case OPT_LIST_STYLES:
            style_list();
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


    /* Apply stylesheet if requested */
    if (style != NULL) {
        xmlDocPtr newdoc = style_apply(style, doc);

        if (newdoc == NULL) {
            g_critical("Error applying stylesheet to output.");
            xmlFreeDoc(doc);
            xmlCleanupParser();
            exit(EXIT_FAILURE);
        }

        log_info("Generating transformed XML output...");

        /* Output the transformed XML document */
        if (opt_output != NULL) {
            FILE *output = fopen(opt_output, "wb");

            if (output == NULL) {
                g_critical("%s: %s", opt_output, g_strerror(errno));
                exit(EXIT_FAILURE);
            }

            if (xsltSaveResultToFile(output, newdoc, style->stylesheet) < 0) {
                g_critical("%s: %s", opt_output, g_strerror(errno));
                exit(EXIT_FAILURE);
            }

            fclose(output);
        }
        else {
            if (xsltSaveResultToFile(stdout, newdoc, style->stylesheet) < 0) {
                g_critical("%s: %s", opt_output, g_strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        /* Output the resulting XML document without stylesheet */
        log_info("Generating XML output...");
        xmlSaveFormatFileEnc(opt_output ? opt_output : "-", doc, "UTF-8", 1);
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();

    log_info("Done.");
    return EXIT_SUCCESS;
}

/* }}} */

/* log_handler() {{{ */
/**
 * GLib log handler.
 */
static void
log_handler(const gchar *log_domain, GLogLevelFlags log_level,
            const gchar *message, gpointer user_data)
{
    switch (log_level) {
    case G_LOG_LEVEL_ERROR:
        g_printerr("<<<ERROR>>> %s\n", message);
        break;

    case G_LOG_LEVEL_CRITICAL:
        g_printerr("[!] %s\n", message);
        break;

    case G_LOG_LEVEL_WARNING:
        g_printerr("[W] %s\n", message);
        break;

    case G_LOG_LEVEL_MESSAGE:
        g_printerr("%s\n", message);
        break;

    case G_LOG_LEVEL_INFO:
        if (opt_verbose)
            g_printerr("-- %s\n", message);
        break;

    case G_LOG_LEVEL_DEBUG:
#if DEBUG
        g_printerr(":: %s\n", message);
        break;
#endif

    default:
        break;
    }
}

/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
