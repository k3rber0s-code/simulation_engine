#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"
#include "Writer.h"


int main() {

    Xion::System s;

    s.addPType("H", 1.0, 1.0, Xion::Charge::positive);
    s.addPType("A", 2.7, 4.92, Xion::Charge::negative);
    s.addPType("HA", 2.7, 3.0, Xion::Charge::zero);

    std::map<Xion::PTypeID, int> reaction1;
    reaction1["HA"] = -1;
    reaction1["H"] = 1;
    reaction1["A"] = 1;
    s.addReaction(reaction1, 1);

    for (size_t i = 0; i < 3; ++i) {
        s.addParticle("H");
        s.addParticle("HA");
        s.addParticle("A");
    }

    Xion::Writer w;
    w.writeParticlePositions(s);
    for (int i = 0; i < 10000; ++i) {
        s.doMonteCarloStep();
        w.writeObs(s);
        w.writeParticlePositions(s, true);
    }
    return 0;
}
