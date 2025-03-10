#include "socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>

using namespace std;

void get_URL( const string& host, const string& path )
{
  // Your code here.

  // You will need to connect to the "http" service on
  // the computer whose name is in the "host" string,
  // then request the URL path given in the "path" string.

  // Then you'll need to print out everything the server sends back,
  // (not just one call to read() -- everything) until you reach
  // the "eof" (end of file).
  
  TCPSocket socket; // HTTP is based on TCP
  string str;
  socket.connect(Address(host, "http")); // member func of Socket in socket.hh
  string data_send = "GET "+ path + " HTTP/1.1\r\n"+"Host: " + host+"\r\n"+"Connection: close\r\n\r\n";
  socket.write(data_send); //member func of FileDescriptor in file_descriptor.hh
  while(!socket.eof()){ //member func of FileDescriptor in file_descriptor.hh
    socket.read(str); //member func of FileDescriptor in file_descriptor.hh
    cout << str;
  }
  socket.close(); //member func of FileDescriptor in file_descriptor.hh

  cerr << "Function called: get_URL(" << host << ", " << path << ")\n";
  cerr << "Warning: get_URL() has not been implemented yet.\n";
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
