#ifndef _ARRAY_H_
#define _ARRAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
   Specifications
   --------------

   The size of the element is defined when the object is created

   create (size).

   The array is created with a size of 2.

   Then, an add() function allows the array to record the data at the end
   and to grown by one.
   The size is adapted according to the needs (realloc())

   The get() and put() functions allow a random access.
 */
   
#include <stddef.h>
#include "Error.h"

/* macros ============================================================== */

/* constants =========================================================== */

   typedef enum
   {
      FARR_OK,
      #define ITEM(n_, s_)\
      FARR_ERR_##n_,
      #include "ArrayError.h"
      #undef ITEM
      FARR_ERR_NB
   }eArrayError;

/* types =============================================================== */
   typedef int Array_Comparator(void const* va, void const* vb);
   typedef int farr_lst_f (void const *v, void *p_user);

/* structures ========================================================== */
   typedef struct Array_s Array_ts;

/* functions =========================================================== */

/* Identification */
   
   /**
    * @brief Retrieves the unique identifier of the array module.
    *
    * @return A constant string representing the module identifier.
    */
    char const *Array_SID (void);
   
   /**
    * @brief Returns the error message corresponding to an error code.
    *
    * This function takes an error code of type `eArrayError` and returns a string that describes
    * the error. The returned string corresponds to one of the predefined error messages, such as 
    * "OK" or other error types like "undefined error", "context error", and more. If the error code 
    * is invalid, a default string "FARR_ERR_?" is returned.
    *
    * Predefined error messages:
    * - `FARR_ERR_UNDEFINED`: "undefined error"
    * - `FARR_ERR_CONTEXT`: "context error"
    * - `FARR_ERR_INVALID_PARAM`: "parameter invalid"
    * - `FARR_ERR_INDEX`: "index error"
    * - `FARR_ERR_ADDRESS`: "address error"
    * - `FARR_ERR_INTERRUPTED`: "traverse interrupted by the user"
    * - `FARR_ERR_CALLBACK`: "callback address error"
    * - `FARR_ERR_MEMORY`: "memory error"
    *
    * @param[in] err The error code of type `eArrayError` whose description is to be retrieved.
    *
    * @return A string describing the error corresponding to the input error code. If the error code is invalid,
    *         returns the string "FARR_ERR_?".
    */
   char const *Array_ErrorString (eArrayError err);

   /**
    * @brief Creates and initializes an Array structure.
    *
    * This function allocates memory for an `Array_ts` structure and initializes it.
    * It pre-allocates memory for storing `counter` elements of the specified size.
    *
    * @param[in] aItemSize The size (in bytes) of each item stored in the array.
    *
    * @return A pointer to the newly created `Array_ts` structure, or `NULL` if memory allocation fails.
    */
    Array_ts *Array_Create (size_t aItemSize);
   
   /**
    * @brief Deletes an Array structure and frees allocated memory.
    *
    * This function releases the memory allocated for the array data and the structure itself.
    * It ensures that the pointer to the data (`Ptr`) is set to `NULL` before freeing the structure.
    *
    * @param[in] this  A pointer to the `Array_ts` structure to be deleted. If `NULL`, the function does nothing.
    */
    void Array_Delete (Array_ts * this);

   /**
    * @brief Initializes an existing Array structure.
    *
    * This function resets the array index to zero.  
    * If the provided pointer is `NULL`, it returns an error code.
    *
    * @param[in] this  A pointer to the `Array_ts` structure to initialize.
    *
    * @return An error code of type `eArrayError`:   
    *         - `FARR_ERR_CONTEXT` if the array structure pointer (`this`) is `NULL`.
    */
    void Array_Init (Array_ts * this);

   /**
    * @brief Adds data to the array, expanding it if necessary.
    *
    * This function adds the provided data (`aData`) to the array. If the array has reached its
    * current capacity, it will double the size of the array. It will also clear the newly allocated
    * space before adding the data.
    *
    * @param[in] this A pointer to the `Array_ts` structure to add data to.
    * @param[in] aData A pointer to the data to be added to the array.
    *
    * @return An error code of type `eArrayError`:   
    *         - `FARR_ERR_CONTEXT` if the context is invalid.
    *         - `FARR_ERR_MEMORY`  if memory allocation fails during array expansion. 
    */
    void Array_Add (Array_ts * this, void const* aData);
   
   /**
    * @brief Retrieves data from the array at the specified index.
    *
    * This function copies the data from the array at the specified `aIndex` into the provided `aData` buffer.
    * If the index is invalid or `aData` is `NULL`, appropriate error codes will be triggered.
    *
    * @param[in] this A pointer to the `Array_ts` structure from which data will be retrieved.
    * @param[in] aIndex The index of the element to retrieve from the array.
    * @param[out] aData A pointer to the buffer where the data will be copied.
    *
    * @return An error code of type `eArrayError`:   
    *         - `FARR_ERR_CONTEXT` if the array structure pointer (`this`) is `NULL`.
    *         - `FARR_ERR_INDEX` if the specified index is out of bounds.
    *         - `FARR_ERR_ADDRESS` if the provided `aData` pointer is `NULL`.
    */
    void Array_Get (Array_ts * this, size_t i, void* aData);
   
   /**
    * @brief Replaces data in the array at the specified index.
    *
    * This function replaces the data at the specified index (`aIndex`) in the array with the provided data (`aData`).
    * If the index is invalid or `aData` is `NULL`, appropriate error codes will be triggered.
    *
    * @param[in] this A pointer to the `Array_ts` structure in which data will be replaced.
    * @param[in] aIndex The index where the data will be replaced in the array.
    * @param[in] aData A pointer to the data to replace the current data at the specified index.
    *
    * @return An error code of type `eArrayError`:   
    *         - `FARR_ERR_CONTEXT` if the array structure pointer (`this`) is `NULL`.
    *         - `FARR_ERR_INDEX` if the specified index is out of bounds.
    *         - `FARR_ERR_ADDRESS` if the provided `aData` pointer is `NULL`.
    */
    void Array_Put (Array_ts * this, size_t i, void const *aData);

   /**
    * @brief Sorts the array using a specified comparator.
    *
    * This function sorts the array in-place using the `qsort` function, based on the provided comparator
    * function (`aComparator`). It sorts the elements from the beginning up to the `Index` in the array.
    * If the array has fewer than two elements, the sort operation is skipped.
    *
    * @param[in] this A pointer to the `Array_ts` structure to be sorted.
    * @param[in] aComparator A pointer to the comparator function used for sorting. This function must
    *                        take two `const void*` arguments and return an integer value.
    *
    * @return None. The array is sorted in-place.
    *
    * @note The array is only sorted if the `Index` is greater than 1. If `this` is `NULL`, an error is triggered.
    */
    void Array_Sort (Array_ts * this, Array_Comparator* aComparator);

