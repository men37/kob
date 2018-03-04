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

#include "TheRandom.hpp"

#include "TheDefines.hpp"

#include <random>

pcg32 g_rng;


uint8_t randomUInt8InRange(const uint8_t nMin, const uint8_t nMax)
{
    std::uniform_int_distribution<uint8_t> index_dist(nMin, nMax);

    return index_dist(g_rng);
}

uint32_t randomUInt32InRange(const uint32_t nMin, const uint32_t nMax)
{
    std::uniform_int_distribution<uint32_t> index_dist(nMin, nMax);

    return index_dist(g_rng);
}

bool randomBool()
{
    std::uniform_int_distribution<uint8_t> index_dist(0, 1);
    return index_dist(g_rng);
}

bool testRandom(const uint32_t nIters)
{

  bool bLastVal = false;
  uint32_t nLastWasOpposite = 0;
  uint32_t nLastNotOpposite = 0;
  for (uint32_t iii = 0; iii < nIters; ++iii)
  {
    if (iii == 0)
    {
      bLastVal = randomBool();
      continue;
    }

    const bool bGot = randomBool();
    if (bGot == bLastVal)
    {
      ++nLastNotOpposite;
    }
    else
    {
      ++nLastWasOpposite;
    }

    bLastVal = bGot;
  }

  const uint32_t nUnacceptable = nIters - 3;
  if (nLastWasOpposite >= nUnacceptable)
  {
    // outputting a predictable pattern. probably no true support for random_device?
    return false;
  }

  return true;
}
