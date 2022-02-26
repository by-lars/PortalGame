#include "Asset/Registry.h"
#include "Core/Base.h"

namespace Engine {
	ResourceRegistry::ResourceRegistry() {
		ENGINE_DEBUG_ALLOC("Created Resource Registry");
		rID id = CreateContext();
		SetContext(id);
	}

	ResourceRegistry::~ResourceRegistry() {
		if (m_Resources.size() > 0) {
			ENGINE_WARN("Resource Registry is not empty, but destructor was called!");
			Clear();
		}
	}

	void ResourceRegistry::Clear() {
		ENGINE_DEBUG("Clearing Resource Registry");
		m_Resources.clear();
		m_Contexts.clear();
		m_PathToID.clear();
	}

	rID ResourceRegistry::GenID() {
		static rID counter = 0;
		counter++;
		return counter;
	}

	void ResourceRegistry::_DEBUG_PRINT() {
		ENGINE_DEBUG("Resource Registry Contents");
		ENGINE_DEBUG("Num Contexts: " << m_Contexts.size());
		ENGINE_DEBUG("Num Resources: " << m_Resources.size());
		ENGINE_DEBUG("Current Context: " << m_CurrentContext);
	}

	void ResourceRegistry::DeleteResource(rID id) {
		m_Resources.erase(id);
		//TODO(@Lars): needs to be cleared m_Contexts 
		//TODO(@Lars): needs to be cleared m_PathToID 
	}

	rID ResourceRegistry::CreateContext() {
		rID id = GenID();
		m_Contexts[id] = std::vector<rID>();
		ENGINE_DEBUG("Created new Resource Context rID=" << id);
		return id;
	}

	rID ResourceRegistry::GetContext() {
		return m_CurrentContext;
	}

	void ResourceRegistry::SetContext(rID context) {
		ENGINE_ASSERT(context != INVALID_ID, "Invalid ID: " << context);
		ENGINE_ASSERT(m_Contexts.find(context) != m_Contexts.end(), "Context does not exist: " << context);

		m_CurrentContext = context;
	}

	int ResourceRegistry::DeleteContext(rID context) {
		ENGINE_WARN("TODO(@Lars): Test delete context functino");

		ENGINE_ASSERT(context != INVALID_ID, "Invalid ID: " << context);
		ENGINE_ASSERT(m_Contexts.find(context) != m_Contexts.end(), "Context does not exist: " << context);

		int nrResources = m_Contexts[context].size();

		for (rID id : m_Contexts[context]) {
			m_Resources.erase(id);
			//TODO(@Lars): Also cleanup m_PathToID;
		}

		m_Contexts.erase(context);

		ENGINE_DEBUG("Deleted Context rID=" << context << " and Freed " << nrResources << " resources");

		return nrResources;
	}

	void ResourceRegistry::MountDirectory(const std::string& physicalPath, const std::string& mountPath) {
		ENGINE_ERROR("NOT IMPLEMENTED");
	}

}
