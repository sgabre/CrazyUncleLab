# RTOS - Blinky Software

"blinky" software, which makes an LED blink on a RTOS, is a classic first step in embedded programming. 
This simple task demonstrates control over GPIO (General-Purpose Input/Output) pins and the ability to create time delays. 

## Features

### Option #1

Every 500 ms, the LED2 Change his state. 

### Option #2

When the Button B1 is press, the DELAY change to the next delays value.

The DELAY value are 500, 250, 100, 50 or 25 ms.

Every DELAY, the LED2 Change his state. 

### Option #3 (Semaphore)

The Delay and the Led are 2 different Threads.

The Inter-Communication Process between the Delay and the Led Thread use a semaphore.

When the Button B1 is press, the DELAY change to the next delays value.

When the Delay is expired, the Delay Thread release the led semaphore .

As soon the semaphore can acquire, the led change his state.

The DELAY value are 500, 250, 100, 50 or 25 ms.

### Option #4 (Mutex)

The Delay and the Led are 2 different Threads.

The Inter-Communication Process between the Delay and the Led Thread use a Mutex.

When the Button B1 is press, the DELAY change to the next delays value.

When the Delay is expired, the Delay Thread release the led Mutex .

As soon the Mutex can acquire, the led change his state.

The DELAY value are 500, 250, 100, 50 or 25 ms.

### Option #5 (EventFlags)

The Delay and the Led are 2 different Threads.

The Inter-Communication Process between the Delay and the Led Thread use a EventFlags.

When the Button B1 is press, set a FLAG_CHANGE_FREQUENCY to the Delay Thread.

When the Delay is expired, the Delay Thread send FLAG_LED_BLINK to the Led Thread.

When the FLAG_LED_BLINK is received, the  Led Thread change the Led state.

The DELAY value are 500, 250, 100, 50 or 25 ms.

### Option #6 (Timer)

When the Button B1 is press, the delay change to the next value.

When the Timer is expired, the Led Thread change the Led state.

The DELAY value are 500, 250, 100, 50 or 25 ms.

### Option #7 (Low Consomation Mode)

TBD

### Option #9 (Dynamic Memory Allocation Model/Heap #2)

**Features**

1. Static Allocation
   
The allocator uses a single statically allocated array for the heap, the size of which is defined by the configTOTAL_HEAP_SIZE macro in the FreeRTOS configuration file.

3. Best-Fit Algorithm

The best-fit allocation algorithm attempts to minimize fragmentation by selecting the smallest block of memory that satisfies the allocation request. However, this approach is computationally more intensive and can lead to higher fragmentation over time.

3. Memory Freeing

Unlike heap_1.c, which does not allow memory to be freed, heap_2.c supports deallocation. This makes it more flexible but also introduces the risk of fragmentation if allocations and deallocations are not managed properly.

4. Deprecation

The algorithm used in heap_2.c is considered deprecated because it does not handle fragmentation well and is generally less efficient and robust compared to more modern approaches.

### Option #10 (Dynamic Memory Allocation Model/Heap #3)

**Features**

1. Standard C Library Integration:

This implementation delegates all memory allocation and deallocation tasks to the standard C library functions malloc() and free().
As a result, the memory management is determined by the underlying system's heap configuration, not FreeRTOS itself.

2. No configTOTAL_HEAP_SIZE Usage

Since the standard library handles memory management, the configTOTAL_HEAP_SIZE macro, which typically defines the size of the heap for other FreeRTOS heap allocators, has no effect.
Developers must ensure that the linker script is configured properly to allocate sufficient memory for the system heap.

3. Scheduler Suspension for Thread Safety
   
To ensure thread safety when using malloc() and free() in a multi-threaded FreeRTOS environment, the scheduler is temporarily suspended during calls to these functions. This prevents race conditions when multiple tasks attempt to allocate or free memory concurrently.

5. Dependence on the C Library Implementation

The behavior of heap_3.c depends on the implementation of malloc() provided by the compiler and C library.

Newlib vs. Newlib-nano

- Newlib: A more feature-complete implementation, but it consumes significantly more flash and RAM, which might be unsuitable for memory-constrained devices.
- Newlib-nano: A lightweight version optimized for embedded systems, but with fewer features and possibly different performance characteristics.

**Advantages**

1. Ease of Use: 

Since it relies on standard library functions, there's minimal setup required compared to other FreeRTOS heap managers.

2. Flexibility: 

The behavior of malloc() can be customized through linker script modifications and configuration of the underlying library.

**Disadvantages**

1. Lack of Fine Control

Developers lose the ability to control memory allocation explicitly, as they would with heap_1.c, heap_2.c, or heap_4.c.
Fragmentation and heap size are governed entirely by the standard library implementation.

