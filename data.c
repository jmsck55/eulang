
// It needs Unit Testing.

#include "pch.h"
#include "data.h"

#ifdef unix
#ifdef WANT_FLOAT128
#include <quadmath.h>
#endif
#endif

int version()
{
	return VERSION;
}
int version_bit()
{
#ifdef BITS16
	return 16;
#endif
#ifdef BITS32
	return 32;
#endif
#ifdef BITS64
	return 64;
#endif
}

int supports_int128()
{
#ifdef unix
#ifdef WANT_INT128
	return 1; // TRUE
#else
	return 0; // FALSE
#endif
#else
	return 0; // FALSE
#endif
}
int supports_float128()
{
#ifdef unix
#ifdef WANT_FLOAT128
	return 1; // TRUE
#else
	return 0; // FALSE
#endif
#else
	return 0; // FALSE
#endif
}

int which_platform()
{
#ifdef unix
	return WHICH_PLATFORM_UNIX; // 2. unix-type (uses GCC compiler)
#else
	return WHICH_PLATFORM_WINDOWS; // 1. windows (uses Microsoft compiler)
#endif
}

size_t chunk;

void init(size_t initial_chunk_length)
{
	if (initial_chunk_length >= CHUNK_LIMIT)
	{
		error_exit(CHUNK_LIMIT_ERROR);
	}
	chunk = initial_chunk_length;
}

void* allocate(size_t size)
{
	void* ptr;
	ptr = malloc(size); // must set data allocated after calling this function
	if (!ptr)
	{
		error_exit(NOT_ENOUGH_MEMORY_ERROR);
	}
	return ptr;
}

void* array_allocate(size_t number, size_t size)
{
	void* ptr;
	ptr = calloc(number, size); // sets allocated memory to zero (0)
	if (!ptr)
	{
		error_exit(NOT_ENOUGH_MEMORY_ERROR);
	}
	return ptr;
}

void free_allocate(void* ptr)
{
	free(ptr);
}

void error_exit(int i)
{
	printf("Program has crashed with error code: %i\n", i);
	exit(i);
}

// Free object functions:

void free_object(object* obj)
{
	if (obj)
	{
		deallocate_object(obj);
		free_allocate(obj);
	}
}

void deallocate_object(object* object)
{
	//done.
	if (object)
	{
		sequence* sequence;
		switch (object->t.type)
		{
		case is_uninitialized:
			break;
#ifdef BITS16
// For 16-bit data structures:
		case is_16bit_sequence:
			sequence = object->d.sequence;
			if (IS_DEQUE(*sequence))
			{
				sequence->d.objects -= sequence->c.d.before;
			}
			else if (sequence->c.v.count > 1)
			{
				sequence->c.v.count--;
				return;
			}
			// need to free_allocate() the sequence
			deallocate_sequence(sequence);
			free_allocate(sequence);
			break;
		case is_16bit_short:
			break;
		case is_16bit_ushort:
			break;
		case is_16bit_float:
			free_allocate(object->d.float32);
			break;
		case is_16bit_integer:
			free_allocate(object->d.integer);
			break;
		case is_16bit_uinteger:
			free_allocate(object->d.uinteger);
			break;
		case is_16bit_atom:
			free_allocate(object->d.atom);
			break;
#endif
#ifdef BITS32
		case is_32bit_float:
			break;
		case is_32bit_integer:
			break;
		case is_32bit_uinteger:
			break;
		case is_32bit_long:
			free_allocate(object->d.longinteger);
			break;
		case is_32bit_ulong:
			free_allocate(object->d.ulonginteger);
			break;
		case is_32bit_atom:
			free_allocate(object->d.atom);
			break;
#ifdef unix
		case is_32bit_dbl_80:
			free_allocate(object->d.longatom);
			break;
// NOTE: Not availible on 32-bit GCC
//#ifdef WANT_INT128
//		case is_32bit_integer_128:
//			free_allocate(object->quadinteger);
//			break;
//		case is_32bit_uinteger_128:
//			free_allocate(object->uquadinteger);
//			break;
//#endif
#ifdef WANT_FLOAT128
		case is_32bit_dbl_128:
			free_allocate(object->d.quadatom);
			break;
#endif // WANT_FLOAT128
#endif
		case is_32bit_sequence:
			sequence = object->d.sequence;
			if (IS_DEQUE(*sequence))
			{
				sequence->d.objects -= sequence->c.d.before;
			}
			else if (sequence->c.v.count > 1)
			{
				sequence->c.v.count--;
				return;
			}
			// need to free_allocate() the sequence
			deallocate_sequence(sequence);
			free_allocate(sequence);
			break;
#endif
#ifdef BITS64
		case is_64bit_float:
			break;
		case is_64bit_integer:
			break;
		case is_64bit_uinteger:
			break;
		case is_64bit_long:
			break;
		case is_64bit_ulong:
			break;
		case is_64bit_atom:
			break;
#ifdef unix
		case is_64bit_dbl_80:
			free_allocate(object->d.longatom);
			break;
#ifdef WANT_INT128
		case is_64bit_integer_128:
			free_allocate(object->d.quadinteger);
			break;
		case is_64bit_uinteger_128:
			free_allocate(object->d.uquadinteger);
			break;
#endif
#ifdef WANT_FLOAT128
		case is_64bit_dbl_128:
			free_allocate(object->d.quadatom);
			break;
#endif
#endif
		case is_64bit_sequence:
			sequence = object->d.sequence;
			if (IS_DEQUE(*sequence))
			{
				sequence->d.objects -= sequence->c.d.before;
			}
			else if (sequence->c.v.count > 1)
			{
				sequence->c.v.count--;
				return;
			}
			// need to free_allocate() the sequence
			deallocate_sequence(sequence);
			free_allocate(sequence);
			break;
#endif
		default:
			// error handling
			error_exit(DEALLOCATE_OBJECT_ERROR);
			break;
		}
	}
}

void deallocate_sequence(sequence* seq)
{
	//done.
	switch (seq->t.seq_type)
	{
	case IS_MEMORY:
		free_allocate(seq->d.memory);
		break;
	case IS_REPEAT:
		free_object(seq->d.objects);
		break;
	case IS_ARRAY:
		deallocate_sequence_array(seq->d.objects, seq->t.length);
		free_allocate(seq->d.objects);
		break;
	case IS_LINKED_LIST:
		deallocate_list(seq->d.list, 1, seq->t.length, seq->t.length);
		free_allocate(seq->d.list);
		break;
	default:
		// something went wrong,
		// error handling needed
		error_exit(DEALLOCATE_SEQUENCE_ERROR);
		break;
	}
}

void deallocate_sequence_array(object* tmp, size_t length)
{
	size_t i;
	for (i = 0; i < length; i++) // similar to a foreach(), but in plain "C"
	{
		deallocate_object(tmp);
		tmp++;
	}
}

void deallocate_list(linked_list* list, size_t start, size_t stop, size_t len)
{
	//done.
	node* current_node, * next;
	size_t i;
	current_node = eu_at_ref_list(list, start, len);
	for (i = start; i <= stop; i++)
	{
		free_object(current_node->data);
		next = current_node->next;
		free_allocate(current_node);
		current_node = next;
	}
}

// Allocate objects:

object* allocate_objects(size_t number)
{
	object* ret;
	ret = (object*)array_allocate(number, sizeof(object));
	return ret;
}

