//
// Created by ema on 8.5.23.
//

#include "Writer.h"

namespace Xion {
    void Writer::writeParticlePosition(Particle *p) {
        xyz << p->id << " " << p->coordinates.x << " " << p->coordinates.y << " " << p->coordinates.z << std::endl;
    }

    void Writer::writeParticlePosition(Particle *p, std::string pTypeID) {
        xyz << pTypeID << p->id << " " << p->coordinates.x << " " << p->coordinates.y << " " << p->coordinates.z
            << std::endl;
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
} // Xion