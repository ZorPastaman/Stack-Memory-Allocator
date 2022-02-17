# Stack-Memory-Allocator

Usual operators malloc and new are awesome but they have some disadvantages:
memory allocation is random and memory fragmentation is very possible.
One of the solutions is a stack memory allocator. Such an allocator preallocates a buffer of a linear memory segment
and allocates all the object linearly into the buffer. Also, it can free allocations from the end to the beginning.

A stack memory allocator is especially useful when you want to allocate and free objects frequently 
and you know that you access them in a special order.

## Setup

This repo is just a code. Download it and include the headers.

## Usage

### [StackMemoryAllocator](StackMemoryAllocator.h)

This is a basic stack memory allocator. Its constructor takes an argument of a buffer size and dynamically allocates a buffer.
It's your responsibility to set the size that can contain all the objects allocated into it and their infos.

### [TStackMemoryAllocator](TStackMemoryAllocator.h)

This a class derived from [StackMemoryAllocator](StackMemoryAllocator.h).
It takes a buffer size argument in its template parameter and
statically allocates a buffer right after all the other fields in the allocator.
It may make a memory usage even more efficient but the compiler must know its size in advance.

## See Also

- [Linear Memory Allocator](https://github.com/ZorPastaman/Linear-Memory-Allocator)
