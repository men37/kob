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

#include "App.hpp"

#include "Utility.hpp"

#include "RandomCipherLan.hpp"

#include "TheRandom.hpp"

#include "StringOps.hpp"

#include <unistd.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


CApp::CApp() : m_pEngine(nullptr), m_bRestoredEnvVars(true)
{

}

CApp::~CApp()
{
    clearMembers();
    restoreEnvVars();
}


int32_t CApp::run(CEngine* pEngine)
{

    m_pEngine = pEngine;

    cleanEnvVars();

    clearScreen();

    printText("Be advised the file size limit is 2048 bytes.");

    printText("pick an option by letter: ");
    printText("(s)et file name, (c)ancel");
    if (!getInputSafe(m_strInput))
    {
        clearMembers();
        clearScreen();
        return 0;
    }


    if (m_strInput[0] == 's')
    {

        printText("set file name now: ");

        if (!getInputSafe(m_strInput))
        {
            clearMembers();
            clearScreen();
            return 0;
        }

        m_strFileName = m_strInput;
        printText("loading file..");
        if(!loadKeyFile(m_key, m_strFileName))
        {
            clearMembers();
            clearScreen();
            printText("error loading file.");
            return 0;
        }
    }
    else if (m_strInput[0] == 'c')
    {
        clearMembers();
        clearScreen();
        printText("user abort.");
        return 0;
    }
    else
    {
        clearMembers();
        clearScreen();
        printText("unrecognized option.. aborting.");
        return 0;
    }

    printText("pick an option by letter: ");
    printText("(o)bfuscate, (d)eobfuscate, (c)ancel");
    if (!getInputSafe(m_strInput))
    {
        clearMembers();
        clearScreen();
        return 0;
    }

    if (m_strInput[0] == 'o')
    {
        CSecurePassString pass;
        const bool bInput = getPassFromUser(pass);
        if (!bInput)
        {
            clearMembers();
            clearScreen();
            printText("user did input incorrectly.");
            return 0;
        }

        const bool bConfirmed = getConfirmation();
        if (!bConfirmed)
        {
            clearMembers();
            clearScreen();
            printText("user did not confirm obfuscation.");
            return 0;
        }

        const bool bResult = obfuscateAndSaveKF(m_key, pass, m_strFileName);
        pass.deInit();

        if (!bResult)
        {
            clearMembers();
            clearScreen();
            printText("error occurred in opening file to save.");
            return 0;
        }
    }
    else if (m_strInput[0] == 'd')
    {
        CSecurePassString pass;
        const bool bInput = getPassFromUser(pass);
        if (!bInput)
        {
            clearMembers();
            clearScreen();
            printText("user did input incorrectly.");
            return 0;
        }

        const bool bConfirmed = getConfirmation();
        if (!bConfirmed)
        {
            clearMembers();
            clearScreen();
            printText("user did not confirm deobfuscation.");
            return 0;
        }
        const bool bResult = deobfuscateAndSaveKF(m_key, pass, m_strFileName);
        pass.deInit();
        if (!bResult)
        {
            clearMembers();
            clearScreen();
            printText("error occurred in opening file to save.");
            return 0;
        }
    }
    else if (m_strInput[0] == 'c')
    {
        clearMembers();
        clearScreen();
        printText("user abort.");
        return 0;
    }
    else
    {
        clearMembers();
        clearScreen();
        printText("unrecognized option.. aborting.");
        return 0;
    }

    return 0;
}


void CApp::setEnvironVars(const std::vector<std::string>& vars)
{
    m_envVars = vars;
}

std::string CApp::getEnvironVar(const std::string strText)
{
    for (uint32_t iii = 0; iii < m_envVars.size(); ++iii)
    {
        std::size_t nFound = std::string::npos;

        nFound = m_envVars[iii].find(strText);

        if (nFound != std::string::npos)
        {
            return m_envVars[iii];
        }
    }


    return std::string("");
}

bool CApp::getInputSafe(std::string& strInput)
{
    strInput.clear();

    bool bDone = false;

    while (!bDone)
    {
        ALLEGRO_EVENT ev;

        al_wait_for_event(m_pEngine->getEventQueue(), &ev);


        if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z)
            {
                strInput.push_back('a'+(ev.keyboard.keycode  - ALLEGRO_KEY_A));
            }
            else if (ev.keyboard.keycode >= ALLEGRO_KEY_0 && ev.keyboard.keycode <= ALLEGRO_KEY_9)
            {
                strInput.push_back('0'+(ev.keyboard.keycode - ALLEGRO_KEY_0));
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                if (strInput.size() > 0)
                {
                    strInput.erase(strInput.begin() + (strInput.size() - 1));
                }
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                bDone = true;
                break;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_FULLSTOP)
            {
                strInput.push_back('.');
            }
        }
    }

    if (strInput.size() >= 250)
    {
        overwriteStr(strInput);

        printText("Hazardous Input Text Detected.");
        strInput.clear();

        return false;
    }

    if (badInput(strInput))
    {
        overwriteStr(strInput);

        printText("Hazardous Input Text Detected.");
        strInput.clear();

        return false;
    }

    return true;
}

void CApp::drawText(const std::string& strText)
{
    m_strTextToDraw = strText;
    m_pEngine->draw(this);
    overwriteStr(m_strTextToDraw);
}

