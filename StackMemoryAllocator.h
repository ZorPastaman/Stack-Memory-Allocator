// Copyright (c) 2022 Vladimir Popov zor1994@gmail.com https://github.com/ZorPastaman/Stack-Memory-Allocator

#pragma once

#include <cstdlib>
#include <utility>

namespace Zor {
namespace MemoryAllocators
{
	/**
	 * Preallocates a buffer of a specified size and linearly allocates objects into it.
	 * After each allocation, the allocator places an allocation info describing a size of an allocation.
	 * It's used to free a last object in the buffer.
	 */
	class StackMemoryAllocator
	{
	private:
		struct AllocationInfo
		{
			size_t allocationSize;
		};

	public:
		static constexpr size_t allocationInfoAlignment = alignof(AllocationInfo);
		static constexpr size_t allocationInfoSize = sizeof(AllocationInfo);

		const size_t bufferSize;

	private:
		char* m_buffer;
		char* m_nextPlace;

	public:
		StackMemoryAllocator(size_t bufferSize = 1024);
		StackMemoryAllocator(StackMemoryAllocator&& other) noexcept;

		StackMemoryAllocator(const StackMemoryAllocator&) = delete;

	protected:
		/**
		 * Creates an allocator with an external buffer supplied by a derived class.
		 * @param bufferSize
		 * @param buffer
		 * @remark The @a buffer must be of size of the @a bufferSize.
		 * @remark If the @a buffer mustn't be destroyed on destruction,
		 * call @a KillBufferPointer() in a derived destructor.
		 */
		StackMemoryAllocator(size_t bufferSize, char* buffer) noexcept;

	public:
		virtual ~StackMemoryAllocator() noexcept;

	public:
		/**
		 * Allocates a placement in the buffer for an object of the specified parameters with alignment.
		 * @param alignment Alignment of the object.
		 * @param size Size of the object.
		 * @returns Placement for an object of the specified parameters.
		 * @remark The aligned allocation info is placed after the allocated object.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see AllocateTight(size_t)
		 */
		void* Allocate(size_t alignment, size_t size);
		/**
		 * Allocates an object of type @a T in the buffer with alignment.
		 * @tparam T Allocated object type.
		 * @tparam TArgs Constructor argument types.
		 * @param arguments Constructor arguments.
		 * @returns Allocated object.
		 * @remark The aligned allocation info is placed after the allocated object.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see AllocateTight(TArgs&&...)
		 */
		template<typename T, typename... TArgs>
		T* Allocate(TArgs&&... arguments);

		/**
		 * Allocates a placement in the buffer for an object of the specified parameters without alignment.
		 * @param size Size of the object.
		 * @returns Placement for an object of the specified parameters.
		 * @remark The allocation info is placed right after the allocated object.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see Allocate(size_t, size_t)
		 */
		void* AllocateTight(size_t size);
		/**
		 * Allocates an object of type @a T in the buffer without alignment.
		 * @tparam T Allocated object type.
		 * @tparam TArgs Constructor argument types.
		 * @param arguments Constructor arguments.
		 * @returns Allocated object.
		 * @remark The allocation info is placed right after the allocated object.
		 * @throws std::bad_alloc Thrown when it's impossible to allocate a desired placement in the buffer.
		 * @see Allocate(TArgs&&...)
		 */
		template<typename T, typename... TArgs>
		T* AllocateTight(TArgs&&... arguments);

		/**
		 * Frees a last allocated object.
		 * @returns @p True if an object was freed; @p false if the buffer is empty.
		 */
		bool Free() noexcept;

		/**
 		* Starts object allocations from the beginning.
 		*/
		inline void Reset() noexcept { m_nextPlace = m_buffer; };

		/**
		 * Checks if the @a pointer points to the memory in the buffer.
		 * @param pointer Checked pointer.
		 * @returns @p True if the @a pointer points to the memory in the buffer; @p false otherwise.
		 */
		bool Has(const void* pointer) const noexcept;

		inline size_t GetRemainingSize() const noexcept
		{
			return static_cast<size_t>(m_buffer + bufferSize - m_nextPlace);
		}

	protected:
		/**
		 * Kills the pointer to the buffer.
		 * It may be used in a derived destructor to prevent a destruction of the buffer in this class destructor.
		 */
		inline void KillBufferPointer() noexcept { m_buffer = nullptr; };

	public:
		StackMemoryAllocator& operator=(const StackMemoryAllocator&) = delete;
		StackMemoryAllocator& operator=(StackMemoryAllocator&&) = delete;
	};

	template<typename T, typename... TArgs>
	T* StackMemoryAllocator::Allocate(TArgs&&... arguments)
	{
		void* place = Allocate(alignof(T), sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}

	template<typename T, typename... TArgs>
	T* StackMemoryAllocator::AllocateTight(TArgs&&... arguments)
	{
		void* place = AllocateTight(sizeof(T));
		return new (place) T(std::forward<TArgs>(arguments)...);
	}
}}
