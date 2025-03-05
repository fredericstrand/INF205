#include <iostream>
#include <sys/stat.h>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include <regex>
#include <fstream>
#include <chrono>

#include "molecule.h"
#include "molecularsystem.h"

// Function to read .xyz files (ensure this matches the declaration in readxyz.h)
std::vector<std::array<double, 3>> readXYZ(const std::string &filename);

// Structure to store results
struct ThresholdResult
{
    std::string filename;
    double box_val;
    double density_val;
    size_t numMolecules;
    double timeDirect;
    double timeLinked;

    // Constructor
    ThresholdResult(std::string f, double b, double d, size_t n, double tD, double tL)
        : filename(std::move(f)), box_val(b), density_val(d), numMolecules(n),
          timeDirect(tD), timeLinked(tL) {}
};

// Function to find thresholds and process files
void find_thresholds(const std::string &directory)
{
    // Use standard filesystem
    std::filesystem::path file_path(directory);
    if (!std::filesystem::exists(file_path) || !std::filesystem::is_directory(file_path))
    {
        std::cerr << "Error: " << directory << " not found or not a directory.\n";
        return;
    }

    std::vector<ThresholdResult> results;

    // Regex pattern: e.g. "box20-density0.8-positions.xyz"
    std::regex pattern("^box([0-9\\.]+)-density([0-9\\.]+)-positions\\.xyz$");

    for (const auto &entry : std::filesystem::directory_iterator(file_path))
    {
        if (std::filesystem::is_regular_file(entry.path()))
        {
            const std::string filename = entry.path().filename().string();
            std::smatch match;
            if (std::regex_match(filename, match, pattern))
            {
                // Extract values from filename
                double box_val = std::stod(match[1].str());
                double density_val = std::stod(match[2].str());

                // Read .xyz content
                std::string full_path = entry.path().string();
                auto posData = readXYZ(full_path);

                // Build MolecularSystem
                MolecularSystem system(box_val);
                for (size_t i = 0; i < posData.size(); ++i)
                {
                    // Provide all 7 parameters to Molecule
                    system.addMolecule(Molecule(static_cast<int>(i),
                                                posData[i][0], posData[i][1], posData[i][2],
                                                0.0, 0.0, 0.0));
                }

                // Measure Direct potential energy computation
                auto start = std::chrono::high_resolution_clock::now();
                [[maybe_unused]] double E_direct = system.totalPotentialEnergy();
                auto end = std::chrono::high_resolution_clock::now();
                double time_direct_ms = std::chrono::duration<double, std::milli>(end - start).count();

                // Measure Linked Cells potential energy computation
                start = std::chrono::high_resolution_clock::now();
                [[maybe_unused]] double E_linked = system.totalPotentialEnergyLinkedCells();
                end = std::chrono::high_resolution_clock::now();
                double time_linked_ms = std::chrono::duration<double, std::milli>(end - start).count();

                // Use explicit constructor for ThresholdResult
                results.push_back(ThresholdResult(filename, box_val, density_val,
                                                  system.getMolecules().size(), time_direct_ms, time_linked_ms));
            }
        }
    }

    // Find smallest threshold where linked cells is faster
    size_t bestThreshold = std::numeric_limits<size_t>::max();
    for (const auto &r : results)
    {
        if (r.timeLinked < r.timeDirect && r.numMolecules < bestThreshold)
        {
            bestThreshold = r.numMolecules;
        }
    }

    if (bestThreshold == std::numeric_limits<size_t>::max())
    {
        std::cout << "Linked cells never outperformed direct iteration.\n";
    }
    else
    {
        std::cout << "Use linked cells at number molecules >= " << bestThreshold << "\n";
    }
}

int main()
{
    // Create directory if it doesn't exist
    const char *dir_name = "positions_files";
    struct stat info;
    if (stat(dir_name, &info) != 0)
    {
        if (mkdir(dir_name, 0777) == -1)
        {
            std::cerr << "Error creating directory" << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Directory created" << std::endl;
        }
    }

    // Define parameters
    std::map<std::string, std::vector<double>> parameters = {
        {"a", {6.0, 9.0, 10.0, 12.0, 15.0, 20.0}},
        {"rho", {0.4, 0.6, 0.8, 1.0, 1.2}}};

    // Generate .xyz files
    for (double a : parameters["a"])
    {
        for (double rho : parameters["rho"])
        {
            std::string filename = std::string(dir_name) + "/box" +
                                   std::to_string(static_cast<int>(a)) + "-density" +
                                   std::to_string(rho) + "-positions.xyz";

            std::string command = "./genxyz " + std::to_string(a) + " " +
                                  std::to_string(rho) + " " + filename;

            std::cout << "Executing: " << command << std::endl;
            int result = std::system(command.c_str());
            if (result != 0)
            {
                std::cerr << "Error executing command: " << command << std::endl;
            }
        }
    }

    // Compute thresholds
    find_thresholds("positions_files");

    return 0;
}