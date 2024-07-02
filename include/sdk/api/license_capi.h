#pragma once

#include "capi_base.h"

#ifdef __cplusplus
extern "C" {
#endif
	EXPORT char* license_path_to_licenses_directory();
	EXPORT int license_get_current_license_keys(char** keysJson);
	EXPORT int license_get_current_license_info(char** infoJson);
	EXPORT int license_add_license_key(char const* licenseKey);
	EXPORT int license_remove_license_key(char const* licenseKey, int force);
	EXPORT int license_load_licenses();
	EXPORT int license_unload_licenses(); 
#ifdef __cplusplus
}
#endif
