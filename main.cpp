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

#include <iostream>
using namespace std;

#include "App.hpp"
#include "Utility.hpp"
#include "TheRandom.hpp"

#include <random>

void go();

void configure(CApp& app);
void deconfigure(CApp& app);

int main()
{

    printText("start.");
    go();
    printText("end.");

    return 0;
}

void configure(CApp& app)
{

    std::vector<std::string> toPreserve;

    uint32_t nIndex = 0;
    while (environ[nIndex] != nullptr)
    {
        toPreserve.push_back(std::string(environ[nIndex]));
        ++nIndex;
    }

    app.setEnvironVars(toPreserve);

    // setting RLIMIT_CORE so core files don't get dumped.
    struct rlimit rl;
    getrlimit (RLIMIT_CORE, &rl);
    rl.rlim_cur = 0;
    setrlimit (RLIMIT_CORE, &rl);

    // setting RLIMIT_AS limits memory
    getrlimit (RLIMIT_AS, &rl);
    rl.rlim_cur = 35000000;
    setrlimit (RLIMIT_AS, &rl);

    // setting RLIMIT_CPU limits cpu time
    getrlimit (RLIMIT_CPU, &rl);
    rl.rlim_cur = 6;
    setrlimit (RLIMIT_CPU, &rl);


    // so our programs memory doesn't end up in swap area
    mlockall(MCL_FUTURE);
}

void deconfigure(CApp& app)
{
    munlockall();
}

void go()
{
    pcg_extras::seed_seq_from<std::random_device> seed_source;
    g_rng = pcg32(seed_source);

    CApp app;

    configure(app);

    app.run();

    deconfigure(app);

}