#if 1
   /**
    * @brief Returns the current number of elements in the array.
    *
    * This function returns the current number of elements that have been added to the array,
    * which is represented by the `Index` field of the `Array_ts` structure. It does not return
    * the total capacity of the array, which is stored in `Count`.
    *
    * @param[in] this A pointer to the `Array_ts` structure whose element count is to be retrieved.
    *
    * @return The current number of elements in the array, which is the `Index` field of the `Array_ts` structure.
    *         If `this` is `NULL`, returns 0.
    */
   size_t Array_Count(Array_ts * this);
   /**
    * @brief Returns the size of the array (total capacity).
    *
    * This function returns the total capacity of the array, which is the number of elements
    * the array can currently hold. It does not return the number of elements currently stored.
    *
    * @param[in] this A pointer to the `Array_ts` structure whose size (capacity) is to be retrieved.
    *
    * @return The total capacity of the array, which is the `Count` field of the `Array_ts` structure.
    *         If `this` is `NULL`, returns 0.
    */
    size_t Array_Size(Array_ts * this);
#endif

   /**
   * @brief Iterates over the array starting from index 0.
   *
   * Calls a user-provided function for each element of the array, passing 
   * the address of the current data block.
   * 
   * If the user function returns a nonzero value, the iteration stops immediately.
   *
   * @param[in] this   Pointer to the array structure to traverse.
   * @param[in] pf     Pointer to the user-defined function that will be called 
   *                   for each element.
   * @param[in] p_user User-defined pointer passed to the callback function.
   *
   * @return An error code of type `eArrayError`:
   *         - `FARR_OK` if the operation completed successfully.
   *         - `FARR_ERR_CONTEXT` if the context is invalid.
   *         - `FARR_ERR_INTERRUPTED` if 
   *         - `FARR_ERR_CALLBACK` if the parameters are invalid.
   */
   void Array_Walkthrough (Array_ts * this, farr_lst_f * pf, void *p_user);

#ifdef __cplusplus
}
#endif


#endif /* _ARRAY_H_ */
