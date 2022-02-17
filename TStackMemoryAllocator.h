// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Stack-Memory-Allocator

#pragma once

#include "StackMemoryAllocator.h"

namespace Zor {
	namespace MemoryAllocators
	{
		/**
		 * Preallocates a buffer of a specified size inside the allocator and Stackly allocates objects into it.
		 * After each allocation, the allocator places an allocation info describing a size of an allocation.
		 * It's used to free a last object in the buffer.
		 * @tparam NBufferSize
		 */
		template<size_t NBufferSize = 1024>
		class TStackMemoryAllocator : public StackMemoryAllocator
		{
		public:
			static constexpr size_t bufferSizeValue = NBufferSize;

		private:
			char m_buffer[NBufferSize] = {};

		public:
			TStackMemoryAllocator() noexcept :
				StackMemoryAllocator(NBufferSize, m_buffer)
			{
			}

			TStackMemoryAllocator(const TStackMemoryAllocator&) = delete;
			TStackMemoryAllocator(TStackMemoryAllocator&&) = delete;

		public:
			~TStackMemoryAllocator() noexcept override;

		public:
			TStackMemoryAllocator& operator=(const TStackMemoryAllocator&) = delete;
			TStackMemoryAllocator& operator=(TStackMemoryAllocator&&) = delete;
		};

		template<size_t NBufferSize>
		TStackMemoryAllocator<NBufferSize>::~TStackMemoryAllocator() noexcept
		{
			KillBufferPointer();
		}
	}}
