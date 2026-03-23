#include "header.h"

int main(int argc, char *argv[])
{
    WindowData wd;
    Media media;
    Audio audio;
    int run = 1;

    if (!initSystem(&wd)) return 1;
    if (!loadMedia(&wd, &media)) return 1;
    if (!loadAudio(&audio)) return 1;

    while (run)
    {
        events(&wd, &media, &audio, &run);
        render(&wd, &media);
        SDL_Delay(16);
    }

    cleanup(&wd, &media, &audio);
    return 0;
}