bool CApp::getPassFromUser(CSecurePassString& pass)
{

    printText("Input letters and/or numbers, input period (punctuation) symbol when finished");


    std::string strInput;
    std::string got = strInput;
    uint32_t nPassIndex = 0;
    while(got != ".")
    {
        overwriteStr(got, 1);
        got.clear();

        CRandomCipherLAN lan;
        if (!lan.init())
        {
            return false;
        }

        std::string strTable;

        lan.getReAssignmentTable(strTable);
        drawText(strTable);

        overwriteStr(strTable);
        strTable.clear();

        // Clear first
        overwriteStr(strInput, 1);


        if (!getInputSafe(strInput))
        {
            clearMembers();
            return false;
        }

        got = strInput;

        if (got[0] == '.')
        {
            lan.deInit();
            overwriteStr(strTable);
            strTable.clear();
            got.clear();
            strInput.clear();
            clearScreen();
            break;
        }

        if (!allNumeric(got))
        {
            lan.deInit();
            overwriteStr(strTable);
            strTable.clear();
            got.clear();
            strInput.clear();
            printText("Input error. Use numeric values only!");
            clearScreen();
            return false;
        }

        const std::string strSrcTable = "0123456789abcdefghijklmnopqrstuvwxyz ";
        uint8_t nTrueValue = lan.indexFor(boost::lexical_cast<uint32_t>(got));
        if (nTrueValue == 255)
        {
            lan.deInit();
            overwriteStr(strTable);
            strTable.clear();
            got.clear();
            strInput.clear();
            printText("Input error. Chosen numeric value was invaild!");
            clearScreen();
            return false;
        }

        pass.putAtPosAndEncrypt(strSrcTable[nTrueValue], nPassIndex);
        pass.Size += 1;
        ++nPassIndex;


        nTrueValue = 0;
        lan.deInit();

        strTable.clear();


        clearScreen();

    }

    // Clear first
    overwriteStr(strInput, 1);

    clearScreen();
    //printText(strPass);

    return true;
}

bool CApp::getConfirmation()
{
    std::string strConfirmText;
    for (uint32_t iii = 0; iii < 6; ++iii)
    {
        strConfirmText.push_back('0'+randomUInt32InRange(0,9));
    }

    std::string strShow;
    strShow.append("To confirm operation please enter the numbers: ");
    strShow.append(strConfirmText);
    drawText(strShow);
    std::string strInput;
    getInputSafe(strInput);

    if (strInput != strConfirmText)
    {
        overwriteStr(strConfirmText, 1);
        strConfirmText.clear();
        overwriteStr(strInput, 1);
        strInput.clear();
        return false;
    }

    overwriteStr(strShow);
    strShow.clear();
    overwriteStr(strConfirmText, 1);
    strConfirmText.clear();
    overwriteStr(strInput, 1);
    strInput.clear();


    clearScreen();

    return true;
}

void CApp::clearScreen()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void CApp::clearMembers()
{
    overwriteStr(m_strInput, 1);
    m_strInput.clear();
    overwriteStr(m_strFileName, 1);
    m_strFileName.clear();
    m_key.clearBytes();
    overwriteStr(m_strTextToDraw);
}

void CApp::cleanEnvVars()
{
    std::vector<std::string> toKeep ;
    toKeep.push_back(std::string("SHELL"));
    toKeep.push_back(std::string("TERM"));
    toKeep.push_back(std::string("PATH"));
    toKeep.push_back(std::string("DISPLAY"));
    toKeep.push_back(std::string("LD_LIBRARY_PATH"));

    for (uint32_t iii = 0; iii < m_envVars.size(); ++iii)
    {
        std::vector<std::string> strs;
        boost::split(strs, m_envVars[iii], boost::is_any_of("="));

        if (strs.size() < 2)
        {
            continue;
        }

        std::string str = strs[0];
        setenv(str.c_str(), "", 1);

        for (uint32_t jjj = 0; jjj < toKeep.size(); ++jjj)
        {
            if (toKeep[jjj] == str)
            {
                setenv(str.c_str(), strs[1].c_str(), 1);
                break;
            }
        }
    }

    if (getenv("TERM") == nullptr)
    {
        setenv("TERM", "xterm", 1);
    }

    m_bRestoredEnvVars = false;
}

void CApp::restoreEnvVars()
{
    if (m_bRestoredEnvVars)
    {
        return;
    }

    for (uint32_t iii = 0; iii < m_envVars.size(); ++iii)
    {
        std::vector<std::string> strs;
        boost::split(strs, m_envVars[iii], boost::is_any_of("="));

        if (strs.size() < 2)
        {
            continue;
        }

        std::string str = strs[0];
        setenv(str.c_str(), strs[1].c_str(), 1);

    }

    m_bRestoredEnvVars = true;
}

int32_t CApp::destroy()
{

    clearMembers();
    clearScreen();
    restoreEnvVars();

    overwriteStr(m_strTextToDraw);

    return 0;
}

std::string& CApp::getTextToDraw()
{
    return m_strTextToDraw;
}


void drawCode(CApp* pApp, CEngine* pEngine)
{
    std::string strText;
    strText.append(pApp->getTextToDraw());

    std::vector<std::string> texts = split(strText, '\n');
    for (uint32_t iii = 0; iii < texts.size(); ++iii)
    {
        al_draw_text(pEngine->getDefaultFont(), al_map_rgb(255, 0, 0), 0, 32 * iii, 0, texts[iii].c_str());
    }


}
