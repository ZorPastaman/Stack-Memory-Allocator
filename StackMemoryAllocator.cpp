// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Stack-Memory-Allocator

#include "StackMemoryAllocator.h"

#include <new>
#include <memory>

namespace Zor {
namespace MemoryAllocators
{
	StackMemoryAllocator::StackMemoryAllocator(const size_t bufferSize) :
		bufferSize(bufferSize)
	{
		m_buffer = static_cast<char*>(malloc(bufferSize));
		if (!m_buffer)
		{
			throw std::bad_alloc();
		}

		m_nextPlace = m_buffer;
	}

	StackMemoryAllocator::StackMemoryAllocator(StackMemoryAllocator&& other) noexcept :
		bufferSize(other.bufferSize),
		m_buffer(other.m_buffer),
		m_nextPlace(other.m_nextPlace)
	{
		other.m_buffer = nullptr;
	}

	StackMemoryAllocator::StackMemoryAllocator(size_t bufferSize, char* buffer) noexcept :
		bufferSize(bufferSize),
		m_buffer(buffer),
		m_nextPlace(m_buffer)
	{
	}

	StackMemoryAllocator::~StackMemoryAllocator() noexcept
	{
		free(m_buffer);
	}

	void* StackMemoryAllocator::Allocate(const size_t alignment, const size_t size)
	{
		void* place = m_nextPlace;
		size_t remainingSize = GetRemainingSize();

		if (std::align(alignment, size, place, remainingSize))
		{
			void* infoPlace = static_cast<char*>(place) + size;
			remainingSize -= size;

			if (std::align(allocationInfoAlignment, allocationInfoSize, infoPlace, remainingSize))
			{
				char* const allocationEnd = static_cast<char*>(infoPlace) + allocationInfoSize;

				AllocationInfo* const info = static_cast<AllocationInfo*>(infoPlace);
				info->allocationSize = static_cast<size_t>(allocationEnd - m_nextPlace);

				m_nextPlace = allocationEnd;

				return place;
			}
		}

		throw std::bad_alloc();
	}

	void* StackMemoryAllocator::AllocateTight(const size_t size)
	{
		const size_t wholeAllocation = size + allocationInfoSize;

		if (GetRemainingSize() >= wholeAllocation)
		{
			void* infoPlace = m_nextPlace + size;
			AllocationInfo* const info = static_cast<AllocationInfo*>(infoPlace);
			info->allocationSize = wholeAllocation;

			void* const place = m_nextPlace;
			m_nextPlace += wholeAllocation;

			return place;
		}

		throw std::bad_alloc();
	}

	bool StackMemoryAllocator::Free() noexcept
	{
		if (m_nextPlace == m_buffer)
		{
			return false;
		}

		const void* const infoPlace = m_nextPlace - allocationInfoSize;
		const AllocationInfo* const info = static_cast<const AllocationInfo*>(infoPlace);
		m_nextPlace -= info->allocationSize;

		return true;
	}

	bool StackMemoryAllocator::Has(const void* const pointer) const noexcept
	{
		return pointer >= m_buffer && pointer < m_buffer + bufferSize;
	}
}}
