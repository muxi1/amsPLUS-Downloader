#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_


#define ATMOXL_URL  	"http://members.ps3-tools.de/muxi/amsPLUSDL/amsPLUS.zip"
#define TEMP_FILE       "/switch/amsPLUS-downloader/temp"

#define ON              1
#define OFF             0


#include <stdbool.h>

//
bool downloadFile(const char *url, const char *output, int api);

#endif