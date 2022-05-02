#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "encoder.h"
#include "decoder.h"
#include "prompts.inc"

void Run(int argc, char* argv[]) {
    if (static_cast<std::string> (argv[1]) == "-h") {
        std::cout << PROMPT_STRING;
    } else if (static_cast<std::string> (argv[1]) == "-c") {
        std::string archive_name = static_cast<std::string>(argv[2]);
        std::vector<std::string> file_names;
        for (int i = 3; i < argc; ++i) {
            file_names.push_back(static_cast<std::string>(argv[i]));
        }
        Encoder encoder(archive_name, file_names);
        encoder.Encode();
    } else if (static_cast<std::string> (argv[1]) == "-d") {
        std::string archive_name = static_cast<std::string>(argv[2]);
        Decoder decoder(archive_name);
        decoder.Decode();
    } else {
        throw std::runtime_error("No matching function for given args");
    }
}

int main(int argc, char* argv[]) {
    try {
        Run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Exception happened: " << e.what() << std::endl;
    }
    return 0;
}