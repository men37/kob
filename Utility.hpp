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

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdint>

#include <string>

#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <memory>

#include <cstdlib>

#include <sys/mman.h>
#include <sys/resource.h>

#include "Keyfile.hpp"

#include "SecurePassString.hpp"

#define PAGE_SIZE 4096

void* guaranteed_memset(void* v, int c, size_t n);

// Limit is 249 characters.
bool getInputSafe(std::string& strInput);

bool badInput(std::string& strInput);

bool allNumeric(const std::string& strText);

void printText(const std::string& strText, bool bEndLine=true);

void overwriteStr(std::string& strText, const uint8_t nVal=1);

bool loadKeyFile(CKeyFile& file, const std::string& strFilePath);

bool obfuscateAndSaveKF(CKeyFile& file, CSecurePassString& pass, const std::string& strFilePath);

bool deobfuscateAndSaveKF(CKeyFile& file, CSecurePassString& pass, const std::string& strFilePath);



#endif