#ifdef unix
#ifdef WANT_FLOAT128
void set_atom128(object* ret, quadatom* ptr)
{
	ret->d.quadatom = (quadatom*)allocate(sizeof(quadatom));
	memcpy(ret->d.quadatom, ptr, sizeof(quadatom));
#ifdef BITS64
	ret->t.type = is_64bit_dbl_128;
#else
	ret->t.type = is_32bit_dbl_128;
#endif
}
#endif
#ifdef WANT_INT128
void set_integer128(object* ret, quadinteger i)
{
	ret->d.quadinteger = (quadinteger*)allocate(sizeof(quadinteger));
	* ret->d.quadinteger = i;
	ret->t.type = is_64bit_integer_128;
}
void set_uinteger128(object* ret, uquadinteger u)
{
	ret->d.uquadinteger = (uquadinteger*)allocate(sizeof(uquadinteger));
	* ret->d.uquadinteger = u;
	ret->t.type = is_64bit_uinteger_128;
}
#endif
void set_atom80(object* ret, longatom* ptr)
{
	ret->d.longatom = (longatom*)allocate(sizeof(longatom));
	memcpy(ret->d.longatom, ptr, sizeof(longatom));
#ifdef BITS32
	ret->t.type = is_32bit_dbl_128;
#else
	ret->t.type = is_64bit_dbl_128;
#endif
}
#endif
void set_atom64(object* ret, atom* ptr)
{
#ifndef BITS64
	ret->d.atom = (atom*)allocate(sizeof(atom));
	memcpy(ret->d.atom, ptr, sizeof(atom));
#ifdef BITS32
	ret->t.type = is_32bit_atom;
#else
	ret->t.type = is_16bit_atom;
#endif // BITS32
#else
	ret->d.atom = *ptr;
	ret->t.type = is_64bit_atom;
#endif
}
#ifndef BITS16
void set_integer64(object* ret, longinteger i)
{
#ifdef BITS32
	ret->d.longinteger = (longinteger*)allocate(sizeof(longinteger));
	* ret->d.longinteger = i;
	ret->t.type = is_32bit_long;
#else
	ret->d.longinteger = i;
	ret->t.type = is_64bit_long;
#endif
}
void set_uinteger64(object* ret, ulonginteger u)
{
#ifdef BITS32
	ret->d.ulonginteger = (ulonginteger*)allocate(sizeof(ulonginteger));
	* ret->d.ulonginteger = u;
	ret->t.type = is_32bit_ulong;
#else
	ret->d.ulonginteger = u;
	ret->t.type = is_64bit_ulong;
#endif
}
#else
void set_integer16(object* ret, shortinteger i)
{
	ret->d.shortinteger = i;
	ret->t.type = is_16bit_short;
}
void set_uinteger16(object* ret, ushortinteger i)
{
	ret->d.ushortinteger = i;
	ret->t.type = is_16bit_ushort;
}
#endif
void set_integer32(object* ret, integer i)
{
#ifndef BITS16
	ret->d.integer = i;
#ifdef BITS32
	ret->t.type = is_32bit_integer;
#else
	ret->t.type = is_64bit_integer;
#endif
#else
	* ret->d.integer = i;
	ret->t.type = is_16bit_integer;
#endif
}
void set_uinteger32(object* ret, uinteger u)
{
#ifndef BITS16
	ret->d.uinteger = u;
#ifdef BITS32
	ret->t.type = is_32bit_uinteger;
#else
	ret->t.type = is_64bit_uinteger;
#endif
#else
	* ret->d.uinteger = u;
	ret->t.type = is_16bit_uinteger;
#endif
}

void set_float32(object* ret, float f)
{
#ifndef BITS16
	ret->d.float32 = f;
#ifdef BITS32
	ret->t.type = is_32bit_float;
#else
	ret->t.type = is_64bit_float;
#endif
#else
	* ret->d.float32 = f;
	ret->t.type = is_16bit_float;
#endif
}

void set_memory(object* ret, void* mem, size_t size)
{
	sequence* seq;
	if (size > LENGTH_LIMIT)
	{
		error_exit(LENGTH_LIMIT_ERROR);
	}
	seq = (sequence*)array_allocate(1, sizeof(sequence));
	seq->c.v.count = 1; // 0 is uninitialized
	if (size)
	{
		seq->d.memory = (byte*)allocate(size);
		//seq->d.memory = (byte*)allocate(size * sizeof(byte)); // sizeof(byte) == 1
#ifdef unix
		memcpy(seq->d.memory, mem, size);
#else
#ifndef BITS16
		memcpy_s(seq->d.memory, size, mem, size);
#else
		memcpy(seq->d.memory, mem, size);
#endif
#endif
	}
	seq->t.length = size;
	//seq->seq_type = IS_MEMORY; // already set to zero (0)
	ret->d.sequence = seq;
#ifdef BITS64
	ret->t.type = is_64bit_sequence;
#endif
#ifdef BITS32
	ret->t.type = is_32bit_sequence;
#endif
#ifdef BITS16
	ret->t.type = is_16bit_sequence;
#endif
}

node* range_to_nodes(node** first, sequence* src, size_t start, size_t stop)
{
	//done.
	// NOTE: Nodes dont expand.
	// NOTE: (Expanding has been disabled on nodes for the time being.)
	// NOTE: It would require more programming to enable it.
	// supply first, returns last
	node* last, *prev;
	size_t i;
	if (start - stop == 1)
	{
		last = NULL;
		*first = last;
		return last;
	}
	last = (node*)array_allocate(1, sizeof(node));
	last->data = eu_new_at_seq(src, start);
	*first = last;
	for (i = start + 1; i <= stop; i++)
	{
		prev = last;
		last = (node*)array_allocate(1, sizeof(node));
		last->data = eu_new_at_seq(src, i);
		prev->next = last;
		last->prev = prev;
	}
	return last;
}

