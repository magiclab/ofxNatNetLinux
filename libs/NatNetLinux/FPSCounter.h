/*
 * FPSCounter.h
 *
 * NatNetLinux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NatNetLinux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NatNetLinux.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <utility>
#include <time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#define BILLION 1000000000L

class FPSCounter{
public:
    FPSCounter(){
        b1stFrame=false;
        lastFps = 0;
    }
    
    void update(struct timespec newTs){
        if(b1stFrame){
            uint64_t tDiff = BILLION * (newTs.tv_sec - oldTs.tv_sec) +
            newTs.tv_nsec - oldTs.tv_nsec;
            lastFps = BILLION/(double)tDiff;
        }
        oldTs = newTs;
        b1stFrame = true;
    }
    
    double getFps(){
        return lastFps;
    }
private:
    double lastFps;
    struct timespec oldTs;
    bool b1stFrame;
};

#endif /*FPSCOUNTER_H*/