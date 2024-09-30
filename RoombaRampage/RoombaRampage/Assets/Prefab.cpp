#include "Prefab.h"
#include "../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"


namespace prefab {

	void Prefab::m_DeSerializePrefab(std::string path) {

        // Open the JSON file for reading
        std::ifstream inputFile(path);

        if (!inputFile) {
            LOGGING_ERROR("Failed to open Pefab JSON file for reading");
            return;
        }


        std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        // Parse the JSON content
        rapidjson::Document doc;
        doc.Parse(fileContent.c_str());

        for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
            const rapidjson::Value& prefabData = doc[i];

            if (prefabData.HasMember("name") && prefabData["name"].IsString()) {
               // m_prefab_Names[];  // Store the name
            }



        }

	}
}