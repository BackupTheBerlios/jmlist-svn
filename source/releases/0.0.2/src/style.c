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
#include "style.h"
#include "file.h"

#include <popt.h>


static const gchar *style_output_types[] = {
    "other",
    "text",
    "html",
    "xhtml",
    "xml",
    NULL
};


/* get_user_style_dir() {{{ */
/**
 * Returns the path to the user style directory.
 *
 * \returns The path to the user style directory. Shouldn't be freed.
 */

static inline const gchar *
get_user_style_dir(void)
{
    static gchar *dir = NULL;

    if (dir == NULL)
        dir = g_strdup_printf("%s/." PACKAGE "/styles", g_get_home_dir());

    return dir;
}

/* }}} */

/* get_system_style_dir() {{{ */
/**
 * Returns the path to the system style directory.
 *
 * \returns The path to the system style directory. Shouldn't be freed.
 */

static inline const gchar *
get_system_style_dir(void)
{
    static const gchar *dir = DATADIR "/" PACKAGE "/styles";
    return dir;
}

/* }}} */

/* parse_xml_style_info() {{{ */
/**
 * Parse a style-info XML definition into the given Style object.
 *
 * \param root Root <style-info> XML node.
 * \param style Style object.
 * \returns #TRUE if, and only if, the parsing was successful.
 */

static gboolean
parse_xml_style_info(xmlNodePtr root, Style *style)
{
    xmlNodePtr head, params;

    style->name = NULL;
    style->title = NULL;
    style->version = NULL;
    style->author = NULL;
    style->descr = NULL;
    style->output = STYLE_OTHER;
    style->params = NULL;

    if ((head = xml_get_element_by_name(root->children, "head")) != NULL) {
        xmlNodePtr field;

        if ((field = xml_get_element_by_name(head->children, "name")) != NULL)
            style->name = xmlNodeGetContent(field);

        if ((field = xml_get_element_by_name(head->children, "title")) != NULL)
            style->title = xmlNodeGetContent(field);

        if ((field = xml_get_element_by_name(head->children, "version")) != NULL)
            style->version = xmlNodeGetContent(field);

        if ((field = xml_get_element_by_name(head->children, "author")) != NULL)
            style->author = xmlNodeGetContent(field);

        if ((field = xml_get_element_by_name(head->children, "descr")) != NULL)
            style->descr = xmlNodeGetContent(field);

        if ((field = xml_get_element_by_name(head->children, "output")) != NULL) {
            xmlChar *output = xmlNodeGetContent(field);
            gint i;

            for (i = 0; style_output_types[i] != NULL; i++)
                if (strcmp(output, style_output_types[i]) == 0) {
                    style->output = i;
                    break;
                }

            xmlFree(output);
        }
    }
    else {
        return FALSE;
    }

    if ((params = xml_get_element_by_name(root->children, "params")) != NULL) {
        xmlNodePtr param;

        for (param = xml_get_element_by_name(params->children, "param");
             param; param = xml_get_element_by_name(param->next, "param")) {
            StyleParam *param_obj;
            xmlNodePtr descr;
            gchar *name;

            if ((name = xmlGetProp(param, "name")) == NULL)
                continue;

            param_obj = g_new0(StyleParam, 1);
            param_obj->name = name;

            if ((descr = xml_get_element_by_name(param->children, "short-descr")) != NULL)
                param_obj->short_descr = xmlNodeGetContent(descr);

            if ((descr = xml_get_element_by_name(param->children, "descr")) != NULL)
                param_obj->descr = xmlNodeGetContent(descr);

            style->params = g_list_append(style->params, param_obj);
        }
    }

    return TRUE;
}

/* }}} */

/* style_load() {{{ */
/**
 * Load a stylesheet by its filename and return its object.
 *
 * \param filename The full path to the stylesheet file, in filesystem
 *      encoding.
 * \param load_style If #TRUE, the stylesheet data will be loaded also.
 *      Otherwise, only general description data is loaded.
 * \returns A new #Style object or NULL on error.
 */

