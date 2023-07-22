#include "wrapping_integers.hh"

using namespace std;
//abs seq no -> seq no, n = abs seq no, zero_point = ISN
Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  auto seq_no = n + static_cast<uint64_t>(zero_point.raw_value_);
  return Wrap32 { static_cast<uint32_t>(seq_no) }; // uint64_t -> uint32_t 自动截取最低32位
}

//seq no -> abs seq no, zero_point = ISN, checkpoint = ack, this = seq no
uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.

  // 等价 this.raw_value_ - zero_point.raw_value_ = seq - ISN，并隐式转换为64bit的int，可以显式转换但没必要
  //这里的seq no是wrapped的，不过uint的减法就是wrap around的，所以这里的结果一定是两个数之间的距离
  uint64_t abs_seq1 = raw_value_ - zero_point.raw_value_; //这个 abs_seq1 < 2^32

  //如果checkpoint <= abs_seq1，则肯定是目前这个abs_seq离ack最近，因为相邻的只有这一个abs_seq，具体看notion示意图
  if ( checkpoint <= abs_seq1 ) { return abs_seq1; }

  //如果checkpoint > abs_seq1，则有两个相邻的abs_seq，取近的那一个，具体计算原理见notion
  //其中通过右移再左移，消除了低 32 bits，保留了高 32 bits
  uint64_t l = ( ( ( checkpoint - abs_seq1 ) >> 32 ) << 32 ) + abs_seq1;
  uint64_t r = ( ( ( ( checkpoint - abs_seq1 ) >> 32 ) + 1 ) << 32 ) + abs_seq1;
  //取 l 和 r 中离checkpoint近的那个
  uint64_t abs_seq2 = checkpoint - l < r - checkpoint ? l : r; 
  return { abs_seq2 };
}
