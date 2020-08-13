
// Methodology: What you see is what you get (WYSIWYG)

// It needs Unit Testing.

// C header file
// If using C++, include "eulang.h"

// Data functions

#ifndef DATA_H
#define DATA_H

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

// Define this when __int128 doesn't work
//#define NO_INT128
#ifndef NO_INT128
#define WANT_INT128
#endif

// Define this when __float128 doesn't work
//#define NO_FLOAT128
#ifndef NO_FLOAT128
#define WANT_FLOAT128
#endif

// When not using DLL, define this macro:
//#define EULANG_API

// For unix functions, define this macro:
//#define unix

#ifndef EULANG_API
#ifdef unix
// In Unix, use this:
#define EULANG_API extern
#else
// In Microsoft, use this:
#ifdef _EXPORTING
#define EULANG_API __declspec(dllexport)
#else
#define EULANG_API __declspec(dllimport)
#endif
#endif
#endif


// Works for both 64-bit and 32-bit systems.
// Define OLD_X86, for older 32-bit systems:

//#define OLD_X86

// For 16-bit compiler define "OLD_X86" and "BITS16":
//#define BITS16


#pragma pack(push)

#ifdef OLD_X86
#ifdef BITS16
#pragma pack(2)
#else
#define BITS32
#pragma pack(4)
#endif
#else
#define BITS64
#pragma pack(8)
#endif


// Eulang datatypes:

// (NOTE: "MS" is for Microsoft's compiler)

// (1) integer -- 16-bit, 32-bit, 64-bit, or 128-bit
// (2) atom -- ldbl (dbl on MS), or float128
// (3) sequence -- sequence (deque), string, or list, or map (associative array), or stack
// (4) objects -- any type

// New language features:
// (5) string -- read-only memory
// (6) float128 -- GCC-only
// (7) int128 -- unsigned and signed (int128 and uint128), GCC-only
// (8) list -- doubly linked list, use in place of a sequence
// (9) map -- associative array (allow for more than one key per value), key-value pair

// Containers:
// sequence
// list
// map

// constants:
// Version still working on alpha-109
#define VERSION 109
// Suggested length for CHUNK
#define CHUNK 100

// ---------------
// About Types:
// ---------------
// Every type is an object.
// Sequences are special, they represent sequences of objects.
// Sequences can be databytes, arrays of objects, repeats of objects, or linked lists.
// Sequences assigned to another variable are stored only once in memory, its count is increased by one,
// and a reference is used by both variables to point to it. This can be done as many times as required.

// ---------------
// Abstract types:
// ---------------
// Abstract types like strings or data can be stored in memory as bytes, in sequences.
// This memory data can be expanded into an array of objects if required.
// Linked-lists are another abstract type that a sequence can change into if required.
// Repeats store data only once, even though its length can be greater than one (1).

// ---------------
// Sequences can be one of:
// ---------------
// String or MemoryData
// Linked-list
// Repeat
// Array

// ---------------
// Main types are:
// ---------------
// Sequence -- can be a sequence of objects, or an array of bytes
// Integer -- can be Int32
// Long -- can be Int64
// Atom -- can be Double64
// Float32 -- for fast calculations with small floating-point

// ---------------
// Specific types:
// ---------------
// Integer32,
// Uinteger32,
// Double64,
// Integer64,
// Uinteger64,

// ---------------
// Only availible (with GCC):
// ---------------
// Double80,
// Integer128,
// Uinteger128,
// Double128

// ---------------
// Making new variables
// ---------------
// You MUST start your program with "init()"
// Use the functions beginning with "new_" to make new variables in your program.
// You may name these variables to help with debugging.
// If you specify a name or "" empty string, they will be freed if "free_all_objects()" is called.
// Otherwise, you will have to free them yourself by calling "free_object()" on each one.
// You can also use both free_all_objects() and free_object(), without any problems.
// Calling "free_all_objects()" will also call "init()", allowing you to start naming variables again.
// Currently there is no scope, so calling "free_all_objects()" will deallocate all named objects without discrimination.
// Perhaps in the future only some will be deallocated, so that others can still be used based on another scheme.
// Then variables would have scope.

