#pragma once

#include <memory>

template <class Type, size_t Size>
class MemoryPool {
private:

	Type* m_data = nullptr;
	uint64_t address = 0;

public:

	MemoryPool() {
		address = 0;

		uint64_t memorySize = sizeof(Type)*Size;
		std::cerr << "Memory Size:" << memorySize / 1024 / 1024 << "MB" << std::endl;

		m_data = static_cast<Type*>(std::malloc(memorySize));
	}

	~MemoryPool() {

		if (m_data != nullptr)
			std::free(m_data);
	}

	void clear() {
		address = 0;
	}
	uint64_t useRate() {
		return address;
	}

	void* getAddress() {
		address++;

		if (address >= Size)
		{
			std::cerr << "Memory Errer" << std::endl;
			return nullptr;
		}

		return &m_data[address - 1];
	}
	void popAddress() {
		address--;
	}

};
