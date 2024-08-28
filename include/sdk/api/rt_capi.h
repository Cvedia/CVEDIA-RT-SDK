#pragma once

#include "capi_base.h"

#ifdef __cplusplus
extern "C" {
#endif
	EXPORT int api_initialize();
	EXPORT int api_shutdown();

	EXPORT void api_set_plugin_path(char const* path);
	EXPORT int api_is_dev_mode();
	EXPORT int api_is_running();
	EXPORT void api_set_dev_mode(int devMode);
	EXPORT void api_start_webserver();
	EXPORT void api_stop_webserver();
#ifdef __cplusplus
}
#endif
