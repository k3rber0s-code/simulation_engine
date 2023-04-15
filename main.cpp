#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include "Particle.h"
#include "Interaction.h"
#include "System.h"




int main() {

    auto p1 = Xion::ChainedParticle();
    auto p2 = Xion::ChainedParticle();

    auto i1 = std::make_shared<Xion::LennardJonesPotential>();

    p1.addInteraction(i1);
    p2.addInteraction(i1);

    Xion::System s;
    s.addParticle("Ar");
    s.addParticle("Ar");

    s.getParticleByID(1)->printPosition();

    std::cout << s.generateID() << " " << s.generateID();

    return 0;
}
