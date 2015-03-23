#pragma once

#include <algorithm>
#include <deque>
#include <iostream>

namespace kademlia {

template <typename ValueType, template <typename...> class ListType, typename Hash=std::hash<ValueType>>
class routing_table
{
protected:
	using HashType = typename Hash::result_type;
	using list = ListType<ValueType>;
	using iterator = typename list::iterator;

public:
	void set_origin(const ValueType& value)
	{
		_origin = hash(value);
	}

	bool insert(const ValueType& value)
	{
		unsigned i = index(value);
		list& bucket(_table[i]);
		if (std::find(bucket.begin(), bucket.end(), value) != bucket.end())
			return false;

		bucket.push_back(value);
		return true;
	}

	template <typename LookupType>
	bool erase(const LookupType& key)
	{
		unsigned i = index(key);
		list& bucket(_table[i]);

		auto it = std::find(bucket.begin(), bucket.end(), key);
		if (it == bucket.end())
			return false;

		bucket.erase(it);
		return true;
	}

	template <typename LookupType>
	iterator find(const LookupType& key)
	{
		unsigned i = index(key);
		list& bucket(_table[i]);

		auto it = std::find(bucket.begin(), bucket.end(), key);
		if (it == bucket.end())
			return end();
		return it;
	}

	iterator end()
	{
		return _table[0].end();
	}

protected:
	template <typename LookupType>
	unsigned index(const LookupType& key) const
	{
		HashType mask = hash(key) ^ _origin;
		unsigned index = 0;
		while (mask >>= 1)
			++index;
		return index;
	}

protected:
	Hash hash;
	std::array<list, sizeof(HashType)*8> _table;
	HashType _origin;
};

}//namespace
