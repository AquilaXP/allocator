#include "global_pool.h"

std::aligned_storage<sizeof( global_pool::custum_mm )>::type global_pool::storage;
bool global_pool::init = false;