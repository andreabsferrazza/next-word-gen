#include "../lib/argtable3.h"
#include "create_dictionary.c"
#include "generate_random_text.c"

/* global arg_xxx structs */
struct arg_lit *create_dictionary_opt, *generate_random_text_opt, *help, *version;
struct arg_file *o, *file;
struct arg_int *words_number;
struct arg_str *first_word;
struct arg_end *end;

int main(int argc, char *argv[]){
	/* the global arg_xxx structs are initialised within the argtable */
	void *argtable[] = {
		help    			= arg_litn("h", "help", 0, 1, "display this help and exit\nOptions create_dictionary and generate_random_text must not be contemporary.\n"),
		create_dictionary_opt		= arg_litn("c", "create_dictionary", 0, 1,"Create dictionary (Requires option --file)"),
		generate_random_text_opt	= arg_litn("g", "generate_random_text", 0, 1,"Generate random text"),
		words_number			= arg_intn("n", "words_number", "<n>",0,1,"Number of words to randomize"),
		first_word			= arg_str0("w", "first_word", "<string>", "First word of the text"),
		file    			= arg_filen("f", "file", "<file>", 0, 1, "Input file (1)"),
		end     			= arg_end(20),
	};

	int exitcode = 0;
	char progname[] = "next-word-gen";

	int nerrors;
	nerrors = arg_parse(argc,argv,argtable);

	/* special case: '--help' takes precedence over error reporting */
	if (help->count > 0){
		printf("Usage: %s", progname);
		arg_print_syntax(stdout, argtable, "\n");
		printf("Options for next-word-gen:\n\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		exitcode = 0;
		goto exit;
	}

	/* If the parser returned any errors then display them and exit */
	if (nerrors > 0){
		/* Display the error details contained in the arg_end struct.*/
		arg_print_errors(stdout, end, progname);
		printf("Try '%s --help' for more information.\n", progname);
		exitcode = 1;
		goto exit;
	}

	if (generate_random_text_opt->count > 0 && file->count==1 && create_dictionary_opt->count==0){
		const char* fn = file->filename[0];
		int nw = words_number->ival[0];
		if(first_word->count==1){
			const char* fw = first_word->sval[0];
			exitcode = generate_random_text(fn,nw,fw);	
		}else{
			const char* fw = " ";
			exitcode = generate_random_text(fn,nw,fw);	
		}
	}else if (create_dictionary_opt->count > 0 && file->count==1 && generate_random_text_opt->count==0){
		const char* fn = file->filename[0];
		exitcode = create_dictionary(fn);	
	}else{
		arg_print_errors(stdout, end, progname);
		printf("Try '%s --help' for more information.\n", progname);
		exitcode = 2;
		goto exit;
	}

exit:
	/* deallocate each non-null entry in argtable[] */
	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return exitcode;
}

