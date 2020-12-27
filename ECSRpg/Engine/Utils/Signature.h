#pragma once

#include <set>

struct Signature 
{
	std::set<unsigned int> hashes;

	bool operator ==(Signature b)
	{
		if (b.hashes.size() != hashes.size())
			return false;

		std::set<unsigned int>::reverse_iterator revIt = hashes.rbegin();

		while (revIt != hashes.rend())
		{
			if (!b.hashes.count(*revIt))
				return false;
			++revIt;
		}

		return true;

	}
};


template<typename... Args>
Signature CreateSignature()
{
	Signature sig = Signature();
	sig.hashes = {(typeid(Args).hash_code())...};
	return sig;

}
