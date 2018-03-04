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

#ifndef THE_RANDOM_HPP
#define THE_RANDOM_HPP

#include "pcg_random.hpp"

#include <cstdint>
#include <vector>

extern pcg32 g_rng;

uint8_t randomUInt8InRange(const uint8_t nMin, const uint8_t nMax);
uint32_t randomUInt32InRange(const uint32_t nMin, const uint32_t nMax);

bool randomBool();

bool testRandom(const uint32_t nIters = 500);

#endif
