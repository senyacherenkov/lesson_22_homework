#pragma once

#include <cstddef>
#include "manager.h"


namespace async {

handle_t connect(std::size_t N);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

}
