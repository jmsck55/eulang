
// Description: "Euphoria-like", written in "pure-C".
// eulang.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// It needs Unit Testing.


// I need to test linked_lists.


// NOTE TO SELF: Work on programming until you can't work anymore


// Plan: Make it work like Euphoria, but with my modifications and additions.

// It will be a dynamic link library or a static library as a backend for applications, or as a programming language.

// Make it more readable, more user friendly to the average programmer.


// work on full text debugger.

#include "pch.h"

// Include file:
#include "eulang.h"

int main()
{
	object* a = NULL, * b = NULL, * c = NULL, * d = NULL, * e = NULL, * f = NULL, * unnamed, * named;
	object* ele, * s, * list;
	byte buff[] = "This is a test.\n";

	init(200); // try changing "init()" more than once.

	printf("Eulang demo program\n");
	printf("Eulang Version: %u\n", version());

	// unit testing:

	ele = new_atom64(1.23);
	s = new_repeat(ele, 3);
	list = convert_to_linked_list(s->d.sequence);

	free_object(ele);
	ele = new_atom64(3.14);

	eu_assign_at(&list, 2, ele);

	append(list, ele);
	prepend(list, ele);

//	eu_assign_range(&list, length_of(list) + 1, length_of(list), ele);
//	eu_assign_range(&list, 1, 0, ele);

	print(list);
	printf("\n");

	pop_front(list);
	pop_back(list);

	print(list);
	printf("\n");


	free_object(ele);
	free_object(s);
	free_object(list);

	// error_exit(1);

	// "eu_assign_memory_at()" example:

	f = new_string("Replace [x]\n");
	print_string(f);
	eu_assign_memory_at(&f, 10, 'y');
	print_string(f);
	a = new_object(f); // new "object" from another "object"
	eu_assign_memory_at(&f, 10, 'z');
	print_string(f);
	print_string(a);
	free_object(a);

	// "append" and "prepend" example:

	b = new_integer32(1);
	c = new_integer32(2);
	d = new_integer32(3);
	e = new_repeat(b, 4);

	pop_front(e);
	pop_back(e);

	print(e);

	a = new_repeat(NULL, 0);

	prepend(a, b);
	prepend(a, b);

	append(a, c);
	append(a, d);
	append(a, d);

	print(a);

	pop_front(a);
	pop_back(a);

	print(a);

	// examples:

	unnamed = new_repeat(NULL, 0); // new empty sequence without memory management.
	printf("Empty sequence: ");
	print(unnamed);

	named = new_string("My favorite variable"); // named variable, with memory management, empty string works!
	a = new_integer32(-123); // named variable, with memory management
	b = new_atom64(-1.23); // same as above, but with other data
	c = new_uinteger32(-1); // two's compilment on negative numbers
	d = new_uinteger32(0xff);
	e = new_memory(&buff, strlen(buff) + 1);
	f = new_object(e);


	printf("Some variables:\n");
	print_string(named); // no new line character
	print_string(e); // has new line character
	print(a);
	print(b);
	print(c);
	print(d);

	eu_assign_at(&e, 1, a);
	eu_assign_at(&e, 2, a);
	eu_assign_at(&e, 8, a);

	// NOTE:
	// In order to do this, "e->at(17) = e;" you must:
	// a = e;
	// e->at(17) = a;
	// Code for statements below:
	eu_assign(a, e, 0); // a = e; // assign to another variable,
	eu_assign_at(&e, 17, a); // e->at(17) = a; // now this works.

	print(e);

	eu_assign_range(&f, 1, 0, NULL); // pointless, copies and then deletes, accomplishing nothing
	eu_assign_range(&f, 2, 1, c); // insert at position: 2
	eu_assign_range(&f, 1, 0, c); // prepend, actually concatenates onto the beginning
	eu_assign_range(&f, f->d.sequence->t.length + 1, f->d.sequence->t.length, c); // append, actually concatenates onto the end


	print(f);


	// ABOUT FREEING VARIABLES:

	// You can free variables manually,
	free_object(a);
	free_object(b);
	free_object(c);
	free_object(d);
	free_object(e);
	free_object(f);

	free_object(unnamed);
	free_object(named);

	puts("\nEnd of demo program.\n\n");

	puts("Press Enter to exit.\n");
	if (getchar()) {}
	return 0;
}

