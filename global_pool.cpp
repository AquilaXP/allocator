#include "global_pool.h"

std::aligned_storage_t<sizeof( global_pool::custum_mm )> global_pool::storage;
bool global_pool::init = false;