// is_expand == 0 don't expand, == 1 expand first level, == (-1) expand all levels.
// start and stop begin at one (1)
void set_sequence(object* ret, sequence* old_dup, int is_expand, size_t start, size_t stop, object* obj, int want_linked_list)
{
	// set sequence, and (optionally) update a record
	// it can also do: inserting and replacing
	//done.
	size_t length, len, len2, i, flag;
	unsigned long long len3;
	sequence* seq;
	if ((!is_expand) && (!IS_DEQUE(*old_dup)))
	{
		if (old_dup->c.v.count < COUNT_LIMIT)
		{
			old_dup->c.v.count++;
			ret->d.sequence = old_dup;
#ifdef BITS16
			ret->t.type = is_16bit_sequence;
#endif
#ifdef BITS32
			ret->t.type = is_32bit_sequence;
#endif
#ifdef BITS64
			ret->t.type = is_64bit_sequence;
#endif
			return;
		}
	}
//	low_level_scope += 1; // Begin Low-Level scope.
	length = old_dup->t.length;
	//if (!((start > 0) && (start <= stop + 1) && (stop <= length)))
	if ((start == 0) || (start > stop + 1) || (stop > length))
	{
		error_exit(INDEX_RANGE_ERROR);
	}
	if (is_expand) // maximum number of levels to expand sequence to.
	{
		is_expand--;
	}
	seq = (sequence*)array_allocate(1, sizeof(sequence));
	seq->c.v.count = 1; // starts a new "duplicate" sequence, with count==1
	flag = 0;
	if (obj)
	{
#ifdef BITS16
		if (obj->t.type == is_16bit_sequence)
#endif
#ifdef BITS32
		if (obj->t.type == is_32bit_sequence)
#endif
#ifdef BITS64
		if (obj->t.type == is_64bit_sequence)
#endif
		{
			len = obj->d.sequence->t.length;
			if (obj->d.sequence->t.seq_type == IS_MEMORY)
			{
				flag = 1;
			}
		}
		else
		{
#ifdef BITS16
			if (obj->t.type == is_16bit_short)
#else
#ifdef BITS32
			if (obj->t.type == is_32bit_integer)
#else
			if (obj->t.type == is_64bit_integer)
#endif
#endif
			{
#ifndef BITS16
				if ((obj->d.integer >= -128) && (obj->d.integer <= 127))
#else
				if ((obj->d.shortinteger >= -128) && (obj->d.shortinteger <= 127))
#endif
				{
					flag = 2;
				}
			}
			len = 1;
		}
		//len2 = length - (stop - start + 1) + len;
	}
	else
	{
		len = 0;
	}
	len3 = length;
	len3 -= stop;
	len3 += start;
	len3--;
	len3 += len;
	if (len3 > LENGTH_LIMIT)
	{
		error_exit(LENGTH_LIMIT_ERROR); // try breaking large sequences into smaller ones
	}
	len2 = (size_t)len3;
	seq->t.length = len2;
	if ((want_linked_list >= 1) || ((want_linked_list == 0) && (old_dup->t.seq_type == IS_LINKED_LIST)))
	{
		// copy and convert sequence into linked list
		linked_list* list;
		node* n1, * n2, * n3, * n4, * n5, * first, * last;
		n1 = range_to_nodes(&first, old_dup, 1, start - 1);
		last = n1;
		if (len)
		{
#ifdef BITS16
			if (obj->t.type == is_16bit_sequence)
#endif
#ifdef BITS32
			if (obj->t.type == is_32bit_sequence)
#endif
#ifdef BITS64
			if (obj->t.type == is_64bit_sequence)
#endif
			{
				n3 = range_to_nodes(&n2, obj->d.sequence, 1, len); // what if (len == 0) ???
			}
			else
			{
				n2 = (node*)array_allocate(1, sizeof(node));
				n2->data = new_object(obj);
				n3 = n2;
			}
			if (n3)
			{
				last = n3;
			}
		}
		else
		{
			n2 = NULL;
			n3 = NULL;
		}
		if (!first)
		{
			first = n2;
		}
		if (n1)
		{
			if (n2)
			{
				n1->next = n2;
				n2->prev = n1;
			}
		}
		n5 = range_to_nodes(&n4, old_dup, stop + 1, length);
		if (!first)
		{
			first = n4;
		}
		if (n4)
		{
			if (n3)
			{
				n3->next = n4;
				n4->prev = n3;
			}
			else if (!n2)
			{
				if (n1)
				{
					n1->next = n4;
					n4->prev = n1;
				}
			}
		}
		if (n5)
		{
			last = n5;
		}
		list = (linked_list*)array_allocate(1, sizeof(linked_list));
		list->first = first;
		list->last = last;
		if (n2)
		{
			list->mid = n2;
			list->pos = start; // "mid" is somewhere in the middle of linked list
		}
		seq->t.seq_type = IS_LINKED_LIST;
		seq->d.list = list;
	}
	else if ((old_dup->t.seq_type == IS_MEMORY) && ((flag) || (len == 0)))
	{
		// both are "is_memory"
		byte* tmp, * dst, * src;
		size_t size;
		tmp = (byte*)allocate(len2);

		dst = tmp;
		src = old_dup->d.memory;
		size = start - 1;
#ifdef unix
		memcpy(dst, src, size); // first memcpy()
#else
#ifdef BITS16
		memcpy(dst, src, size); // first memcpy()
#else
		memcpy_s(dst, len2, src, size);
#endif
#endif
		dst += size;
		if (len)
		{
			if (flag == 2)
			{
#ifdef BITS16
				* dst = (char)obj->d.shortinteger;
#else
				* dst = (char)obj->d.integer;
#endif
			}
			else
			{
#ifdef unix
				memcpy(dst, obj->d.sequence->d.memory, len); // second memcpy()
#else
#ifdef BITS16
				memcpy(dst, obj->d.sequence->d.memory, len); // second memcpy()
#else
				memcpy_s(dst, len2 - size, obj->d.sequence->d.memory, len); // second memcpy()
#endif
#endif
			}
		}

		dst += len;
		src += stop;
#ifdef unix
		memcpy(dst, src, length - stop); // third (last) memcpy()
#else
#ifdef BITS16
		memcpy(dst, src, length - stop); // third (last) memcpy()
#else
		memcpy_s(dst, len2 - size - len, src, length - stop);
#endif
#endif
		//NOTE: IS_MEMORY is already set to zero (0)
		seq->t.seq_type = IS_MEMORY;
		seq->d.memory = tmp;
	}
	else
	{
		object* tmp, * dst, * src;
		object ob;
		byte* ptr;
		size_t flag;
		tmp = allocate_objects(len2);
		dst = tmp; // dst
		src = old_dup->d.objects; // src, in objects
		ptr = old_dup->d.memory; // src, in bytes
		flag = 1;
		if (start == length + 1)
		{
			flag = 2;
			length++;
		}
		for (i = 1; i <= length; i++)
		{
			if ((i == start) && (flag))
			{
				size_t j;
				if (old_dup->t.seq_type != IS_REPEAT) // IS_MEMORY, IS_ARRAY, IS_DEQUE
				{
					for (j = start; j <= stop; j++)
					{
						src++;
						ptr++;
					}
				}
				if (len)
				{
#ifdef BITS16
					if (obj->t.type == is_16bit_sequence)
#endif
#ifdef BITS32
					if (obj->t.type == is_32bit_sequence)
#endif
#ifdef BITS64
					if (obj->t.type == is_64bit_sequence)
#endif
					{
						ob = get_object_eu_at(*obj, 1);
					}
					else
					{
						ob = *obj;
					}
					set_object(dst, &ob, is_expand);
					for (j = 2; j <= len; j++)
					{
						dst++;
						ob = get_object_eu_at(*obj, j);
						set_object(dst, &ob, is_expand);
					}
				}
				else
				{
					dst--;
				}
				i += (stop - start);
				if (flag == 2)
				{
					i++;
				}
				flag = 0; // only run this code once per loop.
			}
			else
			{
				switch (old_dup->t.seq_type)
				{
				case IS_ARRAY:
					set_object(dst, src, is_expand);
					src++;
					break;
				case IS_REPEAT:
					set_object(dst, old_dup->d.objects, is_expand);
					break;
				case IS_MEMORY:
#ifdef BITS16
					set_integer16(dst, (shortinteger)* ptr);
#else
					set_integer32(dst, (integer)* ptr);
#endif
					ptr++;
					break;
				default:
					error_exit(DATA_CORRUPTION_ERROR);
					break;
				}
			}
			dst++;
		}
		seq->t.seq_type = IS_ARRAY;
		seq->d.objects = tmp;
	}
	ret->d.sequence = seq;
#ifdef BITS16
	ret->t.type = is_16bit_sequence;
#else
#ifdef BITS32
	ret->t.type = is_32bit_sequence;
#else
	ret->t.type = is_64bit_sequence;
#endif
#endif

//	low_level_scope -= 1; // End Low-Level scope.
//	free_all_low_level_objects(0);
}

