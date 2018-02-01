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

#include "SecurePassString.hpp"

#include "TheRandom.hpp"

#include "Utility.hpp"

CSecurePassString::CSecurePassString() : Size(0), m_buffer(nullptr), m_undos(nullptr)
{
    init();
}

CSecurePassString::~CSecurePassString()
{
    deInit();
}


bool CSecurePassString::init()
{
    void* mapped = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (mapped == (void*)-1)
    {
        printText(std::strerror(errno));
        printText("mmap fail.");

        return false;
    }

    m_buffer = (uint8_t*)mapped;

    void* mappedB = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (mappedB == (void*)-1)
    {
        printText(std::strerror(errno));
        printText("mmapB fail.");

        return false;
    }

    m_undos = (uint8_t*)mappedB;

    mprotect(m_buffer, PAGE_SIZE, PROT_WRITE);
    mprotect(m_undos, PAGE_SIZE, PROT_WRITE);
    guaranteed_memset(m_buffer, 0, 256);
    guaranteed_memset(m_undos, 0, 256);
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
    mprotect(m_undos, PAGE_SIZE, PROT_NONE);

    return true;
}

bool CSecurePassString::deInit()
{
    if (m_buffer == nullptr)
    {
        return false;
    }

    mprotect(m_buffer, PAGE_SIZE, PROT_WRITE);
    mprotect(m_undos, PAGE_SIZE, PROT_WRITE);
    guaranteed_memset(m_buffer, 0, 256);
    guaranteed_memset(m_undos, 0, 256);
    munmap(m_buffer, PAGE_SIZE);
    munmap(m_undos, PAGE_SIZE);
    m_buffer = nullptr;
    m_undos = nullptr;
    Size = 0;

    return true;
}

void CSecurePassString::putAtPosAndEncrypt(const uint8_t nVal, const uint32_t nIndex)
{
    mprotect(m_buffer, PAGE_SIZE, PROT_WRITE);
    mprotect(m_undos, PAGE_SIZE, PROT_WRITE);
    m_undos[nIndex] = randomUInt8InRange(0, 255);
    m_buffer[nIndex] = nVal ^ m_undos[nIndex];
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
    mprotect(m_undos, PAGE_SIZE, PROT_NONE);
}

uint8_t CSecurePassString::getDecryptedAtPos(const uint32_t nIndex)
{
    mprotect(m_buffer, PAGE_SIZE, PROT_READ);
    mprotect(m_undos, PAGE_SIZE, PROT_READ);
    uint8_t nVal = m_buffer[nIndex] ^ m_undos[nIndex];
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
    mprotect(m_undos, PAGE_SIZE, PROT_NONE);
    return nVal;
}

void CSecurePassString::overwritePos(const uint32_t nIndex)
{
    mprotect(m_buffer, PAGE_SIZE, PROT_WRITE);
    mprotect(m_undos, PAGE_SIZE, PROT_WRITE);
    m_buffer[nIndex] = 0;
    m_undos[nIndex] = 0;
    mprotect(m_buffer, PAGE_SIZE, PROT_NONE);
    mprotect(m_undos, PAGE_SIZE, PROT_NONE);
}
