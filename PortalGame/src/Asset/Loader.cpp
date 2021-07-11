#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <sstream>
#include <filesystem>

#include "Asset/Loader.h"
#include "Core/Base.h"
#include "Renderer/Data.h"
#include "OpenGL/Types.h"

namespace PGame {
	namespace Asset {
		void LoadOBJ(const std::filesystem::path& path, std::vector<Renderer::Vertex>& outVerticies) {
			std::string data; LoadText(path, data);

			std::vector<glm::vec3> vertexPositions;
			std::vector<glm::vec3> vertexNormals;
			std::vector<glm::vec2> uvCoords;

			std::string line;
			std::string prefix;
			std::stringstream ss;
			std::stringstream dataStream(data);

			glm::vec3 tempVec3;
			int tempInt;
			Renderer::Vertex tempVertex;

			while (std::getline(dataStream, line)) {
				ss.clear();
				ss.str(line);
				ss >> prefix;

				if (prefix == "#") {
					continue;
				} 
				else if (prefix == "vt") {
					ss >> tempVec3.x >> tempVec3.y;
					uvCoords.push_back(tempVec3);
				}
				else if (prefix == "vn") {
					ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
					vertexNormals.push_back(tempVec3);
				}
				else if (prefix == "v") {
					ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
					vertexPositions.push_back(tempVec3);
				}
				else if (prefix == "f") {
					int typeCounter = 0;
					while (ss >> tempInt) {
						
						if (typeCounter == 0) {
							tempVertex.pos = vertexPositions[tempInt-1];
						} else if (typeCounter == 1) {
							tempVertex.uv = uvCoords[tempInt-1];
						} else if (typeCounter == 2) {
							tempVertex.normal = vertexNormals[tempInt-1];
						}

						if (ss.peek() == '/') {
							ss.ignore(1, '/');
							typeCounter++;
						}
						else if (ss.peek() == ' ') {
							ss.ignore(1, ' ');
							typeCounter = 0;
							outVerticies.push_back(tempVertex);
						}

					}
					outVerticies.push_back(tempVertex);
				}
				else if (prefix == "mtllib") {

				}
				else if (prefix == "usemtl")
				{

				}
				else if (prefix == "o") {

				} 
				else if (prefix == "s") {

				}
				else {
					pgWarn("Unknown token '" << prefix << "' in OBJ file");
				}
			}
			std::cout << "Vertex Pos: " << vertexPositions.size() << std::endl;
			std::cout << "Text Pos: " << uvCoords.size() << std::endl;
			std::cout << "Normal Pos: " << vertexNormals.size() << std::endl;
		}

		bool Load(const std::filesystem::path& path, std::shared_ptr<GL::Shader> outShader) {
			std::string shaderSource;
			if (LoadText(path, shaderSource) == PG_FAILURE) { return PG_FAILURE; }
			return outShader->Load(shaderSource);
		}

		bool LoadText(const std::filesystem::path& path, std::string& contents) {
			std::ifstream file(path);

			if (!file) {
				pgError("Could not open file -> " << path);
				return PG_FAILURE;
			}

			file.seekg(0, std::ios::end);
			size_t size = file.tellg();

			contents = std::string(size, ' ');

			file.seekg(0, std::ios::beg);
			file.read(&contents[0], size);

			return PG_SUCCESS;
		}
	}
}