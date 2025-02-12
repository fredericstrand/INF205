#include <iostream>
#include <vector>
#include <string>
#include "Molecule.h"

// Function declared in readerXYZ.cpp
std::vector<std::vector<float>> readXYZ(const std::string &filename);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " positions.xyz [velocities.xyz]\n";
        return 1;
    }

    std::string posFile = argv[1];
    std::vector<std::vector<float>> posData = readXYZ(posFile);

    std::vector<std::vector<float>> velData;
    bool hasVelocityFile = (argc > 2);
    if (hasVelocityFile) {
        std::string velFile = argv[2];
        velData = readXYZ(velFile);
    }

    std::vector<Molecule> molecules;
    molecules.reserve(posData.size());

    for (size_t i = 0; i < posData.size(); ++i) {
        float x = posData[i][0];
        float y = posData[i][1];
        float z = posData[i][2];

        float vx = 0.f, vy = 0.f, vz = 0.f;
        if (hasVelocityFile && i < velData.size()) {
            vx = velData[i][0];
            vy = velData[i][1];
            vz = velData[i][2];
        }

        Molecule mol(static_cast<int>(i), x, y, z, vx, vy, vz);
        molecules.push_back(mol);
    }

    for (const auto &m : molecules) {
        const auto& c = m.getCoordinates();
        const auto& v = m.getVelocities();
        std::cout << "Molecule " << m.getID() 
                  << ": coords=(" << c[0] << ", " << c[1] << ", " << c[2] << ")"
                  << " vels=("   << v[0] << ", " << v[1] << ", " << v[2] << ")\n";
    }

    return 0;
}
