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
    public:
        // Methods for writing particles
        void writeParticlePosition(Particle* p);
        void writeParticlePositions(System& s, bool write_ptype=false);

        // Method for writing observables
        void writeObs(System& s);

        // Methods for writing different types of log things
        void writeLog(const std::string& s = "");
        void writeDateTime(bool local=true);
        void writeSimulationParameters(const data_intake &);
        void writeSystemState(System& s);

        Writer() : obs("xion.obs"), xyz("xion.xyz"), log("xion.log") {
        }
    private:
        // Output file streams
        std::ofstream xyz;
        std::ofstream obs;
        std::ofstream log;

        Writer(const Writer &);
        Writer &operator=(const Writer &);

        // Utils
        void writeHeading(const std::string& heading);
        void writeKVP(const std::string &k, const std::string &v);

    };
} // Xion

#endif //ENGINE_WRITER_H