typedef enum
{
	//NOTE TO SELF, JJC: Keep this enum the way it is.
	// There are no signed or unsigned __int128 for 32bit
	// GCC for 32bit doesn't support them, so, they are no longer in the code.
	// For 32-bit:
	is_uninitialized, // 0
	// For 16-bit data structures:
	is_16bit_sequence, // 1
	is_16bit_short, // 2 -- Only on 16-bit
	is_16bit_ushort, // 3 -- Only on 16-bit
	is_16bit_float, // 4 32bit
	is_16bit_integer, // 5 32bit
	is_16bit_uinteger, // 6
	is_16bit_atom, // 7 64bit
	// For 32-bit data structures:
	is_32bit_sequence, // 8
	is_32bit_float, // 9 for faster calculations on small floating-point
	is_32bit_integer, // 10 32bit:
	is_32bit_uinteger, // 11
	is_32bit_atom, // 12 64bit:
	is_32bit_long, // 13
	is_32bit_ulong, // 14
	is_32bit_dbl_80, // 15 only on unix systems:
	is_32bit_dbl_128, // 16
	is_32bit_integer_128, // 17 never used in GCC
	is_32bit_uinteger_128, // 18 never used in GCC
	// For 64-bit data structures:
	is_64bit_sequence, // 19
	is_64bit_float, // 20 for faster calculations on small floating-point
	is_64bit_integer, // 21 32bit:
	is_64bit_uinteger, // 22
	is_64bit_atom, // 23 64bit:
	is_64bit_long, // 24
	is_64bit_ulong, // 25
	is_64bit_dbl_80, // 26 only on unix systems:
	is_64bit_dbl_128, // 27
	is_64bit_integer_128, // 28
	is_64bit_uinteger_128 // 29
} is_type;

// Types, to be used with "int get_type(object * ob);"

#define IS_UNINITIALIZED 0

// FOR 16-BIT: (first byte)
#define IS_16BIT_SEQUENCE 1
#define IS_16BIT_SHORT 2
#define IS_16BIT_USHORT 3
#define IS_16BIT_FLOAT 4
#define IS_16BIT_INTEGER 5
#define IS_16BIT_UINTEGER 6
#define IS_16BIT_ATOM 7

#define IS_32BIT_SEQUENCE 8
#define IS_32BIT_FLOAT 9
#define IS_32BIT_INTEGER 10
#define IS_32BIT_UINTEGER 11
#define IS_32BIT_ATOM 12
#define IS_32BIT_LONG 13
#define IS_32BIT_ULONG 14
#define IS_32BIT_DBL_80 15
// NEVER USED IN GCC 32BIT:
#define IS_32BIT_INTEGER_128 16
// NEVER USED IN GCC 32BIT:
#define IS_32BIT_UINTEGER_128 17
#define IS_32BIT_DBL_128 18
// FOR 64-BIT DATA STRUCTURES:
#define IS_64BIT_SEQUENCE 19
#define IS_64BIT_FLOAT 20
#define IS_64BIT_INTEGER 21
#define IS_64BIT_UINTEGER 22
#define IS_64BIT_ATOM 23
#define IS_64BIT_LONG 24
#define IS_64BIT_ULONG 25
#define IS_64BIT_DBL_80 26
#define IS_64BIT_INTEGER_128 27
#define IS_64BIT_UINTEGER_128 28
#define IS_64BIT_DBL_128 29

#ifndef BITS16
#define IS_ATOM_LONGINTEGER(T) ((T)-(atom)(longinteger)(T)==0)
typedef long long longinteger; // signed long int 64-bit
typedef unsigned long long ulonginteger; // unsigned long int 64-bit
typedef int integer; // int 32-bit
typedef unsigned int uinteger; // unsigned 32-bit
#else
typedef long integer; // int 32-bit
typedef unsigned long uinteger; // unsigned 32-bit
#endif
typedef short shortinteger; // only used in 16-bit
typedef unsigned short ushortinteger; // only used in 16-bit
typedef double atom; // for compatibility reasons, between Windows and Linux
//typedef size_t boolean;
typedef char byte; // int 8-bit, used for blocks of memory, only in sequences
#define IS_ATOM_INTEGER(T) ((T)-(atom)(integer)(T)==0)

