#include "pch.h"
#include "file.h"

Size FileSizeOf( FILE *fp ) {
    Position fpos_bkp = ftell(fp);
    Size ret;

    fseek(fp, 0, SEEK_END);
    ret = ftell(fp);
    fseek(fp, fpos_bkp, SEEK_SET);

    return ret;
}

Size FileTailSizeOf(FILE *fp) {
    Position filepos_bkp = ftell(fp);
    Size ret;

    fseek(fp, 0, SEEK_END);
    ret = ftell(fp) - filepos_bkp;
    fseek(fp, filepos_bkp, SEEK_SET);

    return ret;
}

Size FileSectionSizeOf( FILE *fp, int sep) {
    Position fsize_bkp = ftell(fp);
    Counter read_bytes;
    Size ret;
    char c;

    do {
		SHIELD( (read_bytes=fread(&c, 1, sizeof(char), fp)) > 0,
            "Falha na leitura do arquivo",
			fseek(fp, fsize_bkp, SEEK_SET);
			return 0; )
    } while(c!=(char)sep);

    ret = ftell(fp) - fsize_bkp - sizeof(char);
    fseek(fp, fsize_bkp, SEEK_SET);

    return ret;
}

Position FilePositionAtChar(FILE *fp, int c) {
    Position fsize_bkp = ftell(fp), ret;
    Counter read_bytes;
    char read_c;

    do {
		SHIELD( (read_bytes=fread(&read_c, 1, sizeof(char), fp)) > 0,
            "Falha na leitura do arquivo",
			fseek(fp, fsize_bkp, SEEK_SET);
			return 0; )
    } while(read_c!=(char)c);

    fseek(fp, -1, SEEK_CUR);
    ret = ftell(fp);

    return ret;
}

Position FilePositionOf( FILE *fp, int c) {
    Size fsize_bkp = ftell(fp), read_bytes;
    Position ret;
    char read_c;

    do {
		SHIELD( (read_bytes=fread(&read_c, 1, sizeof(char), fp)) > 0,
            "Falha na leitura do arquivo",
			fseek(fp, fsize_bkp, SEEK_SET);
			return 0; )
    } while(read_c!=(char)c);

    ret = ftell(fp) - sizeof(char);

    fseek(fp, fsize_bkp, SEEK_SET);

    return ret;
}

Counter FileSkipChars(FILE *fp, char *c) {
    Position fpos_bkp = ftell(fp);
    Counter count=0, read_bytes;
    char read_c;

    do {
		SHIELD( (read_bytes=fread(&read_c, 1, sizeof(char), fp)) > 0,
            "Falha na leitura do arquivo",
			fseek(fp, fpos_bkp, SEEK_SET);
			return 0L; )
        if(strchr(c, read_c))
            count++;
        else
            break;
    } while(1);

    fseek(fp, -1, SEEK_CUR);

    return count;
}
