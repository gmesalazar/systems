#ifndef AR_H
#define AR_H

#define AR_MAGIC_STR "!<arch>\n"
#define AR_MAGIC_SZ 8

#define F_HDR_SZ 60

enum sizes {
    FNAME_SZ = 16,
    DATE_SZ = 12,
    UID_SZ = 6,
    GID_SZ = 6,
    MODE_SZ = 8,
    FSIZE_SZ = 10,
    MAGIC_SZ = 2
};

struct header {
    char fname[FNAME_SZ];
    char date[DATE_SZ];
    char uid[UID_SZ];
    char gid[GID_SZ];
    char mode[MODE_SZ];
    char fsize[FSIZE_SZ];
    char magic[MAGIC_SZ];
};

#define TRUE 1
#define FALSE 0

typedef unsigned char bool;

#endif //AR_H
