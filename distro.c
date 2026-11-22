#include "distro.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_SIZE 4096

static Distro distro;
static bool initialized = false;

static char *trim(char *str)
{
    while (isspace(*str))
        str++;

    char *end = str + strlen(str);

    while (end > str && isspace(end[-1]))
        end--;

    *end = '\0';

    return str;
}

static size_t decode_value(char *dst, size_t size, const char *src)
{
    size_t j = 0;
    char quote = 0;

    while (isspace((unsigned char)*src))
        src++;

    if (*src == '"' || *src == '\'')
        quote = *src++;

    while (*src && j + 1 < size) {

        if (quote && *src == quote)
            break;

        if (!quote && isspace((unsigned char)*src))
            break;

        if (*src == '\\' && src[1]) {
            src++;
        }

        dst[j++] = *src++;
    }

    dst[j] = '\0';
    return j;
}

static void parse_line(Distro *distro, char *line)
{
    char *p = trim(line);

    if (*p == '#' || *p == '\0')
        return;

    char *eq = strchr(p, '=');

    if (!eq)
        return;

    *eq = '\0';

    char *key = trim(p);
    char *value = eq + 1;

    char decoded[DISTRO_FIELD_SIZE] = {0};

    size_t len = decode_value(decoded, sizeof(decoded), value);

#define SET_FIELD(k, f) \
    if (strcmp(key, k) == 0) { \
        memcpy(distro->f, decoded, len); \
        return; \
    }

    SET_FIELD("ID", id)
    SET_FIELD("ID_LIKE", id_like)
    SET_FIELD("NAME", name)
    SET_FIELD("PRETTY_NAME", pretty_name)
    SET_FIELD("VERSION", version)
    SET_FIELD("VERSION_ID", version_id)
    SET_FIELD("VERSION_CODENAME", version_codename)
    SET_FIELD("BUILD_ID", build_id)
    SET_FIELD("VARIANT", variant)
    SET_FIELD("VARIANT_ID", variant_id)

#undef SET_FIELD
}

void distro_load(Distro *distro) {

    memset(distro, 0, sizeof(*distro));
    FILE *fp = fopen("/etc/os-release", "r");

    if (!fp)
        fp = fopen("/usr/lib/os-release", "r");

    if (!fp)
        return;

    char line[LINE_SIZE];

    while (fgets(line, sizeof(line), fp))
        parse_line(distro, line);

    fclose(fp);
}

Distro distro_get(void) {
    if (!initialized) {
        distro_load(&distro);
        initialized = true;
    }
    return distro;
}
