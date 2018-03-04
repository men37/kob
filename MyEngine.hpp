/*

    Pseudonym.XgQpAOMjEs claims all copyright interest in the program "kob" (which dynamically masks inputted passwords through reassignment of ascii key codes)


    Copyright 2018 Pseudonym.XgQpAOMjEs

        This file is part of kob

        kob is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://w...content-available-to-author-only...u.org/licenses/>.

*/

#ifndef MY_ENGINE_HPP
#define MY_ENGINE_HPP

#include <cstdint>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

enum
{
    eEngine_Ok = 0,
    eEngine_Ok_Rendered,
    eEngine_AllegroCouldNotInit,
    eEngine_AllegroCouldNotInitKeyboard,
    eEngine_AllegroCouldNotInitFontAddon,
    eEngine_AllegroCouldNotInitTTFAddon,
    eEngine_AllegroCouldNotInitImageAddon,
    eEngine_AllegroCouldNotInitPrimitivesAddon,
    eEngine_AllegroDisplayCreationFailed,
    eEngine_AllegroTimerCreationFailed,
    eEngine_AllegroEventQueueCreationFailed,
    eEngine_FontLoadError,
    eEngine_Event_ClosedDisplay
};

class CEngine;
class CApp;

typedef void (*drawCodeFcn)(CApp*,CEngine*);

class CEngine
{
public:
    CEngine();
    ~CEngine();
    int32_t init(const int32_t nScreenW, const int32_t nScreenH, const bool bMaximized);
    int32_t deInit();
    void setDimensions(const int32_t nScreenW, const int32_t nScreenH);
    void setFPS(const double dFPS);
    int32_t getScreenWidth();
    int32_t getScreenHeight();
    double getFPS();
    ALLEGRO_FONT* getDefaultFont();
    void setDefaultFontSize(const int32_t nSize);
    int32_t frame(CApp* pApp);
    void setDrawCode(drawCodeFcn pfcn);
    bool running();
    void draw(CApp* pApp);
    ALLEGRO_EVENT_QUEUE* getEventQueue();
private:
    int32_t m_screenWidth;
    int32_t m_screenHeight;

    double m_dFPS;

    bool m_bRunning;
    bool m_bRedraw;

    ALLEGRO_DISPLAY* m_display;
    ALLEGRO_DISPLAY_MODE m_displayData;

    ALLEGRO_FONT* m_font;
    int32_t m_nFontSize;

    ALLEGRO_EVENT_QUEUE* m_event_queue;

    ALLEGRO_TIMER* m_timer;

    drawCodeFcn m_drawCode;
};

#endif
