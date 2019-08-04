#ifndef MINI_FILE_H
#define MINI_FILE_H

#include "base/type.h"

Size        FileSizeOf( FILE *fp );

Size        FileTailSizeOf(FILE *fp);

Size        FileSectionSizeOf( FILE *fp, int sep);

Position   FilePositionOf( FILE *fp, int c);

Counter      FileSkipChars(FILE *fp, char *c);

Position   FilePositionAtChar(FILE *fp, int c);

#endif // MINI_FILE_H
