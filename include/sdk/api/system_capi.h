#pragma once

#include "capi_base.h"

#ifdef __cplusplus
extern "C" {
#endif
	EXPORT void system_set_rt_home_dir(char const* path);
	EXPORT char* system_registry_get_keys();
	EXPORT char* system_registry_get_key_value(char const* key);
#ifdef __cplusplus
}
#endif
