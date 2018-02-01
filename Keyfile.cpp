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

#include "Keyfile.hpp"

#include <iostream>
#include <fstream>

using namespace std;

#include <boost/lexical_cast.hpp>

#include "Utility.hpp"

CKeyFile::CKeyFile() : Bytes(nullptr), Size(0)
{
    void* mapped = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (mapped == (void*)-1)
    {
        cout << std::strerror(errno) << endl;
        cout << "mmap fail." << endl;
    }

    Bytes = (uint8_t*)mapped;
    clearBytes();
}

CKeyFile::~CKeyFile()
{
    clearBytes();
    Size = 0;
}

bool CKeyFile::clearBytes()
{
    if (Bytes == nullptr)
    {
        return false;
    }

    mprotect(Bytes, PAGE_SIZE, PROT_WRITE);
    guaranteed_memset(Bytes, 0, PAGE_SIZE);
    mprotect(Bytes, PAGE_SIZE, PROT_NONE);

    return true;
}

bool CKeyFile::save(const std::string& strFilePath, CSecurePassString& pass)
{
    std::ofstream ofs(strFilePath, std::ofstream::binary);
    if (!ofs)
    {
        return false;
    }


    uint32_t nPassIndex = 0;
    uint8_t nVal = 255;
    mprotect(Bytes, PAGE_SIZE, PROT_WRITE);
    for (int32_t iii = 0; iii < Size; ++iii)
    {
        nVal = Bytes[iii] ^ pass.getDecryptedAtPos(nPassIndex);
        ofs.write((char*)&nVal, 1);
        nVal = 255;

        ++nPassIndex;
        if (nPassIndex >= pass.Size)
        {
            nPassIndex = 0;
        }
    }

    nPassIndex = 0;
    for (int32_t iii = 0; iii < Size; ++iii)
    {
        pass.overwritePos(nPassIndex);
        ++nPassIndex;
        if (nPassIndex >= pass.Size)
        {
            nPassIndex = 0;
        }
    }

    guaranteed_memset(Bytes, 0, PAGE_SIZE);

    mprotect(Bytes, PAGE_SIZE, PROT_NONE);

    ofs.close();

    return true;
}

bool CKeyFile::load(const std::string& strFilePath)
{
    std::ifstream ifs(strFilePath, std::ifstream::binary);
    if (!ifs)
    {
        return false;
    }

    ifs.seekg(0, ios::end);
    Size = ifs.tellg();

    if (Size > MaxKeyFileSize)
    {
        const int32_t nMaxKeyFileSize = MaxKeyFileSize;
        printText("File exceeds: " + boost::lexical_cast<std::string>(nMaxKeyFileSize));
        ifs.close();
        return false;
    }

    ifs.seekg(0);
    mprotect(Bytes, PAGE_SIZE, PROT_WRITE);
    ifs.read((char*)Bytes, Size);
    mprotect(Bytes, PAGE_SIZE, PROT_NONE);
    ifs.close();

    return true;
}
