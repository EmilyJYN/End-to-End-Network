#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream )
{
  // set ISN if this is the first SYN
  if ( !syn_received && message.SYN ) {
    zero_point = message.seqno;
    syn_received = true;
  }
  // if ISN is already set, send payload into reassembler
  if ( syn_received ) {
    reassembler.insert(
      // SYN flag will take one byte even for non-SYN messages. zero_point is seqno + 1 for non-SYN messages.
      message.seqno.unwrap(( message.SYN ? zero_point : zero_point+1 ), inbound_stream.bytes_pushed() ),
      message.payload,
      message.FIN,
      inbound_stream );
  }
}

TCPReceiverMessage TCPReceiver::send( const Writer& inbound_stream ) const
{
  TCPReceiverMessage rm;
  // find window size, cast to uint_t
  rm.window_size = static_cast<uint16_t>(
    inbound_stream.available_capacity() > UINT16_MAX ? UINT16_MAX : inbound_stream.available_capacity() );
  // find ack, the next expected seqno
  if ( syn_received ) {
    // if closed, ack = byte_pushed + 2; if not closed, ack = bytes_pushed + 1
    rm.ackno = Wrap32::wrap( inbound_stream.bytes_pushed() + ( inbound_stream.is_closed() ? 2 : 1 ), zero_point );
  }
  return rm;
}