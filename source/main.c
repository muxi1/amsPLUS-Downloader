#include <stdio.h>
#include <stdarg.h>
#include <unistd.h> // chdir
#include <dirent.h> // mkdir
#include <switch.h>

#include "download.h"
#include "unzip.h"


#define ROOT                    "/"
#define APP_PATH                "/switch/amsPLUS-downloader/"
#define APP_OUTPUT              "/switch/amsPLUS-downloader/amsPLUS-downloader.nro"
#define OLD_APP_PATH            "/switch/amsPLUS-downloader.nro"



#define APP_VERSION             "1.0.7"
#define CURSOR_LIST_MAX         0 // Anzahl an Cursorn


const char *OPTION_LIST[] =
{
    "Download amsPLUS Daten fuer das Update ueber das Tegra Explorer Script",
    
    };

void refreshScreen(int cursor)
{
    consoleClear();
    printf("================================================================================");
    printf("\x1B[33mamsPLUS Downloader: v%s\x1B[0m\n", APP_VERSION);
	printf("================================================================================");
	printf("\n\n\n");
    printf("Druecke (A) zum Downloaden\n\n");
    printf("Druecke (+) zum Beenden\n\n\n");

    for (int i = 0; i < CURSOR_LIST_MAX + 1; i++)		
	    printf("================================================================================");
        printf("Download amsPLUS Daten fuer das Update ueber das Tegra Explorer Script\n");
		printf("================================================================================");
		printf("\n\n");

    consoleUpdate(NULL);
}

void printDisplay(const char *text, ...)
{
    va_list v;
    va_start(v, text);
    vfprintf(stdout, text, v);
    va_end(v);
    consoleUpdate(NULL);
}

int appInit()
{
    consoleInit(NULL);
    socketInitializeDefault();
    return 0;
}

void appExit()
{
    socketExit();
    consoleExit(NULL);
}

int main(int argc, char **argv)
{
    // init stuff
    appInit();
    mkdir(APP_PATH, 0777);

    // change directory to root (defaults to /switch/)
    chdir(ROOT);

    // set the cursor position to 0
    short cursor = 0;

    // HID init
    padConfigureInput(8, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeAny(&pad);
    
    // main menu
    refreshScreen(cursor);

    // muh loooooop
    while(appletMainLoop())
    {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        // move cursor down...
        if (kDown & HidNpadButton_Down)
        {
            if (cursor == CURSOR_LIST_MAX) cursor = 0;
            else cursor++;
            refreshScreen(cursor);
        }

        // move cursor up...
        if (kDown & HidNpadButton_Up)
        {
            if (cursor == 0) cursor = CURSOR_LIST_MAX;
            else cursor--;
            refreshScreen(cursor);
        }

        if (kDown & HidNpadButton_A)
        {
            switch (cursor)
            {
            case UP_ATMOXL:
                if (downloadFile(ATMOXL_URL, TEMP_FILE, OFF))
                    unzip(TEMP_FILE);
                
				else
                {
                    printDisplay("Download fehlgeschlagen\n");
                }
                break;

            }
        }
        
        // exit...
        if (kDown & HidNpadButton_Plus) break;
    }

    // cleanup then exit
    appExit();
    return 0;
}