#include "sound.h"
#include "log.h"
#include <windows.h>

#include "gen/bg.wav.h"

void sound_create()
{
}

void sound_destroy()
{
}

void sound_play(enum game_sound sound)
{
    BOOL rc;
    const wchar_t* name;

    if (sound == sound_test)
    {
        name = L"whatocean.wav";
    }
    else if (sound == sound_bg)
    {
        name = L"bg.wav";
    }
    else
    {
        return;
    }

    rc = PlaySound(name, NULL, SND_FILENAME | SND_ASYNC);
    if (!rc)
    {
        LOGE("playing %S, error: %d", name, GetLastError());
    }
}
