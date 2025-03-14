#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "molecule.h"
#include "molecularsystem.h"

std::vector<std::array<double, 3>> readXYZPositions(double a, const std::string &filename);

struct ThresholdResult
{
    std::string filename;
    double box, density;
    size_t numMolecules;
    double timeDirect, timeLinked;
    ThresholdResult(std::string f, double b, double d, size_t n, double tD, double tL)
        : filename(std::move(f)), box(b), density(d), numMolecules(n),
          timeDirect(tD), timeLinked(tL) {}
};

void find_thresholds(const std::string &directory)
{
    std::filesystem::path dir(directory);
    if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
    {
        std::cerr << "Error: " << directory << " not found or not a directory.\n";
        return;
    }
    std::vector<ThresholdResult> results;
    std::regex pattern("^box([0-9\\.]+)-density([0-9\\.]+)-positions\\.xyz$");

    for (const auto &entry : std::filesystem::directory_iterator(dir))
    {
        if (!std::filesystem::is_regular_file(entry.path()))
            continue;

        std::string fname = entry.path().filename().string();
        std::smatch match;

        if (!std::regex_match(fname, match, pattern))
            continue;

        double box_val = std::stod(match[1].str());
        double density_val = std::stod(match[2].str());
        auto posData = readXYZPositions(box_val, entry.path().string());

        MolecularSystem system(box_val);
        for (size_t i = 0; i < posData.size(); ++i)
            system.add_molecule(Molecule(static_cast<int>(i),
                                         posData[i][0], posData[i][1], posData[i][2],
                                         0.0, 0.0, 0.0));

        auto start = std::chrono::high_resolution_clock::now();
        double E_direct = system.total_potential_energy();
        auto end = std::chrono::high_resolution_clock::now();
        double tDirect = std::chrono::duration<double, std::milli>(end - start).count();
        start = std::chrono::high_resolution_clock::now();
        double E_linked = system.total_potential_energy_LinkedCells();
        end = std::chrono::high_resolution_clock::now();
        double tLinked = std::chrono::duration<double, std::milli>(end - start).count();
        results.push_back(ThresholdResult(fname, box_val, density_val,
                                          system.get_molecules().size(), tDirect, tLinked));
    }
    const ThresholdResult *best = nullptr;
    for (const auto &r : results)
        if (r.timeLinked < r.timeDirect && (!best || r.numMolecules < best->numMolecules))
            best = &r;
    if (best)
        std::cout << "Use linked cells for box_size = " << best->box
                  << " and density = " << best->density
                  << " (num molecules = " << best->numMolecules << ").\n";
    else
        std::cout << "Linked cells never outperformed direct iteration.\n";
}

int main()
{
    const std::string dir_name = "positions_files";
    if (!std::filesystem::exists(dir_name) && !std::filesystem::create_directory(dir_name))
    {
        std::cerr << "Error creating directory\n";
        return 1;
    }
    std::vector<double> a_vals = {6.0, 8.0, 9.0, 10.0, 11.0, 12.0, 15.0, 20.0};
    std::vector<double> rho_vals = {0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0, 1.2};

    for (double a : a_vals)
        for (double rho : rho_vals)
        {
            std::string filename = dir_name + "/box" + std::to_string(static_cast<int>(a)) +
                                   "-density" + std::to_string(rho) + "-positions.xyz";
            std::string command = "./genxyz " + std::to_string(a) + " " +
                                  std::to_string(rho) + " " + filename;
            std::cout << "Executing: " << command << "\n";

            if (std::system(command.c_str()) != 0)
                std::cerr << "Error executing: " << command << "\n";
        }
    find_thresholds(dir_name);
    return 0;
}
