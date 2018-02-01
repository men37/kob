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

#ifndef RANDOM_CIPHER_LAN
#define RANDOM_CIPHER_LAN

#include <cstdint>
#include <string>

const uint32_t g_nSingleDigitNums = 10;
const uint32_t g_nLowercaseLetters = 26;

// LAN =  LowerAlpha and Numeric
// note the layout is numbers then letter like (ascii table).
class CRandomCipherLAN
{
public:
    CRandomCipherLAN();
    ~CRandomCipherLAN();

    bool init();
    bool deInit();

    uint8_t valueForCharIndex(const uint8_t nIndex);
    uint8_t indexFor(const uint8_t nVal);

    void getReAssignmentTable(std::string& strTable);

private:
    CRandomCipherLAN(const CRandomCipherLAN&);
    CRandomCipherLAN operator=(const CRandomCipherLAN&);
    uint8_t* m_buffer;
};

#endif
