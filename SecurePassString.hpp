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

#ifndef SECURE_STRING_HPP
#define SECURE_STRING_HPP

#include <cstdint>


class CSecurePassString
{
public:
    CSecurePassString();
    ~CSecurePassString();

    uint32_t Size;

    bool init();
    bool deInit();

    void putAtPosAndEncrypt(const uint8_t nVal, const uint32_t nIndex);
    uint8_t getDecryptedAtPos(const uint32_t nIndex);
    void overwritePos(const uint32_t nIndex);

private:
    CSecurePassString(const CSecurePassString&);
    CSecurePassString operator=(const CSecurePassString&);

    uint8_t* m_buffer;
    uint8_t* m_undos;
};

#endif
