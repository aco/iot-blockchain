#include <vector>

#include "block.hpp"
#include "transaction.hpp"

struct LocalConfiguration
{
	std::string host_profile_identifier;
	bool observe_only;
	size_t block_size_reserve;
};
