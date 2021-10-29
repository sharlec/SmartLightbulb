Members: Chris, Charles, Jacob, Kirk, Pedro 

Description: this package includes all of Team 8's current cpp and respective header file for stage 2 of 3307 assignment.

The following files are included: 

FileController.cpp
FileController.h
Main.cpp
OurApplication.cpp
OurApplication.h
User.cpp
User.h
styles.css
data.txt
testingClass.C
TestFileController.cpp
Makefile
start_server
UML_Class_Diagram.pdf
README.txt

How to compile:
type the following command in terminal while inside the project folder

	make

it will automatically compile all necessary files and create a start_server file

How to launch server:
Type the following into terminal:

	./start_server

If the above command does not work, enter this instead:

	./wt_server --docroot . --http-address 0.0.0.0 --http-port 8080

How to access the server:
Navigate to localhost:8080 on Mozilla Firefox or Google Chrome

When running please note a password must be numbers or letters and 6-12 characters long. 
