#include <stdexcept>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  // Your code here.
  uint64_t data_size = data.size();
  if(data_size > available_capacity()){
    data_size = available_capacity();
  }
  buffer_ += data.substr(0,data_size);
  write_count_ += data_size;
}

void Writer::close()
{
  // Your code here.
  close_ = 1;
}

void Writer::set_error()
{
  // Your code here.
  error_ = 1;
}

bool Writer::is_closed() const
{
  // Your code here.
  return {close_ == 1};
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return {capacity_ - buffer_.size()};
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return {write_count_};
}
/***********************Reader*******************/
string_view Reader::peek() const
{
  // Your code here.
  return buffer_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return {close_ && (buffer_.size() == 0)}; // either closed or empty
}

bool Reader::has_error() const
{
  // Your code here.
  return {error_ == 1};
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (len > buffer_.size()){
    len = buffer_.size();
  }
  buffer_.erase(0,len);
  read_count_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return {buffer_.size()};
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return {read_count_};
}
