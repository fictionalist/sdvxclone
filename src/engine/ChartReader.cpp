#include "ChartReader.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include <optional>

#include "Logging.hpp"

bool ChartReader::init() {
    return true;
}

std::optional<ChartData*> ChartReader::readFromFile(std::string path) {
    std::ifstream file(path.c_str());
    if (!file.good()) {
        Logging::error("Failed to open chart \"%s\".", path.c_str());
        return std::nullopt;
    }

    Logging::info("Reading chart file \"%s\"...", path.c_str());

    std::vector<std::string> fileLines;

    while (file.good()) {
        std::string buf;
        std::getline(file, buf);
        fileLines.push_back(buf);
    }

    Logging::info("Parsing chart file \"%s\"...", path.c_str());

    std::vector<std::string>::iterator chartStartLine;

    ChartData* data = new ChartData();

    for (auto it = fileLines.begin(); it != fileLines.end(); it++) {
        size_t separatorPos = it->find('=');
        if (separatorPos == std::string::npos && it->find("--") != std::string::npos) {
            chartStartLine = it;
            break;
        }
        std::string key = it->substr(0, separatorPos);
        std::string value = it->substr(separatorPos + 1);
        data->metadata.insert(std::make_pair(key, value));
    }

    Measure* currentMeasure = nullptr;

    for (auto it = chartStartLine; it != fileLines.end(); it++) {
        if (it->compare("--") == 0) { // new measure
            if (currentMeasure != nullptr) {
                data->measures.push_back(currentMeasure);
            }
            currentMeasure = new Measure();
            continue;
        }
        if (it->find('=') != std::string::npos) { // modifier
            
            continue;
        }

        Beat* beat = new Beat();
        const char* line = it->c_str();

        beat->A   = (line[0] != '0') ? ((line[0] == '1') ? NoteType::Chip : NoteType::Hold) : NoteType::NoNote;
        beat->B   = (line[1] != '0') ? ((line[1] == '1') ? NoteType::Chip : NoteType::Hold) : NoteType::NoNote;
        beat->C   = (line[2] != '0') ? ((line[2] == '1') ? NoteType::Chip : NoteType::Hold) : NoteType::NoNote;
        beat->D   = (line[3] != '0') ? ((line[3] == '1') ? NoteType::Chip : NoteType::Hold) : NoteType::NoNote;

        beat->FXL = (line[5] != '0') ? ((line[5] == '2') ? NoteType::Chip : NoteType::Hold) : NoteType::NoNote;
        beat->FXR = (line[6] != '0') ? ((line[6] == '2') ? NoteType::Chip : NoteType::Hold) : NoteType::NoNote;

        currentMeasure->beats.push_back(beat);
    }

    return data;
}
//TODO: currently leaking memory