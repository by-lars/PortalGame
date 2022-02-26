#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "Core/Base.h"
#include "Core/AnyPtr.h"

namespace Engine {

	using rID = uint32_t;

	static const rID INVALID_ID = 0;

	class ResourceRegistry {
	public:
		void _DEBUG_PRINT();

		/**
		* Gets a resource id from its path
		* and trys to load it into memory when not
		* yet loaded
		*
		* @returns Resource id to resource
		* @returns INVALID_ID if resource can't be found
		**/
		template<typename T>
		rID GetID(const std::string& path) {
			if (m_PathToID.find(path) == m_PathToID.end()) {
				rID id = CreateResource<T>(path);
				// bool didLoad = Asset::Load<T>(path, *(T*)m_Resources[id].Pointer);
				bool didLoad = false;
				if (didLoad == false) {
					DeleteResource(id);
					return INVALID_ID;
				}

				return id;
			}

			return m_PathToID[path];
		}

		/**
		* Gets a resource from its ID
		*
		* @returns Pointer to resource
		* @returns nullptr if resource can't be found
		**/
		template<typename T>
		T* Get(rID id) {
			return AnyCast<T>(m_Resources[id]);
			//return (T*)m_Resources[id].Pointer;
		}

		/**
		* Creates an empty resource at a virtual path
		* to be filled with builin content,
		* like default shaders
		*
		* @param path Virtual Path to Resource
		* @returns Resource ID
		**/
		template<typename T>
		rID CreateResource(const std::string& path) {
			ENGINE_DEBUG("Creating resources at vPath=" << path << " and type=" << typeid(T).name());

			rID id = GenID();
			m_Resources[id] = MakeAnyPtr<T>();

			m_PathToID[path] = id;
			m_Contexts[m_CurrentContext].push_back(id);

			return id;
		}

		/**
		* Createas an empty resource without
		* registering a virtual path to be used
		* by internal resources like Virtual Textures
		*
		* @returns Resource ID
		**/
		template<typename T>
		rID CreateResource() {
			ENGINE_DEBUG("Creating Resource of type=" << typeid(T).name());

			rID id = GenID();
			m_Resources[id] = MakeAnyPtr<T>();
			m_Contexts[m_CurrentContext].push_back(id);

			return id;
		}

		template<typename T>
		void StoreResource(T* ptr) {
			ENGINE_DEBUG("Storing Ptr=" << ptr << " of type=" << typeid(T).name());
			rID id = GenID();
			m_Resources[id] = ToAnyPtr<T>(ptr);
			m_Contexts[m_CurrentContext].push_back(id);
		}

		/**
		 * Deletes a sigle resource
		 * @param id Resource ID to be deleted
		 **/
		void DeleteResource(rID id);

		/**
		* Creates a Resource Context which is used to
		* determine the lifetime of a resource.
		* @returns Context ID
		**/
		rID CreateContext();

		/**
		* Gets the current context
		* @returns Current Context ID
		**/
		rID GetContext();

		/**
		* Sets current context
		**/
		void SetContext(rID context);

		/**
		* Deletes a Resource Context all its resources
		* @param context Context to be deleted
		* @returns Number of freed resources
		**/
		int DeleteContext(rID context);

		/**
		* Mounts a physical directory to a internal virtual path
		*
		* @param physicalPath Path to a directory on disk
		* @param mountPath Virtual (internal) path
		**/
		void MountDirectory(const std::string& physicalPath, const std::string& mountPath);

		/**
		 * Deletes all resources
		 **/
		void Clear();

		ResourceRegistry();
		~ResourceRegistry();
	private:
		/**
		* Generates a new resource ID
		* @returns Resource ID
		**/
		rID GenID();

		//Private Constructor - Singleton

		rID m_CurrentContext;
		std::unordered_map<rID, AnyPtr> m_Resources;
		std::unordered_map<rID, std::vector<rID>> m_Contexts;
		std::unordered_map<std::string, rID> m_PathToID;
	};



}