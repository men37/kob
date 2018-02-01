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

#ifndef KEY_FILE_HPP
#define KEY_FILE_HPP

#include <cstdint>
#include <string>

#include "SecurePassString.hpp"


struct CKeyFile
{
    CKeyFile();
    ~CKeyFile();

    static const int32_t MaxKeyFileSize = 2048;

    uint8_t* Bytes;
    int32_t Size;

    bool clearBytes();

    bool save(const std::string& strFilePath, CSecurePassString& pass);
    bool load(const std::string& strFilePath);

private:
    CKeyFile(const CKeyFile&);
    CKeyFile operator=(const CKeyFile&);
};


#endif
