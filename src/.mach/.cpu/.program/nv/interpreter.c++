#include <fstream>
#include <iostream> 
//#include <string>

#include "../mathematics.h++"
#include "interpreter.h++"
//#include "B:\Documents\Visual Studio Projects\NoWare (2013)\lexical_analyzer.cpp"
//#include "lexical_analyzer.cpp"
#include <lexer/lexical_analyzer.cpp>
//#include ""
//#include <>

#ifndef ENCODING_NAME
	#define ENCODING_NAME (0x0)
#endif

LIB::language::interpreter::interpreter (void)
{

}

LIB::language::interpreter::~interpreter (void)
{

}

unsigned short int LIB::language::interpreter::enqueue (const std::string & file_name, std::queue <LIB::machine::resources::processor::instructions> & instructions_queue, std::string & error)
{
	/*
		0 success
		1 syntax error
	*/

	LIB::machine::resources::processor::instruction instruction_internal;
	LIB::machine::resources::processor::instructions instructions_internal;
	std::queue <LIB::machine::resources::processor::instructions> instructions_local;
	std::string error_internal;
	bool correct = true;
	bool serializing = false;
	bool valid_instruction;
	unsigned short int visits;
	LIB::mathematics::numbers::natural passes;
	std::string identifier, output;

	quex::Token * token_p = 0x0;
	// (*) create the lexical analyser
	//     1st arg: input file, default = 'example.txt'
	//     2nd arg: input character encoding name, 0x0 --> no codec conversion
	//quex::lexical_analyzer qlex (file_name, ENCODING_NAME);
	quex::lexical_analyzer qlex (file_name);

	//unsigned short int number_of_tokens = 0;

	do
	{
		// (*) get next token from the token stream
		qlex.receive (& token_p);

		//std::cout << "Current Token: " << token_p -> get_string () << ";" << std::endl;

		// (*) print out token information
		//std::cout << token_p -> type_id_name () << ": " << token_p -> pretty_char_text () << ": " << token_p -> get_number () << std::endl;

		//cout << "String: " << token_p -> get_string () << endl;

		switch (token_p -> type_id ())
		{
			case QUEX_TKN_SERIALIZE:
				// Get the next token.
				qlex.receive (& token_p);

				// ...it should be a left brace ({).
				if (token_p -> type_id () != QUEX_TKN_BRACE_BEGINNING)
				{
					error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected {, received " + token_p -> pretty_char_text ();

					return 1;
				}

				serializing = true;

				break;

			case QUEX_TKN_BRACE_END:
				if (serializing)
					serializing = false;
				else
				{
					error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unexpected character: }";

					return 1;
				}

				break;

			case QUEX_TKN_INCLUDE:
				// Now, parse the input file.

				// Get the next token.
				qlex.receive (& token_p);

				// ...it should be a double quote.
				if (token_p -> type_id () == QUEX_TKN_STRING_RELATIVE_BEGINNING || token_p -> type_id () == QUEX_TKN_STRING_ABSOLUTE_BEGINNING)
				{
					std::string file_name_internal = "";

					qlex.receive (& token_p);

					while (token_p -> type_id () != QUEX_TKN_TERMINATION && token_p -> type_id () != QUEX_TKN_FAILURE && token_p -> type_id () != QUEX_TKN_STRING_RELATIVE_END && token_p -> type_id () != QUEX_TKN_STRING_ABSOLUTE_END)
					{
						//if (token_p -> type_id () == QUEX_TKN_CHAR)
							file_name_internal += token_p -> pretty_char_text ();

						qlex.receive (& token_p);
					}
				/*
					do
					{
						qlex.receive (& token_p);

						if (token_p -> type_id () == QUEX_TKN_CHAR)
							file_name_internal += token_p -> pretty_char_text ();
					}
					while (token_p -> type_id () != QUEX_TKN_TERMINATION && token_p -> type_id () != QUEX_TKN_FAILURE && token_p -> type_id () != QUEX_TKN_STRING_END);
				*/	

					if (token_p -> type_id () == QUEX_TKN_STRING_RELATIVE_END || token_p -> type_id () == QUEX_TKN_STRING_RELATIVE_END)
					{
						unsigned short int ret = enqueue (file_name_internal, instructions_local, error_internal);

						if (ret != 0)
						{
							error = error_internal;

							return ret;
						}
					}
					else if (token_p -> type_id () == QUEX_TKN_TERMINATION)
					{
						error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unexpected end of file";

						return 1;
					}
					else if (token_p -> type_id () == QUEX_TKN_FAILURE)
					{
						error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected \", received " + token_p -> pretty_char_text ();

						return 1;
					}
					else
					{
						error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unknown error";

						return 1;
					}
				}
				else
				{
					error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected \", received " + token_p -> pretty_char_text ();

					return 1;
				}

				break;

			case QUEX_TKN_OUTPUT:
				instruction_internal.action = LIB::machine::devices::processor::operation::output;

				do
				{
					qlex.receive (& token_p);

					//std::cout << "Current Token O/P: " << token_p -> get_string () << ";" << std::endl;	// Temporary.

					valid_instruction = true;

					switch (token_p -> type_id ())
					{
						case QUEX_TKN_IDENTIFIER_VARIABLE:
							instruction_internal.inputs [0] = token_p -> pretty_char_text ();

							break;
						case QUEX_TKN_ENDL:
							instruction_internal.inputs [0] = /*(std::string) */"endl";

							break;
						case QUEX_TKN_TAB:
							instruction_internal.inputs [0] = /*(std::string) */"tab";

							break;
						case QUEX_TKN_STRING_ABSOLUTE_BEGINNING:
							output = "";

							qlex.receive (& token_p);

							while (token_p -> type_id () != QUEX_TKN_TERMINATION && token_p -> type_id () != QUEX_TKN_FAILURE && token_p -> type_id () != QUEX_TKN_STRING_ABSOLUTE_END)
							{
								//if (token_p -> type_id () == QUEX_TKN_CHAR)
									output += token_p -> pretty_char_text ();

								qlex.receive (& token_p);
							}
							/*
							if (token_p -> type_id () == QUEX_TKN_STRING_END)
							{
								qlex.receive (& token_p);

								if (! token_p -> type_id () != QUEX_TKN_STATEMENT_END)
								{
									error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected ;, received " + token_p -> pretty_char_text ();

									return 1;
								}
							}
							else if (token_p -> type_id () == QUEX_TKN_TERMINATION)
							{
								error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unexpected end of file";

								return 1;
							}
							else if (token_p -> type_id () == QUEX_TKN_FAILURE)
							{
								error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected \", received " + token_p -> pretty_char_text ();

								return 1;
							}
							else
							{
								error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unknown error";

								return 1;
							}
							*/

							instruction_internal.inputs [0] = output;

							break;
						case QUEX_TKN_STRING_RELATIVE_BEGINNING:
							output = "";

							qlex.receive (& token_p);

							while (token_p -> type_id () != QUEX_TKN_TERMINATION && token_p -> type_id () != QUEX_TKN_FAILURE && token_p -> type_id () != QUEX_TKN_STRING_ABSOLUTE_END)
							{
								//switch (token_p -> type_id ())
								//{
									//QUEX_TKN_ENDL:
									//	output = "endl";
									//	break;
									//QUEX_TKN_TAB:
									//	output = '\t';
									//	break;
								//	default:
										output += token_p -> pretty_char_text ();
								//}
								
								qlex.receive (& token_p);
							}
							/*
							if (token_p -> type_id () == QUEX_TKN_STRING_END)
							{
								qlex.receive (& token_p);

								if (! token_p -> type_id () != QUEX_TKN_STATEMENT_END)
								{
									error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected ;, received " + token_p -> pretty_char_text ();

									return 1;
								}
							}
							else if (token_p -> type_id () == QUEX_TKN_TERMINATION)
							{
								error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unexpected end of file";

								return 1;
							}
							else if (token_p -> type_id () == QUEX_TKN_FAILURE)
							{
								error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Expected \", received " + token_p -> pretty_char_text ();

								return 1;
							}
							else
							{
								error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unknown error";

								return 1;
							}
							*/

							instruction_internal.inputs [0] = output;

							break;
						case QUEX_TKN_NUMBER:
							instruction_internal.inputs [0] = token_p -> pretty_char_text ();
							
							break;
						case QUEX_TKN_CONCATENATION:
							valid_instruction = false;
							
							break;
						case QUEX_TKN_STATEMENT_END:
							valid_instruction = false;
							
							break;

						case QUEX_TKN_SERIALIZE:
							valid_instruction = false;
							break;
						case QUEX_TKN_INCLUDE:
							valid_instruction = false;
							break;
						case QUEX_TKN_FAILURE:
							valid_instruction = false;
							break;
						//case QUEX_TKN_STATEMENT_END:
						//	valid_instruction = false;
						//	break;
						case QUEX_TKN_IDENTIFIER:
							valid_instruction = false;
							break;
							valid_instruction = false;
							break;
						//case QUEX_TKN_IDENTIFIER_VARIABLE:
						//	valid_instruction = false;
						//	break;
						//case QUEX_TKN_CONCATENATION:
						//	valid_instruction = false;
						//	break;
						case QUEX_TKN_OUTPUT:
							valid_instruction = false;
							break;
						case QUEX_TKN_EQUALITY_RELATIVE:
							valid_instruction = false;
							break;
						case QUEX_TKN_EQUALITY_ABSOLUTE:
							valid_instruction = false;
							break;
						case QUEX_TKN_INEQUALITY_RELATIVE:
							valid_instruction = false;
							break;
						case QUEX_TKN_INEQUALITY_ABSOLUTE:
							valid_instruction = false;
							break;
						case QUEX_TKN_ASSIGNMENT:
							valid_instruction = false;
							break;
						case QUEX_TKN_NEGATION:
							valid_instruction = false;
							break;
						case QUEX_TKN_ADDITION:
							valid_instruction = false;
							break;
						case QUEX_TKN_SUBSTRACTION:
							valid_instruction = false;
							break;
						case QUEX_TKN_MULTIPLICATION:
							valid_instruction = false;
							break;
						case QUEX_TKN_DIVISION:
							valid_instruction = false;
							break;
						case QUEX_TKN_EXPONENTIATION:
							valid_instruction = false;
							break;
						case QUEX_TKN_MODULO:
							valid_instruction = false;
							break;
						case QUEX_TKN_STRING:
							valid_instruction = false;
							break;
						case QUEX_TKN_CHARACTER:
							valid_instruction = false;
							break;
						//case QUEX_TKN_ENDL:
						//	valid_instruction = false;
						//	break;
						//case QUEX_TKN_TAB:
						//	valid_instruction = false;
						//	break;
						//case QUEX_TKN_STRING_ABSOLUTE_BEGINNING:
						//	valid_instruction = false;
						//	break;
						case QUEX_TKN_STRING_ABSOLUTE_END:
							valid_instruction = false;
							break;
						//case QUEX_TKN_STRING_RELATIVE_BEGINNING:
						//	valid_instruction = false;
						//	break;
						case QUEX_TKN_STRING_RELATIVE_END:
							valid_instruction = false;
							break;
						case QUEX_TKN_PARENTHESIS_BEGINNING:
							valid_instruction = false;
							break;
						case QUEX_TKN_PARENTHESIS_END:
							valid_instruction = false;
							break;
						case QUEX_TKN_BRACE_BEGINNING:
							valid_instruction = false;
							break;
						case QUEX_TKN_BRACE_END:
							valid_instruction = false;
							break;
						case QUEX_TKN_BACKSLASHED_QUOTE_DOUBLE:
							valid_instruction = false;
							break;
						case QUEX_TKN_BACKSLASHED_BACKSLASH:
							valid_instruction = false;
							break;
						case QUEX_TKN_LOGIC_AND:
							valid_instruction = false;
							break;
						case QUEX_TKN_LOGIC_OR:
							valid_instruction = false;
							break;
						case QUEX_TKN_GREATER:
							valid_instruction = false;
							break;
						case QUEX_TKN_GREATER_EQUAL:
							valid_instruction = false;
							break;
						case QUEX_TKN_LOWER:
							valid_instruction = false;
							break;
						case QUEX_TKN_LOWER_EQUAL:
							valid_instruction = false;
							break;
						case QUEX_TKN_FUNCTION_DEFINITION:
							valid_instruction = false;
							break;
						default:
							valid_instruction = false;
					}

					if (valid_instruction)
					{
						/*
							If we are parallelizing,
							before we enqueue our local instruction to the instructions serialized set,
							we should take care of the current instruction serialized set,
							meaning that we should enqueue them, and we should clear them
						*/
						if (! serializing)
						{
							instructions_local.push (instructions_internal);
						
							//instructions_internal._instructions.
							while (! instructions_internal.instructions_.empty ())
								instructions_internal.instructions_.pop ();
						}

						instructions_internal.instructions_.push (instruction_internal);
					}
				}
				while (token_p -> type_id () != QUEX_TKN_STATEMENT_END && token_p -> type_id () != QUEX_TKN_TERMINATION && token_p -> type_id () != QUEX_TKN_FAILURE);

				break;

			case QUEX_TKN_IDENTIFIER:
				break;
			case QUEX_TKN_IDENTIFIER_VARIABLE:
				identifier = token_p -> pretty_char_text ();
				visits = 0;
				passes = 0;

				do
				{
					//qlex.receive (& token_p);

					switch (token_p -> type_id ())
					{
						case QUEX_TKN_IDENTIFIER_VARIABLE:
						case QUEX_TKN_NUMBER:
							if (visits > 0)
							{
								//std::cout << token_p -> pretty_char_text () << std::endl;
								instruction_internal.inputs [visits > 1 ? 1 : 0] = token_p -> pretty_char_text ();

								/*
								if (visits > 1)
								{
									instruction_internal.inputs [1] = identifier;
								}
								else
								{
									instruction_internal.inputs [0] = token_p -> pretty_char_text ();
								}
								*/
							}
							else
							{
								//instruction_internal.output = token_p -> pretty_char_text ();
								instruction_internal.output = identifier;
							}

							++ visits;

							break;
						
						case QUEX_TKN_ASSIGNMENT:
							//instruction_internal.output = token_p -> pretty_char_text ();
							instruction_internal.action = LIB::machine::devices::processor::operation::assignment;

							break;

						case QUEX_TKN_ADDITION:
							instruction_internal.action = LIB::machine::devices::processor::operation::addition;

							break;

						case QUEX_TKN_SUBSTRACTION:
							instruction_internal.action = LIB::machine::devices::processor::operation::substraction;

							break;

						case QUEX_TKN_MULTIPLICATION:
							instruction_internal.action = LIB::machine::devices::processor::operation::multiplication;

							break;

						case QUEX_TKN_DIVISION:
							instruction_internal.action = LIB::machine::devices::processor::operation::division;

							break;

						case QUEX_TKN_MODULO:
							instruction_internal.action = LIB::machine::devices::processor::operation::modulo;

							break;

						//case QUEX_TKN_STATEMENT_END:
						//	break;

						default:
							error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unexpected token: " + token_p -> pretty_char_text ();

							return 1;
					}

					++ passes;
					
					qlex.receive (& token_p);
				}
				while (token_p -> type_id () != QUEX_TKN_STATEMENT_END && token_p -> type_id () != QUEX_TKN_TERMINATION && token_p -> type_id () != QUEX_TKN_FAILURE);

				// If there was not something like "$var;".
				if (passes > 1)
				{
					if (! serializing)
					{
						instructions_local.push (instructions_internal);

						//instructions_internal._instructions.
						while (! instructions_internal.instructions_.empty ())
							instructions_internal.instructions_.pop ();
					}

					instructions_internal.instructions_.push (instruction_internal);
				}
				
				break;
			//case QUEX_TKN_COMMENT_BLOCK:
			//	qlex.receive (& token_p);
				
				//if (token_p -> type_id () != QUEX_TKN_COMMENT_BLOCK_END)
				//{
				//	correct = false;
				//	
				//	break;
				//}
				
				/*
				if (token_p -> type_id () == QUEX_TKN_BACKSLASH)
				{
					// The next token is expected to be a new line.
					// Skip it so that the next one (the next after the next) would be counted as the end of the comment.
					if (token_p -> type_id () == QUEX_TKN_NEW_LINE)
					{
						qlex.receive (& token_p);
					}
				}
				
				while (token_p -> type_id () != QUEX_TKN_NEW_LINE)
				{
					qlex.receive (& token_p);
				}
				*/
			//	break;
			default:
				//error = file_name + ": " + std::to_string ((long long int) token_p -> line_number ()) + ": " + std::to_string ((long long int) token_p -> column_number ()) + ": Unexpected character: " + token_p -> pretty_char_text ();

				//return 1;
				break;
		}

		if (token_p -> type_id () == QUEX_TKN_FAILURE)
		{
			correct = false;
			
			break;
		}

		//++number_of_tokens;
	}
	// (*) check against 'termination'
	while (token_p -> type_id () != QUEX_TKN_TERMINATION /*&& token_p -> type_id () != QUEX_TKN_FAILURE*/);

	if (correct)
	{
		instructions_local.push (instructions_internal);	// Add the last set of instruction.

		while (! instructions_local.empty ())
		{
			instructions_queue.push (instructions_local.front ());
			instructions_local.pop ();
		}
	}

	return correct ? 0 : 1;
}
