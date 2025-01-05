# Next-word-gen

This is my solution for the project for the exam "Operating Systems" taken during the degree course in Computer Science at Sapienza University.
The project specifications are to be found in section 6 of this readme.

## 1. Installation

Download Argtable3 from https://www.argtable.org/ and put the argtable.h and the argtable3.c in the lib folder.

Run the command:
make

## 2. Usage
./nwg --help
or
./nwg -h
to display the help

## 2.1. Create a dictionary
To create a file dictionary

./nwg --create-dictionary --file=[path/to/inputfile]

or

./nwg -cf [path/to/inputfile]

dictionary.csv will be created in the directory of the program

To generate a text from a dictionary

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary]

or

./nwg -gf path/to/dictionary -w [n]

It is possible to specify a first word with the option first_word

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary] --first_word=[word]

or

./nwg -gf path/to/dictionary -n [n] -w [word]

## THE OUTPUT WILL BE WRITTEN IN output.txt

## 2.3. Multiprocess mode
It is possible to execute the program in multiprocess mode with the option multi_process. If the option is selected the program will execute using 3 processes.

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary] --multi_process

./nwg -gmf path/to/dictionary -n [n]

./nwg --generate_random_text --words_number=[n] --file=[path/to/dictionary] --first_word=[word] --multi_process

./nwg -gmf path/to/dictionary -n [n] -w [word]

./nwg --create-dictionary --file=[path/to/inputfile] --multi_process

./nwg -cmf [path/to/inputfile]

## 3. Domains

[path/to/inputfile] string - path of the inputfile

[path/to/dictionary] string - path to the dictionary

[n] integer - is the number of words desired in the text

[word] string - s a word specified as first

## 4. Examples

./nwg -cf test/testo && ./nwg -gf dictionary.csv -n100 && cat output.txt
./nwg -cmf test/testo2 && ./nwg -gf dictionary.csv -n100 && cat output.txt

## 5. Bugs known

The file src/generate_random_text.c gives segfault for significantly big inputs, that's due to the static allocation of the memory. A dynamic implementation (as the one of src/create_dictionary.c) is the solution. 

## 6. Project Spefications
### English
The goal is to implement an ANSI C program that:
Task 1:
1. given a text in Italian, produces a table containing for each word of the text the immediately following words and their frequency of occurrence in the text;
Task 2:
2. generates a text randomly using the frequency of the words calculated in point 1.

The program must offer two execution modes: single process and multi-process with at least three concurrent processes. Both modes must give the user the possibility to independently execute tasks 1 and 2
Input data and general requirements:
Task 1: a text file in Unicode (UTF-8) encoding containing a text in Italian
structured in sentences terminated by the characters ., ?, or ! (other punctuation characters
may be present);
Task 2:
1) a text file in CSV format (comma-separated values: each line of the file
represents a row of a table, in each row the elements are separated
by a comma). The file contains a table in which each line reports a word and the
words immediately following with their frequencies;
2) the number of words to generate;
3) (optional) a previous word from which to start the generation.

Output data and general requirements:
Task 1: the word frequency table, as a text file in CSV format
(see point a. above);
task 2: a text file in Unicode (UTF-8) encoding containing the
randomly generated words.
punctuation marks ., ?, ! must be treated as separate words; apostrophes
are part of the word; the remaining punctuation can be discarded. See the
attached file for some examples of input text and the corresponding table;
case insensitive has no effect: e.g., ‘today’ = ‘Today’ = ’TODAY’ = etc.
the last word (or punctuation) of the input text is considered to be followed by the first
word;
when generating the random text you can specify a word preceding the first,
otherwise the program selects a random punctuation among ., ?, ! (according to the
input table) and generates the next word;
when generating the random text the word at the beginning of a sentence (i.e. the word after a
period ., ?, or !) must have the initial capital letter;
assume that a word is at most 30 printable characters long;
you must generate only one executable that implements the required features and the
two execution modes.