Style *
style_load(const gchar *filename, gboolean load_style)
{
    Style *style = NULL;
    gchar *info_filename;

    xmlDocPtr doc = NULL;

    /* if file doesn't exist, just quit */
    if (!g_file_test(filename, G_FILE_TEST_IS_REGULAR))
        return NULL;

    info_filename = filename_change_extension(filename, "-info.xml");

    /* check for a meta information file */
    if (g_file_test(info_filename, G_FILE_TEST_IS_REGULAR) &&
        (doc = xmlReadFile(info_filename, NULL, 0)) != NULL) {

        style = g_new0(Style, 1);

        /* parse xml from info file */
        if (parse_xml_style_info(xmlDocGetRootElement(doc), style)) {
            /* finally, load the stylesheet */
            if (load_style) {
                style->stylesheet = xsltParseStylesheetFile(filename);

                if (style->stylesheet == NULL) {
                    /* failed to load stylesheet */
                    g_critical("%s: error loading stylesheet.", filename);
                    style_destroy(style);
                    style = NULL;
                }
            }
        }
        else {
            /* TODO: check utf8 */
            g_critical("%s: error parsing XML data from info file %s.",
                       filename, info_filename);
            style_destroy(style);
            style = NULL;
        }

        xmlFreeDoc(doc);
    }
    else {
        /* TODO: check utf8 */
        g_critical("%s: error parsing XML data from info file %s.",
                   filename, info_filename);
    }


    g_free(info_filename);
    return style;
}

/* }}} */

/* style_destroy() {{{ */
/**
 * Free memory used by a Style object.
 */

void
style_destroy(Style *style)
{
    GList *node;

    g_free(style->name);
    g_free(style->title);
    g_free(style->version);
    g_free(style->author);
    g_free(style->descr);

    while ((node = g_list_first(style->params)) != NULL) {
        StyleParam *param = (StyleParam *) node->data;

        style->params = g_list_delete_link(style->params, node);

        g_free(param->name);
        g_free(param->short_descr);
        g_free(param->descr);
        g_free(param->value);
        g_free(param);
    }

    if (style->stylesheet != NULL)
        xsltFreeStylesheet(style->stylesheet);

    g_free(style);
}

/* }}} */

/* style_popt_callback() [UNUSED] {{{ */
#if 0
/**
 * Callback function used with popt. Currently not necessary.
 */

static void
style_popt_callback(poptContext con,
                    enum poptCallbackReason reason,
                    const struct poptOption *opt,
                    const char *arg, void *data)
{
}

#endif
/* }}} */

/* style_get_popt_table() {{{ */
/**
 * Generate a popt option table based on the parameters of the given style.
 *
 * \param style A Style object.
 * \returns A newly allocated popt option table.
 */

struct poptOption *
style_get_popt_table(Style *style)
{
    const gint num_params = g_list_length(style->params);

    struct poptOption *options = g_new0(struct poptOption, num_params + 2);
    gint i = 0, n;
    GList *node;

#if 0
    /* set callback function */
    options[i].argInfo = POPT_ARG_CALLBACK;
    options[i].arg = (gpointer) style_popt_callback;
    options[i].descrip = (gpointer) style->params;
    i++;
#endif

    /* add options */
    for (n = 0, node = g_list_first(style->params);
         node; n++, node = g_list_next(node)) {
        StyleParam *param = (StyleParam *) node->data;

        options[i].longName = g_strconcat("s-", param->name, NULL);
        options[i].argInfo = POPT_ARG_STRING;
        options[i].arg = &param->value;
        options[i].descrip = param->short_descr;
        i++;
    }

    /* mark end of table */
    options[i].longName = NULL;
    options[i].shortName = 0;
    options[i].arg = NULL;

    return options;
}

/* }}} */

/* style_find_by_name() {{{ */
/**
 * Find an user or system installed stylesheet given its name.
 */

Style *
style_find_by_name(const gchar *name)
{
    Style *style = NULL;
    gchar path[PATH_MAX+1];

    /* check for style in user directory */
    g_snprintf(path, sizeof path, "%s/%s.xsl", get_user_style_dir(), name);
    log_info("Trying \"%s\"...", path);

    if ((style = style_load(path, TRUE)) == NULL) {
        /* not found in user directory, try system directory */
        g_snprintf(path, sizeof path, "%s/%s.xsl", get_system_style_dir(), name);
        log_info("Trying \"%s\"...", path);

        if ((style = style_load(path, TRUE)) == NULL) {
            /* also not found in system directory, leave */
            return NULL;
        }
    }

    log_info("Using stylesheet \"%s\".", path);
    return style;
}

