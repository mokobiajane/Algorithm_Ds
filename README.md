To build Task 1.1(Decoder):

BUILD: "make"


RUN PROGRAM: ./ascii85 -e < input >
             ./ascii85 -d < "-e result" >


RUN TEST: ./test_ascii85


CLEAN BUILD FILES: make clean
