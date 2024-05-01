#pragma once

#include <string>
#include <map>
#include <optional>
#include <vector>

enum class NoteType {
    NoNote,
    Chip,
    Hold,
    KeysoundChip,
};

enum class Modifiers {
    NoModifiers,
    BPM
};

struct Beat {
    NoteType A;
    NoteType B;
    NoteType C;
    NoteType D;
    NoteType FXL;
    NoteType FXR;

    Modifiers modifiers;
};

struct Measure {
    std::vector<Beat*> beats;
};

struct ChartData {
    std::map<std::string, std::string> metadata;
    std::vector<Measure*> measures;
};

namespace ChartReader {
    bool init();

    std::optional<ChartData*> readFromFile(std::string path);
}