/*

char debug_filename[] = "C:\\Euphoria41\\projects\\test.ex";
char * program_name = debug_filename;
//char * program_name = NULL;
char * sourcename;

c_block * c_gets(FILE * stream)
{
	// done.
	unsigned char * buf = (unsigned char *)malloc(CHUNK);
	c_block * ret;
	if (fgets((char *)buf, CHUNK, stream))
	{
		ret = (c_block *)malloc(sizeoc_block));
		ret->size = strlen(buf);
		ret->size++;
		if (ret->size == CHUNK)
		{
			c_block * tmp;
			tmp = c_gets(stream);
			if (tmp)
			{
				ret->size--;
				ret->ptr = (unsigned char *)malloc(ret->size + tmp->size);
				memcpy(ret->ptr + ret->size, tmp->ptr, tmp->size);
				free(tmp->ptr);
				memcpy(ret->ptr, buf, ret->size);
				//free(buf);
				ret->size += tmp->size;
				free(tmp);
			}
		}
		else
		{
			ret->ptr = (unsigned char *)malloc(ret->size);
			memcpy(ret->ptr, buf, ret->size);
			//free(buf);
		}
	}
	else
	{
		//free(buf);
		ret = NULL;
	}
	free(buf);
	return ret;
}

node * lines = NULL;
size_t line, col, len, length_of_line, flag;
char * pch;

int seek_next_char()
{
	// seek to the next non-whitespace char in file, and skip comments
	while (1)
	{
		while (isblank(pch[col]))
		{
			col++;
		}
		if ((pch[col] == '-') && (pch[col + 1] == '-'))
		{
			// ignore the rest of the line
			//flag = 0;
			col = length_of_line;
		}

		if (col >= length_of_line)
		{
			// get new line:
			lines = lines->next;
			if (!lines)
			{
				return 1; // end of file. (eof==true)
			}
			pch = (char *(((c_block*)lines->data)->ptr);
			length_of_line = strlen((const char *((c_block*)lines->data)->ptr);
			col = 0;
			line++;
		}
		else
		{
			return 0; // not the end of file. (eof==false)
		}
	}
}

char * get_identifier(int(*ignore(int), int(*isfirst(int), int (*isrest(int))
{
	size_t begin;
	flag = col;
	while ((*ignore(pch[col]))
	{
		col++;
	}
	begin = col;
	len = 0;
	if ((pch[col] == '-') && (pch[col + 1] == '-'))
	{
		// ignore the rest of the line
		flag = 0;
		col = length_of_line;
		return NULL;
	}
	if ((*isfirst(pch[col]))
	{
		begin = col;
		len = 1;
		col++;
		while (pch[col])
		{
			if (!(*isrest(pch[col]))
			{
				break;
			}
			len++;
			col++;
		}
		return pch + begin;
	}
	return NULL;
}

object read_object()
{
	// read an objects at the current position in the file.
	object ob;

	if (seek_next_char())
	{
		//here, put error_msg here.
		ob.data = 0;
		return ob;
	}

	

	//here

	return ob;
}

sequence * get_arguments(size_t num)
{
	sequence * ret = (sequence *)malloc(sizeosequence));
	size_t i;

	if (seek_next_char())
	{
		return NULL;
	}
	if (pch[col] != '(')
	{
		return NULL; // syntax error
	}



	ret->length = num;
	ret->ptr = (object *)calloc(num, sizeoobject));
	for (i = 0; i < num; i++)
	{
		ret->ptr[i] = read_object();
	}

	return ret;
	//here
}

int evaluate_expression(char want) // evaluate any expression
{
	size_t begin;
	object * arg; // pointer to an array of "objects"
	object ob;
	char ch;
	char * identifier;

	if (seek_next_char())
	{
		return 1;
	}

	ch = pch[col];

	switch (ch)
	{
	case '(':
		// begin parenthesis
		break;
	case '{':
		// begin sequence
		break;
	case '\"':
		// begin string
		break;
	case '\'':
		// begin char
		break;
	case '#':
		// begin hexadecimal
		break;
	default:
		if (isnum(ch))
		{
			// begin number
			begin = col;
			len = 1;
			col++;
			while (pch[col])
			{
				if (!isnum(pch[col]))
				{
					break;
				}
				len++;
				col++;
			}
			identifier = pch + begin;
			ob = c_value(identifier, len);

		}
		if (isalpha(ch))
		{
			// begin identifier
			begin = col;
			len = 1;
			col++;
			while (pch[col])
			{
				if (!isalnum(pch[col]))
				{
					break;
				}
				len++;
				col++;
			}
			identifier = pch + begin;

			if (strncmp(identifier, "puts", len) == 0)
			{
				print"DEBUG: Found ya==puts!\n");
				evaluate_expression('(');
				arg = get_arguments(2);
				eu_puts(arg[0], arg[1]);
			}
			else if (strncmp(identifier, "getc", len) == 0)
			{
				print"DEBUG: Found ya==getc!\n");
				evaluate_expression('(');
				arg = get_arguments(1);
				eu_getc(arg[0]);
			}
			// here, add more identifiers here...
			else
			{
				// not an identifier, must be custom
				// search in list of custom

			}
		}
		break;
	}


	//here
	return 0;
}

int main(int argc, char *argv[])
{
		//int i;
		//for (i = 0; i < argc; i++)
		//{
		//	print"[%d] %s\n", i, argv[i]);
		//}
	
	FILE * pFile;

	if (program_name == NULL)
	{
		if (argc <= 1)
		{
			print"Usage:\n eulang program.ex [arg1 [arg2 ...]]\n");
			return 0;
		}
		program_name = argv[1];
	}

	sourcename = program_name;

	pFile = fopen(sourcename, "rb");
	if (pFile == NULL)
	{
		print"Error: Could not open file \"%s\"", sourcename);
		return 0;
	}
	// read entire file into a linked-list of lines (c_block strings):
	node * file;
	c_block * bline; // c_block string
	c_linked_list_append_first = NULL;
	while (1)
	{
		bline = c_gets(pFile);
		if (!bline)
		{
			break;
		}
		if (bline->ptr[bline->size - 2] == '\n')
		{
			bline->ptr[bline->size - 2] = '\0';
			if (bline->ptr[bline->size - 3] == '\r')
			{
				bline->ptr[bline->size - 3] = '\0';
			}
		}
		c_linked_list_append(&lines, bline);
	}
	lines = c_linked_list_append_first; // rewind
	file = lines;

	// process file (lines)

#ifdef DEBUG
	print"DEBUG: first line is: %s\n", ((c_block*)lines->data)->ptr);
	print"DEBUG: Tokens in file:\n");
#endif
	
	if (lines)
	{
		pch = (char *(((c_block*)lines->data)->ptr);
		length_of_line = strlen((const char *((c_block*)lines->data)->ptr);
	}
	col = 0; // column number
	line = 1; // line number
	while (lines)
	{
#ifdef DEBUG
		print"DEBUG: (%zi) %s\n", line, ((c_block*)lines->data)->ptr + col);
#endif
		// evaluate all expressions in file:

		//pch = (char *(((c_block*)lines->data)->ptr);
		
		if (evaluate_expression('\0')) // want end-of-file.
		{
			break;
		}
		if (flag)
		{
			print"Error: Bad identifier on line %zi, column %zi in file \"%s\"\n", line, col + 1, sourcename);
			break;
		}
	}

	free_entire_c_linked_list(file, &c_block_free);

	print"DEBUG: Success!\n");
	return 0;
}
*/
// end of file.