General project requirements:
each .c/.h file must be well commented: for each function briefly
comment the input/output parameters and its general operation; in the
body of each function comment the most important lines of code;
provide a makefile to compile the project with a simple make from the
command line;
provide a README text file with a brief explanation of the included files and a
short user manual;
the multi-process mode of the program must consist of at least three
concurrent and intercommunicating processes: one to read the input file, one
to create the table, and finally one to write the output file. (Further
divisions of the workload into more than three processes are allowed.)
You are NOT allowed to use external libraries with functions for manipulating
strings or text other than the standard C ones. Exceptions:
to generate the words according to the table you can use the GNU Scientific Library
or directly implement (with the rand() function and the constant
RAND_MAX) the method described here:
https://www.gnu.org/software/gsl/doc/html/randist.html#general-discrete-distri
butions
You are allowed to use external libraries for parsing the command-line options
for your program (e.g. https://gflags.github.io/gflags/ or
https://www.argtable.org/ ).

### Italian
L’obiettivo è implementare un programma ANSI C che:
Compito 1:
1. dato un testo in Italiano, produca una tabella contenente per ogni parola del testo le parole immediatamente successive e la loro frequenza di occorrenza nel testo;
Compito 2:
2. generi un testo in maniera casuale usando la frequenza delle parole calcolata nel punto 1.

Il programma dovra’ offrire due modalita’ di esecuzione: singolo processo e multi-processo con almeno tre processi concorrenti. Entrambe le modalita’ dovranno dare all’utente la possibilità di eseguire indipendentemente i compiti 1 e 2
Dati in ingresso e requisiti generali:
Compito 1: un file di testo in codifica Unicode (UTF-8) contenente un testo in Italiano
strutturato in frasi terminate dai caratteri ., ?, o ! (altri caratteri di punteggiatura
possono essere presenti);
Compito 2:
1) un file di testo in formato CSV (comma-separated values: ogni riga del file
rappresenta una riga di una tabella, in ogni riga gli elementi sono separati da
una virgola). Il file contiene una tabella in cui ogni riga riporta una parola e le
parole immediatamente successive con le loro frequenze;
2) il numero di parole da generare;
3) (opzionale) una parola precedente da cui iniziare la generazione.

Dati in uscita e requisiti generali:
Compito 1: la tabella della frequenza delle parole, come file di testo in formato CSV
(vedere il punto a. di cui sopra);
compito 2: un file di testo in codifica Unicode (UTF-8) contenente le parole generate
casualmente.
le punteggiature ., ?, ! devono essere trattate come parole separate; gli apostrofi
fanno parte della parola; la rimanente punteggiatura può essere scartata. Vedere il
file allegato per alcuni esempi di testo di input e la corrispondente tabella;
maiuscole/minuscole non hanno effetto: ad es., ‘oggi’= ‘Oggi’ = ’OGGI’ = etc.
l’ultima parola (o punteggiatura) del testo di input si intende seguita dalla prima
parola;
nel generare il testo casuale si può specificare una parola precedente la prima,
altrimenti il programma seleziona una punteggiatura a caso tra ., ?, ! (secondo la
tabella di input) e genera la parola successiva;
nel generare il testo casuale la parola all’inizio di una frase (cioè la parola dopo un
punto ., ?, o !) deve avere l'iniziale maiuscola;
si assuma che una parola sia lunga al massimo 30 caratteri stampabili;
si dovra’ generare un solo eseguibile che implementi le funzionalita’ richieste e le
due modalita’ di esecuzione.

Requisiti generali del progetto:
ogni file .c/.h dovrà essere ben commentato: per ogni funzione commentare
brevemente i parametri di ingresso/uscita e il suo funzionamento generale; nel
corpo di ogni funzione commentare le linee di codice più importanti;
fornire un makefile per compilare il progetto con un semplice make da linea di
comando;
fornire un file di testo README con una breve spiegazione dei file inclusi e un breve
manuale utente;
la modalita’ multi-processo del programma dovrà essere costituita da almeno tre
processi concorrenti ed intercomunicanti: uno per leggere il file di ingresso, uno
per creare la tabella, ed infine uno per la scrittura del file di output. (Ulteriori
suddivisioni del carico di lavoro in più di tre processi sono ammesse.)
NON è ammesso l’uso di librerie esterne con funzioni per la manipolazione di
stringhe o testo che non siano quelle standard del C. Eccezioni:
oper generare le parole secondo la tabella potete usare la GNU Scientific Library
oppure implementare direttamente (con la funzione rand() e la costante
RAND_MAX) il metodo descritto qui:
https://www.gnu.org/software/gsl/doc/html/randist.html#general-discrete-distri
butions
è ammesso l’uso di librerie esterne per l’analisi delle opzioni della linea di
comando per il vostro programma (ad es. https://gflags.github.io/gflags/ o
https://www.argtable.org/ ).