#ifdef unix
#ifdef WANT_FLOAT128
typedef __float128 quadatom;
#endif
#ifdef WANT_INT128
typedef __int128 quadinteger;
typedef unsigned __int128 uqaudinteger;
typedef union quad1 {
	quadinteger i;
	uquadinteger u;
	struct parts1 {
		ulonginteger low;
		ulonginteger high;
	};
} unionquad;
#endif
typedef long double longatom; // may be 80-bit on Unix-like, 64-bit on Windows
#define IS_LONGATOM_LONGINTEGER(T) ((T)-(longatom)(longinteger)(T)==0)
#define IS_QUADATOM_LONGINTEGER(T) ((T)-(quadatom)(longinteger)(T)==0)
#endif


/* Begin Datatypes:**********************************************************/

// objects: data structure

// ul == unsigned long int
// ull == unsigned long long
// ulll == unsigned __int128
// q == __float128


typedef struct obj1 {
	union type1 {
		is_type type; // Better for debugging, 32bit variable enums are always 32bit
		int type_value; // 16-bit or 32-bit
	} t;
	union data1 {
#ifdef unix
#ifdef WANT_FLOAT128
		quadatom* quadatom; // 128-bit atom
#endif
#ifdef WANT_INT128
		quadinteger* quadinteger; // 128-bit integer
		uquadinteger* uquadinteger; // 128-bit unsinged integer
#endif
		longatom* longatom; // may be 80-bit on Unix-like, 64-bit on Windows
#endif
#ifndef BITS16
#ifdef BITS64
		atom atom; // for compatibility reasons, between Windows and Linux
		longinteger longinteger; // int 64-bit
		ulonginteger ulonginteger; // unsinged int 64-bits
#endif
#ifdef BITS32
		atom* atom; // for compatibility reasons, between Windows and Linux
		longinteger* longinteger;
		ulonginteger* ulonginteger;
#endif
		integer integer; // int 32-bit
		uinteger uinteger; // unsigned int 32-bits
		float float32;
#else
		atom* atom; // double 64-bit
		integer* integer; // int 32-bit
		uinteger* uinteger; // unsigned int 32-bits
		float* float32; // float 32-bit
		shortinteger shortinteger; // int 16-bit
		ushortinteger ushortinteger; // unsigned int 16-bit
#endif
		struct seq1* sequence;
	} d;
	//size_t optional_object_id; // NOT RECOMMENDED: takes up too much memory, use "sequence->id;" instead
	// I'm trying to make it slim and use the least amount of memory possible.
	// I'm also trying to make it fast, and use constant time whenever possible.
	// If you need an id, I would recommend you make a one-element sequence instead.
} object;

typedef struct node1 {
	object* data;
	struct node1* prev; // "previous" comes before "next"
	struct node1* next;
} node;

typedef struct list1 {
	size_t pos; // position, should start at one (1), works with length
	node* mid;
	node* first;
	node* last;
} linked_list;

#ifdef BITS16
#define SEQ1_BEFORE 7
#define SEQ1_AFTER 7
#define SEQ1_LENGTH 14
#endif
#ifdef BITS32
#define SEQ1_BEFORE 15
#define SEQ1_AFTER 15
#define SEQ1_LENGTH 30
#endif
#ifdef BITS64
#define SEQ1_BEFORE 31
#define SEQ1_AFTER 31
#define SEQ1_LENGTH 62
#endif

#define NO_DEQUE 0
#define APPEND_DEQUE 1
#define PREPEND_DEQUE 2
#define BOTH_DEQUE 3

typedef struct seq1 {
	union count1 {
		struct val1 {
			size_t count : SEQ1_LENGTH;
			size_t deque_flags : 2; // 0==no_deque, 1==append, 2==prepend, 3==both_append_and_prepend
		} v;
		struct deque1 {
			size_t after : SEQ1_AFTER;
			size_t before : SEQ1_BEFORE;
			size_t reserved : 2;
		} d;
	} c;
	struct length1 {
		size_t length : SEQ1_LENGTH;
		size_t seq_type : 2; // 0==memory, 1==repeat, 2==array, 3==linked_list
	} t;
	union array1 {
		linked_list* list;
		byte* memory; // array of data, for use with read-only memory, essentially a byte string
		object* objects; // array, or used with repeat
	} d;
	size_t id; // set by user, used to help in debugging
} sequence;

#define IS_MEMORY 0
#define IS_REPEAT 1
#define IS_ARRAY 2
#define IS_LINKED_LIST 3
// Use to determine whether a sequence is a deque:
#define IS_DEQUE(S) ((S).c.v.deque_flags)