2. Flash and RAM Overhead:

When using the full version of Newlib, the flash and RAM consumption can be significant, making it unsuitable for small embedded systems.

3. Thread-Safety Overhead

Suspending the scheduler during memory operations introduces a small performance penalty and can lead to increased task latency.



### Option #11 (Dynamic Memory Allocation Model/Heap #4)

**Features**

1. Statically Allocated Array

Like heap_1.c and heap_2.c, heap_4.c uses a single, statically allocated array as the heap space. Its size is defined by the configTOTAL_HEAP_SIZE macro, which must be configured in the FreeRTOS configuration file.

2. First-Fit Allocation Algorithm:

It employs a first-fit strategy to allocate memory, meaning it selects the first block of memory large enough to satisfy the request.
This approach is generally faster than a best-fit algorithm, as it requires less searching.

3. Coalescing Free Blocks

A key feature of heap_4.c is its ability to combine adjacent free blocks into a single larger block during deallocation.
This coalescing technique helps reduce fragmentation and maximizes the usable heap space, especially in scenarios with frequent allocation and deallocation of memory.

4. Improved Speed Over malloc()

By using a simpler algorithm tailored for embedded systems, heap_4.c often performs faster than standard malloc() implementations, particularly those provided by libraries like Newlib-nano.


**Advantages**

1. Reduced Fragmentation

The coalescing mechanism minimizes fragmentation, making it suitable for systems with a mix of small and large memory allocations.

2. Flexibility

Supports both memory allocation and deallocation, unlike heap_1.c.

3. Efficient Use of Memory

By merging adjacent free blocks, it can reuse memory efficiently over time, improving long-term stability in applications with variable memory demands.

**Disadvantages**

1. Non-Deterministic Behavior

The coalescing process introduces variability in the time taken for allocation and deallocation, which can be problematic in real-time systems requiring deterministic behavior.

2. Fragmentation Risks

Although coalescing reduces fragmentation, excessive allocation and deallocation of small blocks, combined with frequent thread creation and deletion, can still lead to memory fragmentation over time.

3. No Built-In Leak Detection
   
The allocator does not inherently prevent or detect memory leaks. Proper coding practices and external tools are required to ensure memory is correctly freed.

### Option #12 (Dynamic Memory Allocation Model/Heap #5)


**Features**

1. Multiple Non-Contiguous Memory Regions

Unlike other heap implementations (heap_1.c, heap_2.c, heap_3.c, and heap_4.c), which rely on a single contiguous memory block, heap_5.c allows memory pools to be distributed across multiple regions.
This is useful for systems with both internal and external memory, such as MCUs with limited internal SRAM and external SDRAM or CCM (Core-Coupled Memory).

2. Same Algorithm as heap_4.c

heap_5.c uses the same first-fit allocation algorithm with coalescing as heap_4.c, making it efficient and capable of reducing fragmentation.

3. Customizable Memory Regions

Developers can define multiple memory pools in different physical or logical memory regions using the vPortDefineHeapRegions() function.
Each region is defined as an array of structures, specifying the base address and size for each memory pool.

4. Advanced Memory Management

By combining a custom linker script and heap_5.c, developers can strategically allocate critical data (e.g., threads) to fast internal SRAM or CCM and less-critical data (e.g., semaphores, mutexes) to slower external SDRAM.

**Advantages**

1. Efficient Use of Available Memory
   
Makes it possible to take full advantage of all available memory regions in the system, even if they are non-contiguous.
Useful for systems with fragmented or tiered memory resources.

2. Flexibility

Allows developers to optimize memory allocation based on the speed and characteristics of each memory region. For instance:
Place time-critical tasks in fast internal SRAM.
Allocate less-critical objects in external SDRAM.

3. Scalability
   
Especially beneficial in systems requiring large memory, such as STM32 MCUs with FSMC controllers that can utilize external SDRAM.

4. Reduced Fragmentation
   
Inherits the coalescing feature of heap_4.c, which helps to minimize memory fragmentation.

**Disadvantages**

1. Complex Setup
   
Requires a deep understanding of memory architecture, linker scripts, and the FreeRTOS vPortDefineHeapRegions() API.
Not suitable for beginners or simpler embedded systems.

2. Potential Non-Determinism

Like heap_4.c, heap_5.c is non-deterministic due to the first-fit algorithm and coalescing mechanism, which may be unsuitable for hard real-time constraints.

3. Advanced Debugging Needs
   
Debugging memory issues can become challenging due to the added complexity of multiple regions and the need to monitor fragmentation across all pools.

## Thread-Safety Option

TBD

## Stack Overflow Detection

TBD

## Debugging Features

TBD

## Logging Features

TBD