/* }}} */

/* style_apply() {{{ */
/**
 * Apply a style to the given document.
 *
 * \param style The style to be applied.
 * \param doc The document to be transformed.
 * \returns The resulting XML document, that should be destroied with
 *      xmlFreeDoc().
 */

xmlDocPtr
style_apply(Style *style, xmlDocPtr doc)
{
    gchar **params = NULL;
    gint n_params;
    xmlDocPtr newdoc;

    log_info("Applying XSL Stylesheet...");

    /* generate parameter array */
    if (style->params != NULL) {
        gint i;
        GList *node;

        n_params = g_list_length(style->params);
        params = g_new0(gchar *, 2 * n_params + 1);

        for (node = g_list_first(style->params), i = 0; node;
             node = g_list_next(node)) {
            StyleParam *param = (StyleParam*) node->data;

            if (param->value != NULL) {
                params[i++] = g_strdup(param->name);

                if (strchr(param->value, '"') == NULL) {
                    params[i++] = g_strconcat("\"", param->value, "\"", NULL);
                }
                else if (strchr(param->value, '\'') == NULL) {
                    params[i++] = g_strconcat("'", param->value, "'", NULL);
                }
                else {
                    gchar *s;

                    g_warning("Parameter %s contains both quotes and double-quotes!",
                              param->name);

                    for (s = param->value; *s != '\0'; s++)
                        if (*s == '"')
                            *s = '\'';

                    params[i++] = g_strconcat("\"", param->value, "\"", NULL);
                }
            }
        }
    }

    /* apply the stylesheet */
    newdoc = xsltApplyStylesheet(style->stylesheet, doc, (const gchar **)params);

    /* cleanup */
    if (params != NULL) {
        gint i;

        for (i = 0; params[i] != NULL; i++)
            g_free(params[i]);

        g_free(params);
    }

    return newdoc;
}

/* }}} */

/* style_list_dir() {{{ */
/**
 * List styles in a given directory.
 *
 * \param path Directory.
 */

static void
style_list_dir(const gchar *path)
{
    GDir *dir = g_dir_open(path, 0, NULL);
    Style *style;
    const gchar *entry;

    if (dir == NULL) {
        g_print("  (directory doesn't exist or is not readable)\n");
        return;
    }

    while ((entry = g_dir_read_name(dir)) != NULL) {
        gchar *entry_utf8, *stylename, *filename;
        const gchar *extension;

        /* Convert it to UTF-8 */
        if ((entry_utf8 = g_filename_to_utf8(entry, -1, NULL, NULL, NULL)) == NULL)
            continue;

        /* Find file extension */
        extension = filename_get_extension(entry_utf8);

        /* Check if it is a XSL file */
        if (extension != NULL && g_strcasecmp(extension, "xsl") == 0) {
            filename = g_build_filename(path, entry, NULL);
            stylename = g_strndup(entry_utf8, extension - entry_utf8 - 1);

            if ((style = style_load(filename, FALSE)) != NULL) {
                g_print("  %-20s%s\n", style->name, style->title);
                style_destroy(style);
            }

            g_free(stylename);
            g_free(filename);
        }

        g_free(entry_utf8);
    }

    g_dir_close(dir);
}

/* }}} */

/* style_list() {{{ */
/**
 * List all styles available.
 */

void
style_list(void)
{
    const gchar *path;

    /* list user styles */
    path = get_user_style_dir();

    if (g_file_test(path, G_FILE_TEST_IS_DIR)) {
        g_print("User styles [%s]:\n", path);
        style_list_dir(path);
        g_print("\n");
    }
    else {
        g_print("User style directory '%s' not found. No user styles.\n\n", path);
    }

    path = get_system_style_dir();
    g_print("System styles [%s]:\n", path);
    style_list_dir(path);
    g_print("\n");
}

/* }}} */


/* vim: set ts=8 sw=4 et: */
/* vim600: set fdm=marker fdc=3: */
