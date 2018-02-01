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


class CApp
{
public:
    CApp();
    ~CApp();
    void run();
    void setEnvironVars(const std::vector<std::string>& vars);
    std::string getEnvironVar(const std::string strText);
private:
    void getPassFromUser(CSecurePassString& pass);
    bool getConfirmation();
    void clearScreen();
    void clearMembers();
    void clearEnvVars();
    void restoreEnvVars();

    std::string m_strInput;
    SKeyFile m_key;
    std::string m_strFileName;
    std::vector<std::string> m_envVars;
};


#endif
