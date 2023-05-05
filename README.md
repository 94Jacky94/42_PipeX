# 42_PipeX
The source code of a C program, which gives the output of one program on entry to the next.
Just like the linux shell pipe "|".

Usage example (if the first parameter isn't here_doc, the content of outfile will be replaced by the output of the last program):

	./pipex infile "grep toto" "grep titi" "wc" outfile

Set the first parameter to here_doc, in order to replace the input by the stdin, then replace the string LIMITER by one of your choice (as following):

	./pipex here_doc LIMITER "grep toto" "grep titi" "wc" outfile
	
The keyword here_doc is supposed to make the programm mimic this commande line:

	cmd << LIMITER | cmd1 >> file
