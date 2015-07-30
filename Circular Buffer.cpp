/*
************************ Object Oriented Buffer **************************
Purpose: to create the code from an existing procedural buffer and convert 
		 it into object oriented code with the additional features of saving 
		 and loading messages from a local file.

Limitations: 

Special Requirements: Set the #defined BUFFER_LENGTH to the size of the 
                      buffer you would like to compile.
*/



#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

// the length of the fixed array to be used for the FIFO buffer - must be at least one
// and no greater than half the maximum value allowed in an unsigned long (see
// the file limits.h)
#define BUFFER_LENGTH 10

// Define the Message class. This class will hold the message and the timestamp of the 
// message which are kept private.
// Set member functions are used to keep the data and time variables only witin the scope
// of the class.
// << and >> are overloaded to allow displaying of message and reading and writing it to disk
class Message
{
	public:
		void setTime(time_t);
		void setData(unsigned char);
		friend ostream& operator<<(ostream&, const Message&); 
		friend fstream& operator<<(fstream&, const Message&);
		friend istream& operator>>(istream&, Message&); 
	private:
		unsigned char data;  // single byte of information to be sent
		time_t time;  // number of seconds since 1 January 1970
};

// member function that sets the time variable.
// arguments: a time_t value
// return: void.
void Message::setTime(time_t timevar){
	time = timevar;
	
}

// member function to set the data variable
// arguments: an unsigned char
// return: void
void Message::setData(unsigned char inputchar){
	data = inputchar;
}


// overloading the << operator to allow displaying of messages
// arguments: an input message in the form of an instance of message class.
//			  an ostream variable.
// return: an ostream variable
ostream& operator<<(ostream& OutStream, const Message& OutMess){
	time_t Time = OutMess.time;

	OutStream << OutMess.data << "\t"  <<  ctime(&Time) << endl;
	
	return OutStream;
}


// overloading the << operator to allow saving a message to disk
// arguments: an input message in the form of an instance of message class.
//			  an fstream variable.
// return: an fstream variable
fstream& operator<<(fstream& OutStream, const Message& OutMess){

	OutStream << OutMess.data << endl;
	
	return OutStream;
}

// overloading the >> operator to allow loading a message from disk
// arguments: an input message in the form of an instance of message class.
//			  an istream variable.
// return: an istream variable
istream& operator>>(istream& InStream, Message& InMess){

	InMess.time = time(NULL);

	InStream >> InMess.data; 

	return InStream;
}



// defining the Buffer class. The will instantiate an array of message class instances 
// which will be the length of the buffer. This is done in the constructor. 
// The other functionality of the buffer is also in this class. This is included in the 
// member functions.
// All the frequently used variables are kept private. Such as buffer_tail, buffer_head etc
class Buffer
{
	public:
		Buffer();
		void Produce(Message*);
		void Consume(Message*);
		void Show(Message*);
		void Save(Message*);
		void Load(Message*);
		
	private:
		unsigned long buffer_tail;
		unsigned long buffer_length;
		unsigned long buffer_head;
		unsigned char buffer_input;
		char UserInput;
		int inputLoopCount;
		unsigned long count;
		fstream FilePointer;
};


// The constructor for the buffer class. Instantiates the message array.
// Holds the basic interface for the program.
// Arguments: none
// Return: none
Buffer::Buffer(){
	Message buffer[BUFFER_LENGTH];   // the message buffer

	buffer_tail = 0;  // position of the tail in the message buffer -
                                  // the next message will be consumed from here
	buffer_length = 0;  // number of messages in the buffer
	inputLoopCount = 1;
                      
	// loop until the user wishes to exit
	while (inputLoopCount == 1) {
    
		// show the menu of options
		cout << endl;
		cout << "Buffer Management Menu" << endl;
		cout << "----------------------" << endl;
		cout << "1. Produce a new message for the buffer" << endl;
		cout << "2. Consume a message from the buffer" << endl;
		cout << "3. View the contents of the buffer" << endl;
		cout << "4. Save the messages to disk" << endl;
		cout << "5. Read messages from disk" << endl;
		cout << "6. Exit from the program" << endl << endl;
    
		// get the user's choice
		cout << "Enter your option: ";
		cin >> UserInput;
		cout << endl;
		
		// act on the user's input
		switch(UserInput) {
			case '1':
				Produce(buffer);
				break;
      
			case '2':
				Consume(buffer);
				break;
      
			case '3':
				Show(buffer);
				break;

			case '4':
				Save(buffer);
				break;

			case '5':
				Load(buffer);
				break;

			case '6':
				inputLoopCount = 0;
				break;

			default:
				cout << "Invalid entry" << endl << endl;
				break;
		}
	}
}


