#ifndef UTILFUNCTIONS_H_
#define UTILFUNCTIONS_H_

#include <iostream>


namespace amsi {
    
  /// Unreversible combination of two hashes. Currently only masks the upper 32 bits of hash a
  ///  and the lower 32 bits of hash b into a result.
  inline size_t combine_hashes(size_t a, size_t b)
  {
    size_t result = 0x0;
    result = a & 0xFFFF0000;
    result += b & 0x0000FFFF;
    return result;
  }

  /// @brief A templated struct used to ease communication to allow offsets into a data buffer
  ///        of some type to be used with templated MPI wrapper functions.
  /// @tparam Data The type of data being sent.
  template <typename Data>
    struct buffer_offset
    {
      Data * buffer; // head of the array
      int size() {return s;}
      int s; // allocated size of array
      int offset; // offset into array
      Data & operator[](int) {return buffer[offset];}
    };

  /// @brief A templated struct to ease communication to allow offsets into a data buffer
  ///        of some type to be used with templated MPI wrapper functions.
  /// @tparam Data The type of data being recved in the buffer.
  /// @tparam Cont The data container object type.
  /// @tparam Alloc The allocator object type for the data container object type.
  template <typename D, template <typename T, typename All = std::allocator<T> > class Container>
    struct recv_buffer_offset
    {
      Container<D> * buffer; // pointer to buffer object
      void resize(int s)
	{
	  buffer->resize(buffer->size() + s);
	}
      D & operator[](int) {return (*buffer)[offset];}
      size_t offset;
    };
  
# if defined(__i386__)
  inline unsigned long long rdtsc(void)
  {
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
  }
# elif defined(__x86_64__)
  inline unsigned long long rdtsc(void)
  {
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
  }
# elif defined(__powerpc__)
  inline unsigned long long rdtsc(void)
  {
    unsigned long long int result=0;
    unsigned long int upper, lower,tmp;
    __asm__ volatile(
      "0:                  \n"
      "\tmftbu   %0           \n"
      "\tmftb    %1           \n"
      "\tmftbu   %2           \n"
      "\tcmpw    %2,%0        \n"
      "\tbne     0b         \n"
      : "=r"(upper),"=r"(lower),"=r"(tmp)
      );
    result = upper;
    result = result<<32;
    result = result|lower;
    
    return(result);
  }
# endif

} // namespace amsi

#endif
