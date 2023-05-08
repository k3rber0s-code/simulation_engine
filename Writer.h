//
// Created by ema on 8.5.23.
//

#include <iostream>
#include <fstream>
#include "Particle.h"
#include "System.h"

#ifndef ENGINE_WRITER_H
#define ENGINE_WRITER_H

namespace Xion {
    /*
    * Writer has three output streams: one for storing the quantities we observe (obs),
    * another for storing particle positions (xyz, mainly for later visualizations)
    * and the last one for saving the details about the experiment (log).
    */
    class Writer {
    private:
        std::ofstream xyz;
        std::ofstream obs;
        std::ofstream log;

        Writer(const Writer &);

        Writer &operator=(const Writer &);

    public:
        void writeParticlePosition(Particle* p);
        void writeParticlePosition(Particle* p, std::string pTypeID);
        void writeParticlePositions(System& s, bool write_ptype=false);

        void writeObs(System& s);

        void writeLog(std::string);

        Writer() : obs("xion.obs"), xyz("xion.xyz"), log("xion.log") {
        }

    };
} // Xion

#endif //ENGINE_WRITER_H
