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

#include <stb/stb_image.h>

namespace Engine {
	namespace Asset {
		template<>
		bool Load<GL::Texture>(const fs::path& file, std::shared_ptr<GL::Texture> outTexture) {
			int width, height, nrChannels;
			unsigned char* data = stbi_load(file.string().c_str(), &width, &height, &nrChannels, 0);

			if (data == NULL) {
				pgError(stbi_failure_reason());
				return PG_FAILURE;
			}

			pgDebug("Loaded Image with " << nrChannels << " channels.");

			outTexture->Init(
				GL::TexType::TEX_2D,
				GL::TexUnit::U0,
				GL::TexWrapping::CLAMP_TO_EDGE,
				GL::TexFiltering::LINEAR
			);

			if (nrChannels == 4) {
				outTexture->Upload(width, height, GL_RGBA, GL_RGBA8, (GLubyte*)data);
			}
			else if (nrChannels == 3) {
				outTexture->Upload(width, height, GL_RGB, GL_RGB8, (GLubyte*)data);
			}
			else {
				pgError("Unknown Image Format with less than 3 color channels.");
				return PG_FAILURE;
			}


			return PG_SUCCESS;
		}

		template<>
		bool Load(const std::filesystem::path& path, std::shared_ptr<Renderer::Mesh> outMesh) {
			std::string data; 
			if (LoadText(path, data) == PG_FAILURE) {
				return PG_FAILURE;
			}

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
							tempVertex.Pos = vertexPositions[tempInt-1];
						} else if (typeCounter == 1) {
							tempVertex.Uv = uvCoords[tempInt-1];
						} else if (typeCounter == 2) {
							tempVertex.Normal = vertexNormals[tempInt-1];
						}

						if (ss.peek() == '/') {
							ss.ignore(1, '/');
							typeCounter++;
						}
						else if (ss.peek() == ' ') {
							ss.ignore(1, ' ');
							typeCounter = 0;
							outMesh->Vertecies.push_back(tempVertex);
						}

					}
					outMesh->Vertecies.push_back(tempVertex);
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
			return PG_SUCCESS;
		}

		template<>
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