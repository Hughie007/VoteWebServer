#pragma once
#include<functional>
#include<unordered_set>
#include<boost/circular_buffer.hpp>
class Buffer;
class TcpConn;
class TimeStamp;
class Entry;
typedef std::shared_ptr<TcpConn> TcpConnPtr;
typedef std::function<void(const TcpConnPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnPtr&,Buffer*,TimeStamp)> MessageCallback;
typedef std::function<void(const TcpConnPtr&)> CloseCallback;

typedef std::shared_ptr<Entry> EntryPtr;
typedef std::weak_ptr<Entry> weakEntryPtr;
typedef std::unordered_set<EntryPtr> Bucket;
typedef boost::circular_buffer<Bucket> TimeWheel;