// NOTE: DON'T CHANGE THESE:
// They are based on constants!

// Length limit of a sequence, and chunk limit
#ifdef BITS64
// "ull" for "unsigned long long" literal
#define COUNT_LIMIT 0x3FFFFFFFFFFFFFFFull
#define LENGTH_LIMIT 0x3FFFFFFFFFFFFFFFull
#define CHUNK_LIMIT 2147483647ull
#endif
#ifdef BITS32
// "ul" for "unsigned long" literal
#define COUNT_LIMIT 0x3FFFFFFFul
#define LENGTH_LIMIT 0x3FFFFFFFul
#define CHUNK_LIMIT 32767ul
#endif
#ifdef BITS16
#define COUNT_LIMIT 0x3FFF
#define LENGTH_LIMIT 0x3FFF
#define CHUNK_LIMIT 127
#endif


// function prototypes:

EULANG_API int version();
EULANG_API int version_bit();
EULANG_API int supports_int128();
EULANG_API int supports_float128();

#define WHICH_PLATFORM_WINDOWS 1
#define WHICH_PLATFORM_UNIX 2

EULANG_API int which_platform(); // 1 for windows (Microsoft compiler), 2 for unix (GCC compiler) (more datatypes on GCC)

EULANG_API void init(size_t initial_chunk_length); // must be called before calling any other library functions

void* allocate(size_t size);
void* array_allocate(size_t number, size_t size);
void free_allocate(void* ptr);

// error codes:
#define DEALLOCATE_OBJECT_ERROR 101
#define DEALLOCATE_SEQUENCE_ERROR 102
#define NOT_ENOUGH_MEMORY_ERROR 103
#define SET_OBJECT_ERROR 104
#define DATA_CORRUPTION_ERROR 105
#define NOT_SEQUENCE_ERROR 106
#define LENGTH_RANGE_ERROR 107
#define INDEX_RANGE_ERROR 108
#define LENGTH_LIMIT_ERROR 109
#define UNSUPPORTED_ERROR 110
#define CHUNK_LIMIT_ERROR 111

void error_exit(int error_code);

EULANG_API void free_object(object* obj);
void deallocate_object(object* obj);
void deallocate_sequence(sequence* seq);
void deallocate_sequence_array(object* tmp, size_t length);
void deallocate_list(linked_list* list, size_t start, size_t stop, size_t len);

object* allocate_objects(size_t number);

#ifdef unix
#ifdef WANT_FLOAT128
void set_atom128(object* ret, quadatom* ptr);
#endif
#ifdef WANT_INT128
void set_integer128(object* ret, quadinteger i);
void set_uinteger128(object* ret, uquadinteger u);
#endif
void set_atom80(object* ret, longatom* ptr);
#endif
void set_atom64(object* ret, atom* ptr);

#ifndef BITS16
void set_integer64(object* ret, longinteger i);
void set_uinteger64(object* ret, ulonginteger u);
#else
void set_integer16(object* ret, shortinteger i);
void set_uinteger16(object* ret, ushortinteger u);
#endif
void set_integer32(object* ret, integer i);
void set_uinteger32(object* ret, uinteger u);
void set_float32(object* ret, float f);
void set_memory(object* ret, void* mem, size_t size);

node* range_to_nodes(node** first, sequence* src, size_t start, size_t stop);
void set_sequence(object* ret, sequence* old_dup, int is_expand, size_t start, size_t stop, object* obj, int want_linked_list);
void set_object(object* ret, object* obj, int is_expand);
//EULANG_API object* copy_objects(object* obj, int is_expand, size_t len);

void set_repeat(object* ret, object* element, size_t repeat_count);
EULANG_API object* new_repeat(object* element, size_t repeat_count);

EULANG_API object* new_memory(void* mem, size_t size);
EULANG_API object* new_string(const char* str); // for strings, calls "new_memory()"

#ifdef BITS16
EULANG_API object* new_integer16(shortinteger i);
EULANG_API object* new_uinteger16(ushortinteger i);
#endif
EULANG_API object* new_atom32(float f);
EULANG_API object* new_integer32(integer i);
EULANG_API object* new_uinteger32(uinteger i);
EULANG_API object* new_atom64(atom a);
#ifndef BITS16
EULANG_API object* new_integer64(longinteger i);
EULANG_API object* new_uinteger64(ulonginteger i);
#endif
#ifdef unix
EULANG_API object* new_atom80(longatom a);
#ifdef WANT_INT128
EULANG_API object* new_integer128(quadinteger i);
EULANG_API object* new_uinteger128(uquadinteger i);
#endif
#ifdef WANT_FLOAT128
EULANG_API object* new_atom128(quadatom a);
#endif
#endif
EULANG_API object* new_object(object* old_object);

