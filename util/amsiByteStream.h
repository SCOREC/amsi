#ifndef AMSIBYTESTREAM_H_
#define AMSIBYTESTREAM_H_
//hacky and not portable
typedef char byte;
template <typename Head>
size_t talloc_helper(Head & head)
{
  return sizeof(Head);
}
template <typename Head, typename ... Tail>
size_t talloc_helper(Head & head, Tail & ... tail)
{
  return sizeof(Head) + talloc_helper(tail...);
}
template <typename T>
void * byte_read(void * src, T & dest)
{
  memcpy(&dest,src,sizeof(T));
  return static_cast<void*>(static_cast<byte*>(src) + sizeof(T));
}
template <typename T>
void * byte_write(void * dest, T & src)
{
  memcpy(dest,&src,sizeof(T));
  return static_cast<void*>(static_cast<byte*>(dest) + sizeof(T));
}
template <typename Head, typename ... Tail>
void * talloc(Head & head, Tail & ... tail)
{
  return malloc(talloc_helper(head,tail...));
}
template <typename Head>
void byte_pack(void * memory, Head & value)
{
  byte_write(memory,value);
}
template <typename Head, typename ... Tail>
void byte_pack(void * memory, Head & value, Tail & ... tail)
{
  byte_pack(byte_write(memory,value),tail...);
}
template <typename Head>
void byte_unpack(void * memory, Head & head)
{
  byte_read(memory,head);
}
template <typename Head, typename ... Tail>
void byte_unpack(void * memory, Head & value, Tail & ... tail)
{
  byte_unpack(byte_read(memory,value),tail...);
}
template <typename Head>
void * serialize(Head & head)
{
  void * result = talloc(head);
  byte_pack(result,head);
  return result;
}
template <typename Head, typename ... Tail>
void * serialize(Head & head, Tail & ... tail)
{
  void * result = talloc(head,tail...);
  byte_pack(result,head,tail...);
  return result;
}
#endif
