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

#include "RandomCipherLan.hpp"

#include "TheRandom.hpp"

#include "Utility.hpp"

#include <vector>
#include <boost/lexical_cast.hpp>

CRandomCipherLAN::CRandomCipherLAN() : m_buffer(nullptr)
{
    void* mapped = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (mapped == (void*)-1)
    {
        printText(std::strerror(errno));
        printText("mmap fail.");
    }

    m_buffer = (uint8_t*)mapped;
}

CRandomCipherLAN::~CRandomCipherLAN()
{
    deInit();
}

void CRandomCipherLAN::init()
{
    //deInit();

    const uint32_t nBuffSize = g_nSingleDigitNums + g_nLowercaseLetters + 1;
    std::vector<uint8_t> toUse;

    for (uint32_t iii = 0; iii < 256; ++iii)
    {
        toUse.push_back(iii);
    }

    mprotect(m_buffer, PAGE_SIZE, PROT_WRITE);
    uint32_t nIndex = 0;
    while(toUse.size() > 0)
    {
        const uint32_t nToAssign = randomUInt32InRange(0, toUse.size()-1);
        m_buffer[nIndex] = toUse[nToAssign];
        toUse.erase(toUse.begin() + nToAssign);
        ++nIndex;
        if (nIndex >= nBuffSize)
        {
            break;
        }
    }
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);

}

void CRandomCipherLAN::deInit()
{
    if (m_buffer == nullptr)
    {
        return;
    }
    const uint32_t nBuffSize = g_nSingleDigitNums + g_nLowercaseLetters + 1;
    mprotect(m_buffer, PAGE_SIZE, PROT_WRITE);
    guaranteed_memset(m_buffer, 0, nBuffSize);
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
    munmap(m_buffer, PAGE_SIZE);
    m_buffer = nullptr;
}

uint8_t CRandomCipherLAN::valueForCharIndex(const uint8_t nIndex)
{
    uint8_t nVal = 255;
    mprotect(m_buffer, PAGE_SIZE, PROT_READ);
    if (nIndex >= 'a' && nIndex <= 'z')
    {
        nVal = m_buffer[10 + (nIndex - 'a')];
    }
    else if (nIndex >= '0' && nIndex <= '9')
    {
        nVal = m_buffer[nIndex - '0'];
    }
    else if (nIndex == ' ')
    {
        nVal = m_buffer[36];
    }
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
    return nVal;
}

uint8_t CRandomCipherLAN::indexFor(const uint8_t nVal)
{
    mprotect(m_buffer, PAGE_SIZE, PROT_READ);
    const uint32_t nBuffSize = g_nSingleDigitNums+g_nLowercaseLetters+1;

    for (uint32_t iii = 0; iii < nBuffSize; ++iii)
    {
        if (m_buffer[iii] == nVal)
        {

            mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
            return iii;
        }
    }


    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);

    return 255;
}

void CRandomCipherLAN::getReAssignmentTable(std::string& strTable)
{

    const std::string strSrcTable = "0123456789abcdefghijklmnopqrstuvwxyz ";
    const uint32_t nHalfTableSize = strSrcTable.size() / 2;
    std::vector<std::string> table;
    for (uint32_t iii = 0; iii < strSrcTable.size(); ++iii)
    {
        std::string strItem;
        strItem.push_back(strSrcTable[iii]);
        strItem.push_back('=');
        strItem.append(boost::lexical_cast<std::string>((uint32_t)valueForCharIndex(strSrcTable[iii])));

        if (iii > nHalfTableSize)
        {
            table[iii - nHalfTableSize].push_back('\t');
            table[iii - nHalfTableSize].push_back('\t');
            table[iii - nHalfTableSize].append(strItem);
            overwriteStr(strItem);
            continue;
        }

        table.push_back(strItem);
        overwriteStr(strItem);
    }

    for (uint32_t iii = 0; iii < table.size(); ++iii)
    {
        strTable += table[iii];
        strTable.push_back('\n');
        overwriteStr(table[iii]);
    }

    table.clear();

}
