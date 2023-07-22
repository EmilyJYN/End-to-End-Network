#include "reassembler.hh"
#include <sstream>

using namespace std;
//discard substrings larger than available capacity, store unordered bytes, push ordered next byte to writer
void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  if(is_last_substring){last_rcvd_ = is_last_substring;}

  for (uint64_t i = 0; i < data.size(); i++) {
    //如果该byte 已经被ack(assembled) || 已经进入unassembled buffer || 超过了availablity
    if (first_index+i < ack_ || unassembled_.contains(first_index+i) || first_index+i > output.available_capacity()+ack_-1){
      continue; //进入下一循环
    }
    unassembled_[first_index + i] = data[i]; //否则直接存入unassembled buffer
    bytes_pending_++;
  }

  ostringstream assembled;
  while (unassembled_.contains(ack_)) {
    assembled << unassembled_[ack_];
    bytes_pending_--;
    ack_++;
  }
  string const str = assembled.str();
  if (!str.empty()) {
    output.push(str);
  }
  //last_rcvd_ = 1代表已经接收过last_substring了，在此条件下，bytes_pending() == 0 意为所有的bytes都已经push了
  //也就是说，如果先接收到了last_substring，那么在处理最后一个到达的substring的时候，它将知道前面接收过last one，于是关闭输出
  if (last_rcvd_ && bytes_pending() == 0) {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  return bytes_pending_;
}

/*#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  // Your code here.
  uint64_t available_capacity = output.available_capacity();
  uint64_t data_size = data.size();

  //discard substrings larger than available capacity
  if (data_size > available_capacity) return;

  //store unordered bytes
  uint64_t ack = output.bytes_pushed();
  if(first_index < ack){
    return;
  }
  else if(first_index > ack){
    unassembled_.insert({first_index,data});
  }
  //push ordered next byte to writer
  else{
    output.push(data);
    auto it = unassembled_.begin();
    while((it->first) == ack+data_size){
      output.push(it->second);
      unassembled_.erase(it);
    }
  }
  if(is_last_substring){
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return {unassembled_.size()};
}
*/
