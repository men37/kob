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

#include <unistd.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

CApp::CApp()
{

}

CApp::~CApp()
{
    clearMembers();
    clearScreen();
    restoreEnvVars();

}


void CApp::run()
{
    printText("Be advised the file size limit is 2048 bytes.");

    printText("pick an option by letter: ");
    printText("(s)et file name, (c)ancel");
    if (!getInputSafe(m_strInput))
    {
        clearMembers();
        clearScreen();
        return;
    }


    if (m_strInput[0] == 's')
    {
        printText("set file name now: ");

        if (!getInputSafe(m_strInput))
        {
            clearMembers();
            clearScreen();
            return;
        }

        m_strFileName = m_strInput;
        printText("loading file..");
        if(!loadKeyFile(m_key, m_strFileName))
        {
            clearMembers();
            clearScreen();
            printText("error loading file.");
            return;
        }
    }
    else if (m_strInput[0] == 'c')
    {
        clearMembers();
        clearScreen();
        printText("user abort.");
        return;
    }
    else
    {
        clearMembers();
        clearScreen();
        printText("unrecognized option.. aborting.");
        return;
    }

    printText("pick an option by letter: ");
    printText("(o)bfuscate, (d)eobfuscate, (c)ancel");
    if (!getInputSafe(m_strInput))
    {
        clearMembers();
        clearScreen();
        return;
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
            return;
        }

        const bool bConfirmed = getConfirmation();
        if (!bConfirmed)
        {
            clearMembers();
            clearScreen();
            printText("user did not confirm obfuscation.");
            return;
        }

        const bool bResult = obfuscateAndSaveKF(m_key, pass, m_strFileName);
        pass.deInit();

        if (!bResult)
        {
            clearMembers();
            clearScreen();
            printText("error occurred in opening file to save.");
            return;
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
            return;
        }

        const bool bConfirmed = getConfirmation();
        if (!bConfirmed)
        {
            clearMembers();
            clearScreen();
            printText("user did not confirm deobfuscation.");
            return;
        }
        const bool bResult = deobfuscateAndSaveKF(m_key, pass, m_strFileName);
        pass.deInit();
        if (!bResult)
        {
            clearMembers();
            clearScreen();
            printText("error occurred in opening file to save.");
            return;
        }
    }
    else if (m_strInput[0] == 'c')
    {
        clearMembers();
        clearScreen();
        printText("user abort.");
        return;
    }
    else
    {
        clearMembers();
        clearScreen();
        printText("unrecognized option.. aborting.");
        return;
    }
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
        printText(strTable);
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

    printText("To confirm operation please enter the numbers: " + strConfirmText);
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

    overwriteStr(strConfirmText, 1);
    strConfirmText.clear();
    overwriteStr(strInput, 1);
    strInput.clear();

    clearScreen();

    return true;
}

void CApp::clearScreen()
{
    clearEnvVars();

    printf("\033[2J");

    std::string strExtras;
    uint32_t nTableSize = g_nLowercaseLetters+g_nSingleDigitNums+1;
    nTableSize = nTableSize * (3+2);
    for (uint32_t iii = 0; iii < nTableSize; ++iii)
    {
        strExtras.push_back('1');
    }

    printText(strExtras);

    strExtras.clear();

    printf("\033[2J");

    std::system("clear");
}

void CApp::clearMembers()
{
    overwriteStr(m_strInput, 1);
    m_strInput.clear();
    overwriteStr(m_strFileName, 1);
    m_strFileName.clear();
    m_key.clearBytes();
}

void CApp::clearEnvVars()
{
    std::vector<std::string> toKeep ;
    toKeep.push_back(std::string("SHELL"));
    toKeep.push_back(std::string("TERM"));
    toKeep.push_back(std::string("PATH"));
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
}

void CApp::restoreEnvVars()
{
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
}