EULANG_API size_t is_sequence(object* ob);

EULANG_API size_t get_length(sequence* s);
EULANG_API size_t length_of(object* ob);
EULANG_API size_t eu_check_length_seq(sequence* s, size_t i);
EULANG_API size_t eu_check_length(object* ob, size_t i);

// "seq_id" set by user, used to help in debugging
EULANG_API void set_seq_id(object* dst, size_t id);
EULANG_API size_t get_seq_id(object* dst);

node* eu_at_ref_list(linked_list* list, size_t i, size_t length);
object* eu_at_ref_seq(sequence* s, size_t i);
object* eu_at_ref(object* ob, size_t i);
object* eu_new_at_seq(sequence* s, size_t i);
EULANG_API object* eu_new_at(object* ob, size_t i);

EULANG_API int is_linked_list(sequence* seq);
EULANG_API object* convert_to_linked_list(sequence* seq);
EULANG_API object* convert_from_linked_list(sequence* seq);

object get_object_eu_at_seq(sequence* seq, size_t i);
object get_object_eu_at(object ob, size_t i); // changed to more friendly name, was: at_read_only()
object* eu_copy_entire_sequence(sequence* old_dup, int depth, size_t start, size_t stop, object* ob);
EULANG_API void eu_assign(object* dst, object* from, int depth);

// eu_assign works with length, so indexes start at one (1) and go to length
EULANG_API void eu_assign_range(object** dst, size_t start, size_t stop, object* ob);
EULANG_API void eu_assign_splice(object** dst, size_t index, object* ob);
EULANG_API void eu_assign_insert(object** dst, size_t index, object* ob);
EULANG_API void eu_assign_at(object** dst, size_t index, object* ob);
EULANG_API void eu_assign_memory_at(object** dst, size_t index, byte ch);

void make_deque(object** dst, size_t type, int pop_before, int pop_after); // internal, used by "prepend" and "append"
EULANG_API void prepend(object* dst, object* ele);
EULANG_API void append(object* dst, object* ele);

EULANG_API void push_front(object* dst, object* ele);
EULANG_API void push_back(object* dst, object* ele);
EULANG_API void pop_front(object* dst);
EULANG_API void pop_back(object* dst);

EULANG_API void fprint_obj(FILE* outFile, object* obj, int options);
EULANG_API void print(object* obj); // useful, prints to "stdout"
EULANG_API void pretty_print(object* obj); // useful, prints to "stdout"
EULANG_API void print_string(object* obj); // prints printable characters to "stdout"

// Get functions:

EULANG_API int get_type(object* ob);

#ifndef BITS16
#ifdef unix
#ifdef WANT_FLOAT128
EULANG_API quadatom* get_quadatom(object* ob);
#endif
#ifdef WANT_INT128
EULANG_API quadinteger* get_quadinteger(object* ob);
EULANG_API uquadinteger* get_uquadinteger(object* ob);
#endif
EULANG_API longatom* get_longatom(object* ob);
#endif
#ifdef BITS64
EULANG_API atom* get_atom(object* ob);
EULANG_API longinteger get_longinteger(object* ob);
EULANG_API ulonginteger get_ulonginteger(object* ob);
#else
EULANG_API atom* get_atom(object* ob);
#ifdef BITS32
EULANG_API longinteger* get_longinteger(object* ob);
EULANG_API ulonginteger* get_ulonginteger(object* ob);
#endif
#endif
EULANG_API integer get_integer(object* ob);
EULANG_API uinteger get_uinteger(object* ob);
EULANG_API float* get_float32(object* ob);
EULANG_API void* get_data(object* ob);
#endif

//integer eu_compare(object* a, object* b);

//integer map_find_key(object* table, object* key);
//void insert_self(sequence* self, object* what, integer index);
//integer map_insert(object* table, object* record);


/* End Datatypes. ************************************************************/



#pragma pack(pop)

/*
#ifdef __cplusplus
} // extern "C"
#endif
*/

#endif //DATA_H
