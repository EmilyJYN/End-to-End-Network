#pragma once

#include <cstdint>

/*
 * The Wrap32 type represents a 32-bit unsigned integer that:
 *    - starts at an arbitrary "zero point" (initial value), and
 *    - wraps back to zero when it reaches 2^32 - 1.
 */

class Wrap32
{
protected:
  uint32_t raw_value_ {};

public:
  explicit Wrap32( uint32_t raw_value ) : raw_value_( raw_value ) {}

  /* Construct a Wrap32 given an absolute sequence number n and the zero point. */
  //调用示例： Wrap32 wrapped = Wrap32::wrap(n, zero_point);
  //abs seq no -> seq no, n = abs seq no, zero_point = ISN
  static Wrap32 wrap( uint64_t n, Wrap32 zero_point );

  /*
   * The unwrap method returns an absolute sequence number that wraps to this Wrap32, given the zero point
   * and a "checkpoint": another absolute sequence number near the desired answer.
   *
   * There are many possible absolute sequence numbers that all wrap to the same Wrap32.
   * The unwrap method should return the one that is closest to the checkpoint.
   */
  //这里checkpoint取ack是因为我们假设TCP的receiver正在处理新到的packet，而一般来说新到的packet应该和ack的index相差不大
  //调用示例： uint64_t result = obj.unwrap(zero, checkpoint);
  //seq no -> abs seq no, zero_point = ISN, checkpoint = ack, this = seq no
  uint64_t unwrap( Wrap32 zero_point, uint64_t checkpoint ) const;

  Wrap32 operator+( uint32_t n ) const { return Wrap32 { raw_value_ + n }; }
  bool operator==( const Wrap32& other ) const { return raw_value_ == other.raw_value_; }
};
