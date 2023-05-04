# 42_PipeX
The source code of a C program, which gives the output of one program on entry to the next.
Just like the linux shell pipe "|".

Usage example (if the first parameter isn't here_doc, the content of outfile will be replaced by the output of the last program):
	./pipex infile "grep toto" "grep titi" "wc" outfile

Usage example (if the first parameter is here_doc, the output of the last program will be add to the content of outfile):
	./pipex here_doc infile "grep toto" "grep titi" "wc" outfile
