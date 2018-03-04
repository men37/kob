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

#ifndef APP_HPP
#define APP_HPP

#include <string>
#include <vector>

#include "Keyfile.hpp"
#include "SecurePassString.hpp"
#include "MyEngine.hpp"

class CApp
{
public:
    CApp();
    ~CApp();
    int32_t run(CEngine* pEngine);
    void setEnvironVars(const std::vector<std::string>& vars);
    std::string getEnvironVar(const std::string strText);
    int32_t destroy();
    std::string& getTextToDraw();
private:
    CApp(const CApp&);
    CApp operator=(const CApp&);


    bool getInputSafe(std::string& strInput);
    void drawText(const std::string& strText);
    bool getPassFromUser(CSecurePassString& pass);
    bool getConfirmation();
    void clearScreen();
    void clearMembers();
    void cleanEnvVars();
    void restoreEnvVars();

    CEngine* m_pEngine;

    bool m_bRestoredEnvVars;

    std::string m_strTextToDraw;
    std::string m_strInput;
    CKeyFile m_key;
    std::string m_strFileName;
    std::vector<std::string> m_envVars;

};

void drawCode(CApp* pApp, CEngine* pEngine);


#endif
