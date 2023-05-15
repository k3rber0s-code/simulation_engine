//
// Created by ema on 8.5.23.
//

#include "Writer.h"

namespace Xion {
    void Writer::writeParticlePosition(Particle *p) {
        xyz << p->id << " " << p->coordinates.x << " " << p->coordinates.y << " " << p->coordinates.z << std::endl;
    }


    void Writer::writeObs(System &s) {
        for (auto &&[type, ids]: s.PByType) {
            obs << type << ": " << ids.size() << " ";
        }
        obs << s.energy << std::endl;
    }

    void Writer::writeParticlePositions(System &s, bool write_ptype) {
        xyz << "%" << std::endl;
        for (auto &&[type, ids]: s.PByType) {
            for (auto &&pid: ids) {
                if (write_ptype) {
                    xyz << type << " ";
                    writeParticlePosition(&(s.Particles[pid]));
                }
            }
        }
    }

    void Writer::writeLog(std::string s) {
        log << s << std::endl;
    }

    void Writer::writeDateTime(bool local) {
        time_t now = time(0);
        // convert to string
        char *dt = ctime(&now);
        if (local) {
            log << "The local date and time is: " << dt << std::endl;
        }
        else {
            // convert now to tm struct for UTC
            tm *gmtm = gmtime(&now);
            dt = asctime(gmtm);
            log << "The UTC date and time is: " << dt << std::endl;
        }

    }

    void Writer::writeSimulationParameters(const data_intake & data) {
        const auto [par_reactions, par_particle_types, par_particle_counts, par_system, par_simulation] = data;

        writeHeading("# SYSTEM");
        for (const auto & [k, v] : par_system) {
            writeKVP(k, v);
        }
        writeLog();

        writeHeading("# SIMULATION");
        for (const auto & [k, v] : par_simulation ){
            writeKVP(k, v);
        }
        writeLog();

        writeHeading("# PARTICLES");
        for (auto && pt : par_particle_types ) {
            writeHeading("## PT");
            for (const auto & [k, v] : pt) {
                writeKVP(k, v);
            }
        }
        writeLog();

        writeHeading("# COUNTS");
        for (const auto & [k, v] : par_particle_counts ){
            writeKVP(k, v);
        }
        writeLog();

        writeHeading("# REACTIONS");
        for (auto && r : par_reactions ) {
            writeHeading("## R");
            for (const auto & [k, v] : r) {
                writeKVP(k, v);
            }
        }
    }

    void Writer::writeKVP(const std::string &k, const std::string &v) { log << k << "=" << v << std::endl; }

    void Writer::writeHeading(const std::string& heading) { log << heading << std::endl; }

    void Writer::writeSystemState(System &s) {
        log << s.current_state << std::endl;
    }
} // Xion