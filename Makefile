                                                                          
compile:                                                                                   
	g++ -I src/include -c main.cpp                                              
link:                                                                                      
	g++ main.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system
linklinux:
	g++ main.o -o main -L /usr/include/SFML -l sfml-graphics -l sfml-window -l sfml-system 
buildW: compile link
buildL: compile linklinux
go:
	./main