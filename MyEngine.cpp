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

#include "MyEngine.hpp"


CEngine::CEngine() : m_screenWidth(0), m_screenHeight(0), m_dFPS(60.0), m_bRunning(false), m_bRedraw(false), m_display(nullptr), m_font(nullptr), m_nFontSize(10),
m_event_queue(nullptr), m_timer(nullptr)
{

}

CEngine::~CEngine()
{

}

int32_t CEngine::init(const int32_t nScreenW, const int32_t nScreenH, const bool bMaximized)
{

    if (!al_init())
    {
        return eEngine_AllegroCouldNotInit;
    }

    if (!al_init_image_addon())
    {
        return eEngine_AllegroCouldNotInitImageAddon;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        return eEngine_AllegroCouldNotInitTTFAddon;
    }


    if (!al_init_primitives_addon())
    {
        return eEngine_AllegroCouldNotInitPrimitivesAddon;
    }


    if(!al_install_keyboard())
    {
        return eEngine_AllegroCouldNotInitKeyboard;
    }


    if (bMaximized == true)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    }
    else
    {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
    }

    al_get_display_mode(al_get_num_display_modes() - 1, &m_displayData);

    m_screenWidth = nScreenW;
    m_screenHeight = nScreenH;

    m_display = al_create_display(m_screenWidth, m_screenHeight);


    if (m_display == nullptr)
    {
        return eEngine_AllegroDisplayCreationFailed;
    }

    m_font = al_load_ttf_font("honey_room/HoneyRoom.ttf", m_nFontSize, 0);
    if (!m_font)
    {
        return eEngine_FontLoadError;
    }



    m_timer = al_create_timer(1.0 / m_dFPS);
    if (!m_timer)
    {
        al_destroy_display(m_display);
        return eEngine_AllegroTimerCreationFailed;
    }

    m_event_queue = al_create_event_queue();
    if (!m_event_queue)
    {
        al_destroy_display(m_display);
        return eEngine_AllegroEventQueueCreationFailed;
    }

    al_register_event_source(m_event_queue, al_get_display_event_source(m_display));
    al_register_event_source(m_event_queue, al_get_timer_event_source(m_timer));
    al_register_event_source(m_event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(m_timer);

    m_bRunning = true;

    return eEngine_Ok;
}

int32_t CEngine::deInit()
{
    al_destroy_font(m_font);

    al_shutdown_font_addon();
    al_shutdown_ttf_addon();

    al_destroy_display(m_display);
    al_destroy_timer(m_timer);
    al_destroy_event_queue(m_event_queue);


    return eEngine_Ok;
}

void CEngine::setDimensions(const int32_t nScreenW, const int32_t nScreenH)
{
    m_screenWidth = nScreenW;
    m_screenHeight = nScreenH;
}

void CEngine::setFPS(const double dFPS)
{
    m_dFPS = dFPS;
}

int32_t CEngine::getScreenWidth()
{
    return m_screenWidth;
}

int32_t CEngine::getScreenHeight()
{
    return m_screenHeight;
}

double CEngine::getFPS()
{
    return m_dFPS;
}

ALLEGRO_FONT* CEngine::getDefaultFont()
{
    return m_font;
}

void CEngine::setDefaultFontSize(const int32_t nSize)
{
    m_nFontSize = nSize;
}

int32_t CEngine::frame(CApp* pApp)
{
    ALLEGRO_EVENT ev;

    al_wait_for_event(m_event_queue, &ev);

    if (ev.type == ALLEGRO_EVENT_TIMER)
    {
        m_bRedraw = true;
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        m_bRunning = false;
        return eEngine_Event_ClosedDisplay;
    }

    if (m_bRedraw && al_is_event_queue_empty(m_event_queue))
    {
        m_bRedraw = false;

        m_drawCode(pApp, this);

        al_flip_display();

        return eEngine_Ok_Rendered;
    }


    return eEngine_Ok;
}

void CEngine::setDrawCode(drawCodeFcn pfcn)
{
    m_drawCode = pfcn;
}

bool CEngine::running()
{
    return m_bRunning;
}

void CEngine::draw(CApp* pApp)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    m_drawCode(pApp, this);
    al_flip_display();
}

ALLEGRO_EVENT_QUEUE* CEngine::getEventQueue()
{
    return m_event_queue;
}