void set_object(object* ret, object* obj, int is_expand)
{
	if (obj)
	{
		switch (obj->t.type)
		{
		case is_uninitialized:
			break;
#ifdef BITS16
		case is_16bit_sequence:
			set_sequence(ret, obj->d.sequence, is_expand, 1, 0, NULL, 0);
			break;
		case is_16bit_short:
			set_integer16(ret, obj->d.shortinteger);
			break;
		case is_16bit_ushort:
			set_uinteger16(ret, obj->d.ushortinteger);
			break;
		case is_16bit_float:
			set_float32(ret, *obj->d.float32);
			break;
		case is_16bit_integer:
			set_integer32(ret, *obj->d.integer);
			break;
		case is_16bit_uinteger:
			set_uinteger32(ret, *obj->d.uinteger);
			break;
		case is_16bit_atom:
			set_atom64(ret, obj->d.atom);
			break;
#endif
#ifdef BITS32
		case is_32bit_float:
			set_float32(ret, obj->d.float32);
			break;
		case is_32bit_integer:
			set_integer32(ret, obj->d.integer);
			break;
		case is_32bit_uinteger:
			set_uinteger32(ret, obj->d.uinteger);
			break;
		case is_32bit_long:
			set_integer64(ret, *obj->d.longinteger);
			break;
		case is_32bit_ulong:
			set_uinteger64(ret, *obj->d.ulonginteger);
			break;
		case is_32bit_atom:
			set_atom64(ret, obj->d.atom);
			break;
#ifdef unix
		case is_32bit_dbl_80:
			set_atom80(ret, obj->d.longatom);
			break;
		//case is_32bit_integer_128:
		//	set_integer128(ret, obj->quadinteger);
		//	break;
		//case is_32bit_uinteger_128:
		//	set_uinteger128(ret, obj->uquadinteger);
		//	break;
#ifdef WANT_FLOAT128
		case is_32bit_dbl_128:
			set_atom128(ret, obj->d.quadatom);
			break;
#endif
#endif
		case is_32bit_sequence:
			set_sequence(ret, obj->d.sequence, is_expand, 1, 0, NULL, 0);
			break;
#endif
#ifdef BITS64
		case is_64bit_float:
			set_float32(ret, obj->d.float32);
			break;
		case is_64bit_integer:
			set_integer32(ret, obj->d.integer);
			break;
		case is_64bit_uinteger:
			set_uinteger32(ret, obj->d.uinteger);
			break;
		case is_64bit_long:
			set_integer64(ret, obj->d.longinteger);
			break;
		case is_64bit_ulong:
			set_uinteger64(ret, obj->d.ulonginteger);
			break;
		case is_64bit_atom:
			set_atom64(ret, &obj->d.atom);
			break;
#ifdef unix
		case is_64bit_dbl_80:
			set_atom80(ret, obj->d.longatom);
			break;
#ifdef WANT_INT128
		case is_64bit_integer_128:
			set_integer128(ret, obj->d.quadinteger);
			break;
		case is_64bit_uinteger_128:
			set_uinteger128(ret, obj->d.uquadinteger);
			break;
#endif
#ifdef WANT_FLOAT128
		case is_64bit_dbl_128:
			set_atom128(ret, obj->d.quadatom);
			break;
#endif
#endif
		case is_64bit_sequence:
			set_sequence(ret, obj->d.sequence, is_expand, 1, 0, NULL, 0);
			break;
#endif
		default:
			// data type not supported on this compiler, check compiler()
			error_exit(SET_OBJECT_ERROR);
			break;
		}
	}
}

/*
object* copy_objects(object* obj, longinteger is_expand, size_t len)
{
	object* ret;
	object* tmp;
	size_t i;
	tmp = allocate_objects(len, NULL);
	ret = tmp;
	for (i = 0; i < len; i++)
	{
		set_object(tmp, obj, is_expand);
		tmp++;
		obj++;
	}
	return ret;
}
*/

void set_repeat(object* ret, object* element, size_t repeat_count)
{
	sequence* seq;
	if (repeat_count > LENGTH_LIMIT)
	{
		error_exit(LENGTH_LIMIT_ERROR);
	}
	seq = (sequence*)array_allocate(1, sizeof(sequence));
	seq->c.v.count = 1; // duplicate count (there is only one, so "dup_count"==1)
	seq->t.length = repeat_count;
	seq->t.seq_type = IS_REPEAT;
	if ((element) && (repeat_count))
	{
		seq->d.objects = allocate_objects(1);
		set_object(seq->d.objects, element, 0);
	}
	ret->d.sequence = seq;
#ifdef BITS16
	ret->t.type = is_16bit_sequence;
#else
#ifdef BITS32
	ret->t.type = is_32bit_sequence;
#else
	ret->t.type = is_64bit_sequence;
#endif
#endif
}

object* new_repeat(object* element, size_t repeat_count)
{
	// make a one element sequence out of an element
	object* ret;
	ret = allocate_objects(1);
	set_repeat(ret, element, repeat_count);
	return ret;
}

object* new_memory(void * mem, size_t size)
{
	object* ret;
	ret = allocate_objects(1);
	set_memory(ret, mem, size);
	return ret;
}

object* new_string(const char* str)
{
	if (str)
	{
		return new_memory((void*)str, strlen(str) + 1);
	}
	else
	{
		return new_memory(NULL, 0);
	}
}

// What you see is what you get (WYSIWYG):

#ifdef BITS16
object* new_integer16(shortinteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_integer16(ret, i);
	return ret;
}
object* new_uinteger16(ushortinteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_uinteger16(ret, i);
	return ret;
}
#endif

object* new_atom32(float f)
{
	object* ret;
	ret = allocate_objects(1);
	set_float32(ret, f);
	return ret;
}

object* new_integer32(integer i)
{
	object* ret;
	ret = allocate_objects(1);
	set_integer32(ret, i);
	return ret;
}
object* new_uinteger32(uinteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_uinteger32(ret, i);
	return ret;
}
object* new_atom64(atom a)
{
	object* ret;
	ret = allocate_objects(1);
	set_atom64(ret, &a);
	return ret;
}
#ifndef BITS16
object* new_integer64(longinteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_integer64(ret, i);
	return ret;
}
object* new_uinteger64(ulonginteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_uinteger64(ret, i);
	return ret;
}
#endif
#ifdef unix
object* new_atom80(longatom a)
{
	object* ret;
	ret = allocate_objects(1);
	set_atom80(ret, &a);
	return ret;
}
#ifdef WANT_INT128
object* new_integer128(quadinteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_integer128(ret, i);
	return ret;
}
object* new_uinteger128(uquadinteger i)
{
	object* ret;
	ret = allocate_objects(1);
	set_uinteger128(ret, i);
	return ret;
}
#endif
#ifdef WANT_FLOAT128
object* new_atom128(quadatom a)
{
	object* ret;
	ret = allocate_objects(1);
	set_atom128(ret, &a);
	return ret;
}
#endif
#endif

object* new_object(object* old_object)
{
	object* ret;
	ret = allocate_objects(1);
	set_object(ret, old_object, 0);
	return ret;
}

