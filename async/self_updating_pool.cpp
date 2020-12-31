//
// Created by weining on 31/12/20.
//

#include <atomic>

int main()
{
    static_assert( std::atomic< long long >::is_always_lock_free );
    static_assert( sizeof( std::atomic< long long > ) == 8 );
}
