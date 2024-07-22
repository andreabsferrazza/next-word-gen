# 1. Installation
make

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

# 3. Domains

[path/to/inputfile] string - path of the inputfile

[path/to/dictionary] string - path to the dictionary

[n] integer - is the number of words desired in the text

[word] string - s a word specified as first