size_t is_sequence(object* ob)
{
#ifdef BITS16
	if (ob->t.type != is_16bit_sequence)
#endif
#ifdef BITS32
	if (ob->t.type != is_32bit_sequence)
#endif
#ifdef BITS64
	if (ob->t.type != is_64bit_sequence)
#endif
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

size_t get_length(sequence* seq)
{
	return seq->t.length;
}

size_t length_of(object* ob)
{
#ifdef BITS16
	if (ob->t.type == is_16bit_sequence)
#endif
#ifdef BITS32
	if (ob->t.type == is_32bit_sequence)
#endif
#ifdef BITS64
	if (ob->t.type == is_64bit_sequence)
#endif
	{
		return get_length(ob->d.sequence);
	}
	return 1; // not a sequence, return 1
}

size_t eu_check_length_seq(sequence* s, size_t i)
{
	size_t length;
	length = get_length(s);
	if ((i > length) || (i == 0))
	{
		error_exit(LENGTH_RANGE_ERROR);
	}
	return length;
}

size_t eu_check_length(object* ob, size_t i)
{
#ifdef BITS16
	if (ob->t.type != is_16bit_sequence)
#endif
#ifdef BITS32
	if (ob->t.type != is_32bit_sequence)
#endif
#ifdef BITS64
	if (ob->t.type != is_64bit_sequence)
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	return eu_check_length_seq(ob->d.sequence, i);
}

void set_seq_id(object* dst, size_t id)
{
#ifdef BITS16
	if (dst->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if (dst->t.type != is_32bit_sequence)
#else
	if (dst->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	dst->d.sequence->id = id;
}

size_t get_seq_id(object* dst)
{
#ifdef BITS16
	if (dst->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if (dst->t.type != is_32bit_sequence)
#else
	if (dst->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	return dst->d.sequence->id;
}

/*
linked_list* join_linked_lists(linked_list* lists, size_t number)
{
	linked_list* ret;
	node* next;
	node* prev;
	size_t i;
	ret = (linked_list*)array_allocate(1, sizeof(linked_list));
	if (number == 0)
	{
		return ret;
	}
	ret->first = lists->first;
	for (i = 0; i < number; i++)
	{
		prev = lists->last;

		lists++;

		next = lists->first;
		prev->next = next;
		next->prev = prev;
	}

	ret->last = lists->last;
	return ret;
}
*/

node* eu_at_ref_list(linked_list* list, size_t i, size_t length)
{
	node* mid;
	size_t pos;
	if (i == 0)
	{
		return NULL;
	}
	if (i > length)
	{
		return NULL;
	}
	pos = list->pos;
	if (i == pos) // mid
	{
		return list->mid;
	}
	if (i == 1) // first
	{
		return list->first;
	}
	if (i == length) // last
	{
		return list->last;
	}
	mid = list->mid;
	// if i is closer to first
	if (pos - i > i)
	{
		pos = 1;
		mid = list->first;
	}
	// if i is closer to last
	else if (i - pos > length - i)
	{
		pos = length;
		mid = list->last;
	}
	while (pos < i)
	{
		// seek next
		mid = mid->next;
		pos++;
	}
	while (pos > i)
	{
		// seek previous
		mid = mid->prev;
		pos--;
	}
	list->pos = pos;
	list->mid = mid;
	return mid;
}

object* eu_at_ref_seq(sequence* s, size_t i)
{
	//done.
	// read-only, DOES NOT ALLOW STRING DATATYPE INDEXING, use get_object_eu_at() instead.
	node* n1;
	size_t length;
	length = eu_check_length_seq(s, i);
	switch (s->t.seq_type)
	{
	case IS_REPEAT:
		return s->d.objects; // &s->objects[0]
	case IS_ARRAY:
		// Turn it into C code, where offsets start at 0.
		i--;
		return &s->d.objects[i];
	case IS_LINKED_LIST:
		n1 = eu_at_ref_list(s->d.list, i, length);
		return n1->data;
	case IS_MEMORY:
		return NULL; // Use "get_object_eu_at()" instead.
		//object ret;
		//ret.integer = (integer)s->string[i];
		//ret.flags = IS_INTEGER;
		//return ret;
	default:
		error_exit(UNSUPPORTED_ERROR);
		break;
	}
	return NULL;
}

object* eu_at_ref(object* ob, size_t i)
{
#ifdef BITS16
	if (ob->t.type != is_16bit_sequence)
#endif
#ifdef BITS32
	if (ob->t.type != is_32bit_sequence)
#endif
#ifdef BITS64
	if (ob->t.type != is_64bit_sequence)
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	return eu_at_ref_seq(ob->d.sequence, i);
}

object* eu_new_at_seq(sequence* s, size_t i)
{
	object* ret;
	ret = new_object(eu_at_ref_seq(s, i));
	if (s->t.seq_type == IS_MEMORY) // previous line doesn't do anything if IS_MEMORY, do it below:
	{
		i--;
#ifdef BITS16
		set_integer16(ret, (shortinteger)s->d.memory[i]);
		ret->t.type = is_16bit_short;
#else
		set_integer32(ret, (integer)s->d.memory[i]);
#ifdef BITS32
		ret->t.type = is_32bit_integer;
#else
		ret->t.type = is_64bit_integer;
#endif
#endif
	}
	return ret;
}

object* eu_new_at(object* ob, size_t i)
{
#ifdef BITS16
	if (ob->t.type != is_16bit_sequence)
#endif
#ifdef BITS32
	if (ob->t.type != is_32bit_sequence)
#endif
#ifdef BITS64
	if (ob->t.type != is_64bit_sequence)
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	return eu_new_at_seq(ob->d.sequence, i);
}

object get_object_eu_at_seq(sequence* seq, size_t i)
{
	// read-only, allows string datatype indexing
	object* ptr;
	object ret;
	eu_check_length_seq(seq, i);
	ret.t.type_value = 0;
	ret.d.sequence = NULL;
	if (seq->t.seq_type == IS_MEMORY)
	{
		i--;
#ifdef BITS16
		ret.d.shortinteger = (shortinteger)seq->d.memory[i];
		ret.t.type = is_16bit_short;
#else
		ret.d.integer = (integer)seq->d.memory[i];
#ifdef BITS32
		ret.t.type = is_32bit_integer;
#else
		ret.t.type = is_64bit_integer;
#endif
#endif
		return ret;
	}
	ptr = eu_at_ref_seq(seq, i);
	if (ptr == NULL)
	{
		return ret;
	}
	else
	{
		return *ptr;
	}
}

object get_object_eu_at(object ob, size_t i)
{
	//done.
#ifdef BITS16
	if (ob.t.type != is_16bit_sequence)
#endif
#ifdef BITS32
	if (ob.t.type != is_32bit_sequence)
#endif
#ifdef BITS64
	if (ob.t.type != is_64bit_sequence)
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	return get_object_eu_at_seq(ob.d.sequence, i);
}

int is_linked_list(sequence* seq)
{
	return seq->t.seq_type == IS_LINKED_LIST;
}

object* convert_to_linked_list(sequence* seq)
{
	if (is_linked_list(seq))
	{
		return NULL; // no conversion made
	}
	object* ret;
	ret = allocate_objects(1);
	set_sequence(ret, seq, 1, 1, 0, NULL, 1);
	return ret;
}

object* convert_from_linked_list(sequence* seq)
{
	if (!is_linked_list(seq))
	{
		return NULL; // no conversion made
	}
	object* ret;
	ret = allocate_objects(1);
	set_sequence(ret, seq, 1, 1, 0, NULL, -1);
	return ret;
}

/*
object* copy_entire_object(object* obj)
{
	// for editing an object, read-write access
	object* ret;
	ret = copy_objects(obj, -1, 1);
	return ret;
}
*/

object* eu_copy_entire_sequence(sequence* old_dup, int depth, size_t start, size_t stop, object* ob)
{
	// eu_copy_entire_sequence to a new object, with dup->count equalling 1
	// replaces ob's value at "index"
	object* ret;
	ret = allocate_objects(1);
	set_sequence(ret, old_dup, depth, start, stop, ob, 0);
	return ret;
}

void eu_assign(object* dst, object* from, int depth)
{
	deallocate_object(dst);
	set_object(dst, from, depth); // depth should be zero (0) most of the time.
}

void eu_assign_range(object** dst, size_t start, size_t stop, object* ob)
{
	object* tmp;
	sequence* duplicate;
	size_t length;
#ifdef BITS16
	if ((*dst)->t.type != is_16bit_sequence)
#endif
#ifdef BITS32
	if ((*dst)->t.type != is_32bit_sequence)
#endif
#ifdef BITS64
	if ((*dst)->t.type != is_64bit_sequence)
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	duplicate = (*dst)->d.sequence;
	length = duplicate->t.length;
	if ((start == 0) || (start > stop + 1) || (stop > length))
	{
		error_exit(INDEX_RANGE_ERROR);
	}
	// optimize for linked_list:
	if ((duplicate->c.v.count == 1) && (duplicate->t.seq_type == IS_LINKED_LIST))
	{
		//if (duplicate->is_linked_list)
		node* last1, * first2, * first3, * last3;
		first2 = eu_at_ref_list(duplicate->d.list, stop + 1, length);
		//if (first2)
		//{
		//	first2 = first2->next;
		//}
		last1 = eu_at_ref_list(duplicate->d.list, start - 1, length);
		//if (n1)
		//{
		//	n1 = n1->prev;
		//}
		if (start != stop + 1)
		{
			// delete range
			deallocate_list(duplicate->d.list, start, stop, duplicate->t.length);
			duplicate->t.length -= (stop - start);
			duplicate->t.length--;
		}
		if (ob)
		{
#ifdef BITS16
			if (ob->t.type != is_16bit_sequence)
#endif
#ifdef BITS32
			if (ob->t.type != is_32bit_sequence)
#endif
#ifdef BITS64
			if (ob->t.type != is_64bit_sequence)
#endif
			{
				first3 = (node*)array_allocate(1, sizeof(node));
				first3->data = new_object(ob);
				last3 = first3;
				duplicate->t.length++;
			}
			else
			{
				size_t len;
				len = ob->d.sequence->t.length;
				last3 = range_to_nodes(&first3, ob->d.sequence, 1, len);
				duplicate->t.length += len;
			}
		}
		else
		{
			// (first and last's) (next and prev) need to point to NULL
			first3 = NULL;
			last3 = NULL;
		}
		if (last1)
		{
			if (first3)
			{
				first3->prev = last1;
				last1->next = first3;
			}
			else if (first2)
			{
				first2->prev = last1;
				last1->next = first2;
			}
			else
			{
				last1->next = NULL;
			}
		}
		if (first2)
		{
			if (last3)
			{
				first2->prev = last3;
				last3->next = first2;
			}
		}
		if (start == 1)
		{
			// first needs to change
			if (first3)
			{
				duplicate->d.list->first = first3; // or n2
			}
			else
			{
				duplicate->d.list->first = first2;
				first2->prev = NULL;
			}
		}
		if (stop == length)
		{
			// last needs to change
			if (last3)
			{
				duplicate->d.list->last = last3; // or n1
			}
			else
			{
				duplicate->d.list->last = last1;
				last1->next = NULL;
			}
		}
		duplicate->d.list->mid = first3; // that's what we inserted
		if (first3)
		{
			duplicate->d.list->pos = start; // position where we inserted
		}
		else
		{
			duplicate->d.list->pos = 0;
		}
		return;
	}
	// should work with all features now, still working on depth variable.
	tmp = eu_copy_entire_sequence(duplicate, 1, start, stop, ob);
//	reserved = (*dst)->reserved_internal_id;
	deallocate_object(*dst); //delete original
	**dst = *tmp; // copy object struct
//	(*dst)->reserved_internal_id = reserved;
	free_allocate(tmp);
}

void eu_assign_splice(object** dst, size_t index, object* ob)
{
	eu_assign_range(dst, index, index - 1, ob);
}

void eu_assign_insert(object** dst, size_t index, object* ob)
{
	object* obj;
	obj = new_repeat(ob, 1);
	eu_assign_splice(dst, index, ob);
	free_object(obj);
}

void eu_assign_at(object** dst, size_t index, object* ob)
{
	// should work with all features now, still working on depth variable.
	object* tmp, * obj;
	sequence* duplicate;
	eu_check_length(*dst, index);
	duplicate = (*dst)->d.sequence;
	if ((IS_DEQUE(*duplicate))
		|| ((duplicate->c.v.count == 1)
			&& ((duplicate->t.seq_type == IS_ARRAY)
			 || (duplicate->t.seq_type == IS_LINKED_LIST))))
	{
		//if (duplicate->is_array) // || (duplicate->is_linked_list))
		tmp = eu_at_ref(*dst, index);
		eu_assign(tmp, ob, 0);
		return;
	}
	obj = new_repeat(ob, 1);
	tmp = eu_copy_entire_sequence(duplicate, 1, index, index, obj);
	free_object(obj);
//	reserved = (*dst)->reserved_internal_id;
	deallocate_object(*dst); //delete original
	**dst = *tmp;
//	(*dst)->reserved_internal_id = reserved;
	free_allocate(tmp);
}

void eu_assign_memory_at(object** dst, size_t index, byte ch)
{
	sequence* duplicate;
	eu_check_length(*dst, index);
	duplicate = (*dst)->d.sequence;
	if ((duplicate->t.seq_type == IS_MEMORY)
		&& (duplicate->c.v.count == 1))
	{
		duplicate->d.memory[index - 1] = ch;
		return;
	}
	else
	{
		object ob;
#ifdef BITS16
		set_integer16(&ob, (shortinteger)ch);
#else
		set_integer32(&ob, (integer)ch);
#endif
		eu_assign_at(dst, index, &ob); // use "eu_assign_range()" so as not to use "repeat()"
		return;
	}
}

void make_deque(object** dst, size_t type, int pop_before, int pop_after)
{
	// converts a sequence into a deque
	// NOTE: deques are both "is_deque" and "is_array"
	// type==0 for empty deque
	// type==1 for prepend
	// type==2 for append
	// type==3 for deque (both prepend and append)
	object* tmp;
	sequence* sequence;
	size_t len;
#ifdef BITS16
	if ((*dst)->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if ((*dst)->t.type != is_32bit_sequence)
#else
	if ((*dst)->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	if ((*dst)->d.sequence->t.seq_type == IS_LINKED_LIST)
	{
		tmp = convert_from_linked_list((*dst)->d.sequence);
		deallocate_object(*dst);
		**dst = *tmp;
		free_allocate(tmp);
	}

	if (type == NO_DEQUE)
	{
		eu_assign_range(dst, 1, 0, NULL); // NOTE: needs to return an array.
	}
	else
	{
		tmp = new_repeat(NULL, chunk);
		if (type & PREPEND_DEQUE)
		{
			eu_assign_range(dst, 1, pop_before, tmp); // NOTE: needs to return an array.
		}
		len = (*dst)->d.sequence->t.length;
		if (type & APPEND_DEQUE)
		{
			eu_assign_range(dst, len + 1 - pop_after, len, tmp); // NOTE: needs to return an array.
		}
		free_object(tmp);
	}
	sequence = (*dst)->d.sequence;
	sequence->c.v.count = 0;
	if (type & PREPEND_DEQUE)
	{
		sequence->c.d.before = chunk;
		sequence->t.length -= chunk;
		sequence->d.objects += chunk;
	}
	if (type & APPEND_DEQUE)
	{
		sequence->c.d.after = chunk;
		sequence->t.length -= chunk;
	}
	sequence->c.v.deque_flags = type;
	sequence->t.seq_type = IS_ARRAY;
}

void prepend(object* dst, object* ele)
{
	sequence* sequence;
#ifdef BITS16
	if (dst->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if (dst->t.type != is_32bit_sequence)
#else
	if (dst->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	if (!IS_DEQUE(*dst->d.sequence))
	{
		if (dst->d.sequence->t.seq_type == IS_LINKED_LIST)
		{
			eu_assign_insert(&dst, 1, ele);
			return;
		}
		make_deque(&dst, PREPEND_DEQUE, 0, 0);
		//tmp = new_repeat(NULL, NULL, chunk);
		//eu_assign_range(&dst, 1, 0, tmp); // NOTE: needs to return an array.
		//free_object(tmp);
		//sequence = dst->sequence; // dst changed, set again
		//sequence->count = 0;
		//sequence->before = chunk;
		//sequence->length -= chunk;
		//sequence->objects += chunk; // here
		//sequence->is_deque = 1;
	}
	else
	{
		if (dst->d.sequence->c.d.before == 0)
		{
			if (dst->d.sequence->c.v.deque_flags & APPEND_DEQUE)
			{
				make_deque(&dst, BOTH_DEQUE, 0, 0);
			}
			else
			{
				make_deque(&dst, PREPEND_DEQUE, 0, 0);
			}
		}
	}
	sequence = dst->d.sequence;
	sequence->c.d.before--;
	sequence->d.objects--;
	set_object(sequence->d.objects, ele, 0);
	sequence->t.length++;
	dst->d.sequence = sequence;
}
void append(object* dst, object* ele)
{
	sequence* sequence;
#ifdef BITS16
	if (dst->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if (dst->t.type != is_32bit_sequence)
#else
	if (dst->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	if (!IS_DEQUE(*dst->d.sequence))
	{
		if (dst->d.sequence->t.seq_type == IS_LINKED_LIST)
		{
			eu_assign_insert(&dst, dst->d.sequence->t.length + 1, ele);
			return;
		}
		make_deque(&dst, APPEND_DEQUE, 0, 0);
		//tmp = new_repeat(NULL, NULL, chunk);
		//eu_assign_range(&dst, len + 1, len, tmp); // NOTE: needs to return an array.
		//free_object(tmp);
		//sequence = dst->sequence; // dst changed, set again
		//sequence->count = 0;
		//sequence->after = chunk;
		//sequence->length -= chunk;
		//sequence->is_deque = 1;
	}
	else
	{
		if (dst->d.sequence->c.d.after == 0)
		{
			if (dst->d.sequence->c.v.deque_flags & PREPEND_DEQUE)
			{
				make_deque(&dst, BOTH_DEQUE, 0, 0);
			}
			else
			{
				make_deque(&dst, APPEND_DEQUE, 0, 0);
			}
		}
	}
	sequence = dst->d.sequence;
	sequence->c.d.after--;
	set_object(sequence->d.objects + sequence->t.length, ele, 0);
	sequence->t.length++;
	dst->d.sequence = sequence;
}

void push_front(object* dst, object* ele)
{
	prepend(dst, ele);
}
void push_back(object* dst, object* ele)
{
	append(dst, ele);
}

// Interesting, but I'm not sure I want to have these functions:
// pop_front, delete first element -- might be do-able
// pop_back, delete last element -- might be do-able

void pop_front(object* dst)
{
	sequence* seq;
#ifdef BITS16
	if (dst->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if (dst->t.type != is_32bit_sequence)
#else
	if (dst->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	if (!IS_DEQUE(*dst->d.sequence))
	{
		if (dst->d.sequence->t.seq_type == IS_LINKED_LIST)
		{
			eu_assign_range(&dst, 1, 1, NULL);
			return;
		}
		make_deque(&dst, NO_DEQUE, 0, 0);
		dst->d.sequence->c.v.deque_flags = PREPEND_DEQUE;
	}
	seq = dst->d.sequence;
	if (seq->t.length)
	{
		if (seq->c.d.before == CHUNK_LIMIT)
		{
			make_deque(&dst, seq->c.d.after ? BOTH_DEQUE : PREPEND_DEQUE, 1, 0);
		}
		else
		{
			seq->t.length--;
			deallocate_object(seq->d.objects);
			seq->d.objects++;
			seq->c.d.before++;
		}
	}
}
void pop_back(object* dst)
{
	sequence* seq;
#ifdef BITS16
	if (dst->t.type != is_16bit_sequence)
#else
#ifdef BITS32
	if (dst->t.type != is_32bit_sequence)
#else
	if (dst->t.type != is_64bit_sequence)
#endif
#endif
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
	if (!IS_DEQUE(*dst->d.sequence))
	{
		if (dst->d.sequence->t.seq_type == IS_LINKED_LIST)
		{
			size_t len;
			len = dst->d.sequence->t.length;
			eu_assign_range(&dst, len, len, NULL);
			return;
		}
		make_deque(&dst, NO_DEQUE, 0, 1);
		dst->d.sequence->c.v.deque_flags = APPEND_DEQUE;
	}
	seq = dst->d.sequence;
	if (seq->t.length)
	{
		if (seq->c.d.after == CHUNK_LIMIT)
		{
			make_deque(&dst, seq->c.d.after ? BOTH_DEQUE : APPEND_DEQUE, 0, 1);
		}
		else
		{
			seq->t.length--;
			deallocate_object(seq->d.objects + seq->t.length);
			seq->c.d.after++;
		}
	}
}


void fprint_obj(FILE * outFile, object* obj, int options)
{
	// print an object to: FILE * outFile;
	size_t i, len;
	object lookat;
#ifdef unix
#ifdef WANT_FLOAT128
	int prec = 33; // it has to be 33, for greater accuracy with float128.
	int width = 0;
	size_t size = 128;
	char s[128];
#endif // WANT_FLOAT128
#ifdef WANT_INT128
	unionquad quad;
#endif
#endif
	if (obj)
	{
		switch (obj->t.type)
		{
#ifdef BITS16
		case is_16bit_sequence:
			fprintf(outFile, "{");
			len = obj->d.sequence->t.length;
			if (len)
			{
				lookat = get_object_eu_at(*obj, 1);
				fprint_obj(outFile, &lookat, options);
			}
			for (i = 2; i <= len; i++)
			{
				fprintf(outFile, ",");
				lookat = get_object_eu_at(*obj, i);
				fprint_obj(outFile, &lookat, options);
			}
			fprintf(outFile, "}");
			break;
		case is_16bit_short:
			fprintf(outFile, "%i", obj->d.shortinteger);
			break;
		case is_16bit_ushort:
			fprintf(outFile, "%u", obj->d.ushortinteger);
			break;
		case is_16bit_float:
			fprintf(outFile, "%f", *obj->d.float32);
			break;
		case is_16bit_integer:
			fprintf(outFile, "%li", *obj->d.integer);
			break;
		case is_16bit_uinteger:
			fprintf(outFile, "%lu", *obj->d.uinteger);
			break;
		case is_16bit_atom:
			fprintf(outFile, "%g", *obj->d.atom);
			break;
#endif
#ifdef BITS32
		case is_32bit_float:
			fprintf(outFile, "%f", obj->d.float32);
			break;
		case is_32bit_integer:
			fprintf(outFile, "%i", obj->d.integer);
			break;
		case is_32bit_uinteger:
			fprintf(outFile, "%u", obj->d.uinteger);
			break;
		case is_32bit_long:
			fprintf(outFile, "%lli", *obj->d.longinteger);
			break;
		case is_32bit_ulong:
			fprintf(outFile, "%llu", *obj->d.ulonginteger);
			break;
		case is_32bit_atom:
			fprintf(outFile, "%g", *obj->d.atom);
			break;
#ifdef unix
		case is_32bit_dbl_80:
			fprintf(outFile, "%Lg", *obj->d.longatom);
			break;
//#ifdef WANT_INT128
//		case is_32bit_integer_128: // fallthru
//		case is_32bit_uinteger_128:
//			quad.u = *obj->uquadinteger;
//			fprintf(outFile, "0x%016llx%016llx", quad.high, quad.low);
//			break;
//#endif
#ifdef WANT_FLOAT128
		case is_32bit_dbl_128:
			//printf("%Qe", 1.2Q);
			i = quadmath_snprintf(s, size, "%+-#*.*Qe", width, prec, *obj->d.quadatom);
			fprintf(outFile, "%s", s); // printf("%s", s);
			break;
#endif
#endif
		case is_32bit_sequence:
			fprintf(outFile, "{");
			len = obj->d.sequence->t.length;
			if (len)
			{
				lookat = get_object_eu_at(*obj, 1);
				fprint_obj(outFile, &lookat, options);
			}
			for (i = 2; i <= len; i++)
			{
				fprintf(outFile, ",");
				lookat = get_object_eu_at(*obj, i);
				fprint_obj(outFile, &lookat, options);
			}
			fprintf(outFile, "}");
			break;
#endif
#ifdef BITS64
		case is_64bit_float:
			fprintf(outFile, "%f", obj->d.float32);
			break;
		case is_64bit_integer:
			fprintf(outFile, "%i", obj->d.integer);
			break;
		case is_64bit_uinteger:
			fprintf(outFile, "%u", obj->d.uinteger);
			break;
		case is_64bit_long:
			fprintf(outFile, "%lli", obj->d.longinteger);
			break;
		case is_64bit_ulong:
			fprintf(outFile, "%llu", obj->d.ulonginteger);
			break;
		case is_64bit_atom:
			fprintf(outFile, "%g", obj->d.atom);
			break;
#ifdef unix
		case is_64bit_dbl_80:
			fprintf(outFile, "%Lg", *obj->d.longatom);
			break;
#ifdef WANT_INT128
		case is_64bit_integer_128: // fallthru
		case is_64bit_uinteger_128:
			quad.u = *obj->d.uquadinteger;
			fprintf(outFile, "0x%016llx%016llx", quad.high, quad.low);
			break;
#endif
#ifdef WANT_FLOAT128
		case is_64bit_dbl_128:
			//printf("%Qe", 1.2Q);
			i = quadmath_snprintf(s, size, "%+-#*.*Qe", width, prec, *obj->d.quadatom);
			fprintf(outFile, "%s", s); // printf("%s", s);
			break;
#endif
#endif
		case is_64bit_sequence:
			fprintf(outFile, "{");
			len = obj->d.sequence->t.length;
			if (len)
			{
				lookat = get_object_eu_at(*obj, 1);
				fprint_obj(outFile, &lookat, options);
			}
			for (i = 2; i <= len; i++)
			{
				fprintf(outFile, ",");
				lookat = get_object_eu_at(*obj, i);
				fprint_obj(outFile, &lookat, options);
			}
			fprintf(outFile, "}");
			break;
#endif
		default:
			error_exit(DATA_CORRUPTION_ERROR);
			break;
		}
#ifdef BITS16
		if ((options == 1) && (obj->t.type == is_16bit_integer))
		{
			shortinteger i;
			i = obj->d.shortinteger;
#else
#ifdef BITS32
		if ((options == 1) && (obj->t.type == is_32bit_integer))
#else
		if ((options == 1) && (obj->t.type == is_64bit_integer))
#endif
		{
			integer i;
			i = obj->d.integer;
#endif
			if ((i >= 32) && (i <= 127)) // text file ascii range
			{
				fprintf(outFile, "\'%c\'", (char)i);
			}
		}
	}
}

void print(object* obj)
{
	fprint_obj(stdout, obj, 0);
	printf("\n");
}

void pretty_print(object* obj)
{
	fprint_obj(stdout, obj, 1);
	printf("\n");
}

void print_string(object* obj)
{
#ifdef BITS16
	if (obj->t.type == is_16bit_sequence)
#else
#ifdef BITS32
	if (obj->t.type == is_32bit_sequence)
#else
	if (obj->t.type == is_64bit_sequence)
#endif
#endif
	{
		size_t i, len;
		object ele;
		int ch;
		// might need to work on this.

		len = obj->d.sequence->t.length;
		for (i = 1; i <= len; i++)
		{
			ele = get_object_eu_at(*obj, i);
#ifdef BITS16
			ch = ele.d.shortinteger;
#else
			ch = ele.d.integer;
#endif
			if (((ch >= 32) && (ch <= 127)) || (ch == 9) || (ch == 10) || (ch == 13)) // screen ascii range
			{
				printf("%c", (char)ch);
			}
		}
	}
	else
	{
		error_exit(NOT_SEQUENCE_ERROR);
	}
}


// Get functions:

int get_type(object* ob)
{
	// "get_type()" first, then use the other "get_" functions
	return ob->t.type_value;
}

#ifndef BITS16
#ifdef unix
#ifdef WANT_FLOAT128
quadatom* get_quadatom(object* ob)
{
	return ob->d.quadatom;
}
#endif // WANT_FLOAT128
#ifdef WANT_INT128
quadinteger* get_quadinteger(object* ob)
{
	return ob->d.quadinteger;
}
uquadinteger* get_uquadinteger(object* ob)
{
	return ob->d.uquadinteger;
}
#endif
longatom* get_longatom(object* ob)
{
	return ob->d.longatom;
}
#endif
#ifdef BITS32
atom* get_atom(object* ob)
{
	return ob->d.atom;
}
longinteger* get_longinteger(object* ob)
{
	return ob->d.longinteger;
}
ulonginteger* get_ulonginteger(object* ob)
{
	return ob->d.ulonginteger;
}
#endif
#ifdef BITS64
atom* get_atom(object* ob)
{
	return &ob->d.atom;
}
longinteger get_longinteger(object* ob)
{
	return ob->d.longinteger;
}
ulonginteger get_ulonginteger(object* ob)
{
	return ob->d.ulonginteger;
}
#endif
integer get_integer(object* ob)
{
	return ob->d.integer; // int 32-bit
}
uinteger get_uinteger(object* ob)
{
	return ob->d.uinteger; // int 32-bit
}
float* get_float32(object* ob)
{
	return &ob->d.float32;
}

void* get_data(object* ob)
{
	if (ob->d.sequence->t.seq_type == IS_MEMORY)
	{
		return ob->d.sequence->d.memory;
	}
	else
	{
		return NULL;
	}
}
#endif

// Code for: map
// use a binary search for keys

/*
integer eu_compare(object* a, object* b)
{
	// not done yet.
	return 0;
}
integer map_find_key(object* table, object* key)
{
	//done.
	integer lo, hi, mid, c;
	lo = 1;
	hi = get_length(table);
	if (hi < 0)
	{
		puts("Error...\n");
		exit(1);
	}
	mid = 1;
	c = 0;
	while (lo <= hi)
	{
		mid = (lo + hi) >> 1; // floor((lo + hi) / 2)
		// compare(key, table[mid][1])
		c = eu_compare(key, eu_at_ref(eu_at_ref(table, mid), 1)); // returns 1, -1 or 0
		if (c < 0)
		{
			hi = mid - 1;
		}
		else if (c > 0)
		{
			lo = mid + 1;
		}
		else
		{
			return mid - 1;
		}
	}
	if (c > 0)
	{
		mid += 1;
	}
	return -mid;
}
*/
/*
void insert_self(sequence* self, object* what, integer index)
{
	//here

	
	//How to insert on a linked list:

	// 0. Find insert position in list.
	// 1. Make a new node, then copying "what"
	// 2. Take the above node and make its "next" point to the new node.
	// 3. Then, take below node and make its "prev" point to the new node.
	// 4. Then, make the new node's "prev" point to the above node.
	// 5. Then, make the new node's "next" point to the next node.

	if (self->is_linked_list)
	{
		node* above_node;
		node* below_node;
		node* n1;

		below_node = eu_at_ref_list(self, index);
		above_node = below_node->prev;
		n1 = (node*)allocate(sizeof(node));
		if (!n1)
		{
			// computer has run out of memory.
			// error has to be handled.
		}
		n1->data = copy_object(what, 0);
		above_node->next = n1;
		below_node->prev = n1;
		n1->prev = above_node;
		n1->next = below_node;

		return;
	}

	if (self->is_deque)
	{
		
		
		// shift everything
		memmove(dst, src, size);



	}

}
*/
/*
integer map_insert(object* table, object* record)
{
	integer key_location = map_find_key(table, eu_at_ref(record, 1));
	if (key_location > 0)
	{
		// key is already in the table, replace
		object* ptr;
		ptr = eu_at_ref(table, key_location);
		deallocate_object(ptr);
		set_object(ptr, record, 0);
		return key_location;
	}
	else
	{
		key_location = -key_location;

		//insert(sequence target, objects what, integer index)

		insert_self(table->sequence, record, key_location);
		return key_location;
	}
}
*/

// end of file.
