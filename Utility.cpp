
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

#include "Utility.hpp"
#include "TheRandom.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

void* guaranteed_memset(void* v, int c, size_t n)
{
    volatile char* p = static_cast<volatile char*>(v);
    while (n--)
    {
        *p = c;
        ++p;
    }
    return v;
}

bool getInputSafe(std::string& strInput)
{
    std::getline(std::cin, strInput);

    if (strInput.size() >= 250)
    {
      overwriteStr(strInput);

      printText("Hazardous Input Text Detected.");
      strInput.clear();

      return false;
    }

    if (badInput(strInput))
    {
      overwriteStr(strInput);

      printText("Hazardous Input Text Detected.");
      strInput.clear();

      return false;
    }

    return true;
}

bool badInput(std::string& strInput)
{
  bool bVal = false;

  std::size_t nFound = std::string::npos;

  nFound = strInput.find_first_of("/<>%()&+\'\"\\");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find_first_of("!{}[]?*;^~");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("../");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("..\\");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("/.");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("\\.");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("\r");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("\n");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("\f");

  if (nFound != std::string::npos)
  {
    return true;
  }

  nFound = strInput.find("\v");

  if (nFound != std::string::npos)
  {
    return true;
  }

  for (uint32_t iii = 0; iii < strInput.size(); ++iii)
  {
    if (strInput[iii] == '\0')
    {
      return true;
    }
  }

  return bVal;
}

bool allNumeric(const std::string& strText)
{
    for (uint32_t iii = 0; iii < strText.size(); ++iii)
    {
        if (!(strText[iii] >= '0' && strText[iii] <= '9'))
        {
            return false;
        }
    }
    return true;
}

void printText(const std::string& strText, bool bEndLine)
{
    cout << strText;
    if (bEndLine)
    {
        cout << endl;
    }
}

void overwriteStr(std::string& strText, const uint8_t nVal)
{
    for (uint32_t iii = 0; iii < strText.size(); ++iii)
    {
        strText[iii] = nVal;
    }
}

bool loadKeyFile(SKeyFile& file, const std::string& strFilePath)
{
    return file.load(strFilePath);
}

bool obfuscateAndSaveKF(SKeyFile& file, CSecurePassString& pass, const std::string& strFilePath)
{
    SKeyFile fileB;
    fileB = file;
    const bool bResult = fileB.save(strFilePath, pass);
    file.clearBytes();
    fileB.clearBytes();
    return bResult;
}

bool deobfuscateAndSaveKF(SKeyFile& file, CSecurePassString& pass, const std::string& strFilePath)
{
    SKeyFile fileB;
    fileB = file;
    const bool bResult = fileB.save(strFilePath, pass);
    file.clearBytes();
    fileB.clearBytes();
    return bResult;
}
