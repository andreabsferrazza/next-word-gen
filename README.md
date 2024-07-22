# 1. Installation
make

or in the bin folder there is an executable

# 2. Usage
./nwg --help
or
./nwg -h
to display the help

# 2.1. Create a dictionary
To create a file dictionary

./nwg --create-dictionary --file=[path/to/inputfile]

or

./nwg -cf [path/to/inputfile]

dictionary.csv will be created in the directory of the program

# 2.2. Generate a text from a dictionary
To generate a text from a dictionary

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary]

or

./nwg -gf path/to/dictionary -w [n]

It is possible to specify a first word with the option first_word

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary] --first_word=[word]

or

./nwg -gf path/to/dictionary -n [n] -w [word]

# THE OUTPUT WILL BE WRITTEN IN output.txt

# 2.3. Multiprocess mode
It is possible to execute the program in multiprocess mode with the option multi_process. If the option is selected the program will execute using 3 processes.

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary] --multi_process

./nwg -gmf path/to/dictionary -n [n]

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary] --first_word=[word] --multi_process

./nwg -gmf path/to/dictionary -n [n] -w [word]

./nwg --create-dictionary --file=[path/to/inputfile] --multi_process

./nwg -cmf [path/to/inputfile]

# 3. Domains

[path/to/inputfile] string - path of the inputfile

[path/to/dictionary] string - path to the dictionary

[n] integer - is the number of words desired in the text

[word] string - s a word specified as first

# 4. Examples

./nwg -cf test/testo && ./nwg -gf dictionary.csv -n100 && cat output.txt
./nwg -cmf test/testo2 && ./nwg -gf dictionary.csv -n100 && cat output.txt
