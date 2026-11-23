#ifndef DISTRO_H
#define DISTRO_H

#include <stddef.h>

#define DISTRO_FIELD_SIZE 256

typedef struct {
    char id[DISTRO_FIELD_SIZE];
    char id_like[DISTRO_FIELD_SIZE];
    char name[DISTRO_FIELD_SIZE];
    char pretty_name[DISTRO_FIELD_SIZE];
    char version[DISTRO_FIELD_SIZE];
    char version_codename[DISTRO_FIELD_SIZE];
    char version_id[DISTRO_FIELD_SIZE];
    char codename[DISTRO_FIELD_SIZE];
    char build_id[DISTRO_FIELD_SIZE];
    char variant[DISTRO_FIELD_SIZE];
    char variant_id[DISTRO_FIELD_SIZE];
    char architecture[DISTRO_FIELD_SIZE];
} Distro;

Distro *distro_get(void);

#endif
