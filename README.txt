NAME:
     look -- display lines beginning with a given string


FLAGS:

 -d      Dictionary character set and order, i.e., only alphanumeric characters are compared.


 -f      Ignore the case of alphabetic characters.


 -t      Specify a string termination character, i.e., only the characters in string up to and including the first occurrence of termchar are compared.


COMPILE:
     make

RUN:
     ./look.out [-df] [-t termchar] string [file ...]