// Create a new message and add it to the head of the buffer.
// The data for the message is obtained from the user and a time stamp is obtained from the
// time() function in the C standard library. 
// Arguments:
//   (1) an instance of the Message class
// Returns: void
void Buffer::Produce(Message* buffer){

	// find the element of the buffer in which to store the message
	buffer_head = (buffer_tail + buffer_length) % BUFFER_LENGTH;

	// get the value of the data for the message from the user
	cout << "Add new data" << endl;
	cout << "------------" << endl;
	cout << "Enter data: ";
	cin >> buffer_input;
	cout << endl;

	// set the data value of the message to the user input
	buffer[buffer_head].setData(buffer_input);
	// get the value of the time for the message
	buffer[buffer_head].setTime(time(NULL));

	// if no buffer overflow has occurred, adjust the buffer length
	if (buffer_length < BUFFER_LENGTH) {
		buffer_length++;
	}

	// if a buffer overflow has occurred, display an error statement
	else {
		buffer_tail = (buffer_tail+1) % BUFFER_LENGTH;
		cout << "Overflow error" << endl;
	}  
}


// Pop the message at the head of the buffer and display it.
// Arguments:
//   (1) an instance of the Message class
// Returns: void
void Buffer::Consume(Message* buffer){
	
	// if the buffer is empty, display an error statement
	if (buffer_length == 0) {
		cout << "No messages in the buffer" << endl;
	}

	// if the buffer is not empty, display the message at the tail, remove the message by
	// advancing the tail of buffer offset in a circular manner and adjust the buffer length
	else {
		cout << endl;
		cout << "Offset Data        Time" << endl;
		cout << buffer_tail << "\t";
		cout << buffer[buffer_tail];

		buffer_tail = (buffer_tail+1) % BUFFER_LENGTH;
		buffer_length--;
	}
}


// Display all of the messages in the buffer.
// Arguments:
//   (1) an instance of the Message class
// Returns: void
void Buffer::Show(Message* buffer){
  
	// if the buffer is empty, display an error statement
	if (buffer_length == 0) {
		cout << "No messages in the buffer" << endl;
	}

	// if the buffer is not empty, display all the messages in the buffer
	else {
		// display a title
		cout << endl;
		cout << "Offset Data        Time" << endl;

		// display messages if they are sequential in the array
		buffer_head = (buffer_tail + buffer_length) % BUFFER_LENGTH;

		if (buffer_tail < buffer_head) {
			for (count=buffer_tail;count<buffer_head;count++) {
				cout << count << "\t";
				cout << buffer[count];
			}
		}

		// display messages if part are at the end of the array and the remainder at the start
		else {
			for (count=buffer_tail;count<BUFFER_LENGTH;count++) {
				cout << count << "\t";
				cout << buffer[buffer_tail];
			}
			for (count=0;count<buffer_head;count++) {
				cout << count << "\t";
				cout << buffer[count];
			}
		}				
	}
}


// Save all of the messages in the buffer to a file called datefile.txt.
// Arguments:
//   (1) an instance of the Message class
// Returns: void
void Buffer::Save(Message* buffer){
	FilePointer.open("datafile.txt", ios::out); 

	if (!FilePointer.good()) {
		cout << "FATAL ERROR"; 
		exit(1);
	}

	buffer_head = (buffer_tail + buffer_length) % BUFFER_LENGTH;

	if (buffer_tail < buffer_head) {
		for (count=buffer_tail;count<buffer_head;count++) {
			FilePointer << buffer[count];
		}
	}else {
		for (count=buffer_tail;count<BUFFER_LENGTH;count++) {
			FilePointer << buffer[count];
		}
		for (count=0;count<buffer_head;count++) {
			FilePointer << buffer[count];
		}
	}

	FilePointer.close();

	cout << endl << "Messages saved to disk!" << endl;
}


// Populate the buffer with messages from a local file.
// Arguments:
//   (1) an instance of the Message class
// Returns: void
void Buffer::Load(Message* buffer){
	FilePointer.open("datafile.txt", ios::in); 

	if (!FilePointer.good()) {
		cout << "FATAL ERROR"; 
		exit(1);
	}

	count=0;
	
	while(!FilePointer.eof()){

		FilePointer >> buffer[count];
		count++;
	}

	buffer_tail = 0;
	buffer_length = count - 1;
	

	FilePointer.close();

	cout << endl << "Messages loaded from disk!" << endl;

}




// Instantiate and instance of the Buffer class.
// Arguments: None
// Returns: 0 on completion
int main()
{
	Buffer newBuffer;
	return 0;
}