#include <stdio.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#include "unzip.h"

#define WRITEBUFFERSIZE 0x1000 // 4KiB
#define MAXFILENAME     0x301

int unzip(const char *output)
{
    unzFile zfile = unzOpen(output);
    unz_global_info gi = {0};
    unzGetGlobalInfo(zfile, &gi);
    double entriesPCT = 0.0;
    for (int i = 0; i < gi.number_entry; i++)
    {
        char filename_inzip[MAXFILENAME] = {0};
        unz_file_info file_info = {0};

        unzOpenCurrentFile(zfile);
        unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

        // check if the string ends with a /, if so, then its a directory.
        if ((filename_inzip[strlen(filename_inzip) - 1]) == '/')
        {
            // check if directory exists
            DIR *dir = opendir(filename_inzip);
            if (dir) closedir(dir);
            else
            {
                //printf("Erstelle Verzeichnis: %s\n", filename_inzip);
                mkdir(filename_inzip, 0777);
            }
        }

        else
        {
            const char *write_filename = filename_inzip;
            FILE *outfile = fopen(write_filename, "wb");
            void *buf = malloc(WRITEBUFFERSIZE);

            entriesPCT = (100.0 / gi.number_entry * i);
            printf("                                                                               \r");
            printf("\x1B[32mSchreibe Datei [%.*f%%]: %.*s\r\x1B[0m",2,entriesPCT,52,filename_inzip);
            fflush(stdout);
            consoleUpdate(NULL);

            for (int j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE); j > 0; j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE))
                fwrite(buf, 1, j, outfile);

            fclose(outfile);
            free(buf);
        }

        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
        consoleUpdate(NULL);
    }

    unzClose(zfile);
    remove(output);
    
    printf("\nFertig!\n\n\nDruecke (+) zum Beenden\n");
    consoleUpdate(NULL);

    return 0;
}