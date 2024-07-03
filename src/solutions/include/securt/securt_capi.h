#pragma once

#include "api/capi_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new SecuRT instance with a given ID and name.
 *
 * @param handle The context handle of the SecuRT instance to create.
 * @param instanceName User friendly name for the new SecuRT instance. Must be a null-terminated string.
 *
 * @return On success, the handle (integer > 0) to the created SecuRT instance is returned. On failure, -1 is returned.
 *
 * @note The function returns -1 if the instanceId already exists or if SecuRT cannot be initialized (see the log for details).
 */
EXPORT int securt_create_instance(int const handle, char const* instanceName);

/**
 * Destroys an existing SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance to destroy.
 *
 * @return 1 if the SecuRT instance was successfully destroyed, 0 if it did not exist.
 */
EXPORT int securt_destroy_instance(int const handle);

/**
 * Creates a new area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param ignoreStationaryObjects If true, stationary objects will be ignored in the area.
 * @param eventType The type of event to trigger in this area (1 = Enter, 2 = Exit, 3 = Enter & Exit).
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_crossing_area(int const handle, char const* areaId, char const* name, float const* coords,
                                       int numPoints, int const* classes, int classesSize, int ignoreStationaryObjects,
                                       int eventType, double const* color);

/**
 * Creates a new intrusion area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new intrusion area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance.
 */
EXPORT int securt_create_intrusion_area(int const handle, char const* areaId, char const* name, float const* coords,
                                        int numPoints, int const* classes, int classesSize, double const* color);


/**
 * Creates a new loitering area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new loitering area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param seconds The duration in seconds for an object to be considered as loitering.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance.
 */
EXPORT int securt_create_loitering_area(int const handle, char const* areaId, char const* name, float const* coords,
                                        int numPoints, int const* classes, int classesSize, int seconds,
                                        double const* color);


/**
 * Creates a new crowding area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new crowding area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param objectCnt The count of objects that constitutes crowding.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance.
 */
EXPORT int securt_create_crowding_area(int const handle, char const* areaId, char const* name, float const* coords,
                                       int numPoints, int const* classes, int classesSize, int objectCnt,
                                       double const* color);

/**
 * Creates a new line crossing in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param lineId Unique identifier for the new line crossing area. Must be a null-terminated string.
 * @param name Name for the new line crossing area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the line (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param direction The direction of line crossing to be detected (1 = Up, 2 = Down, 3 = Both).
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the line was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the lineId already exists in the instance.
 */
EXPORT int securt_create_line_crossing(int const handle, char const* lineId, char const* name, float const* coords,
                                       int numPoints, int const* classes, int classesSize, int direction,
                                       double const* color);

/**
 * Creates a new armed person area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_armed_person_area(int const handle, char const* areaId, char const* name, float const* coords,
                                           int numPoints, double const* color);

/**
 * Creates a new object left area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param minDuration The minimum duration in seconds for an object to be considered as left.
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_object_left_area(int const handle, char const* areaId, char const* name, float const* coords,
    int numPoints, int minDuration, double const* color);

/**
 * Creates a new object removed area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param minDuration The minimum duration in seconds for an object to be considered as removed.
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_object_removed_area(int const handle, char const* areaId, char const* name, float const* coords,
    int numPoints, int minDuration, double const* color);

/**
 * Creates a new tailgating line in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param lineId Unique identifier for the new line. Must be a null-terminated string.
 * @param name Name for the new line. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the line (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param maxDuration The maximum duration in seconds for an object to be considered as tailgating.
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the line was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the lineId already exists in the instance.
 */
EXPORT int securt_create_tailgating_line(int const handle, char const* lineId, char const* name, float const* coords,
    int numPoints, int const* classes, int classesSize, int maxDuration, int direction, double const* color);

/**
 * Creates a new fallen person area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_fallen_person_area(int const handle, char const* areaId, char const* name, float const* coords,
	int numPoints, double const* color);

/**
 * Creates a new license plate access control area in the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_license_control_area(int const handle, char const* areaId, char const* name, float const* coords,
	int numPoints, double const* color);

/**
 * @brief Defines a motion area within a specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param coords Array of points defining the area. The array should contain (x, y) coordinates in the following format: (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @return Returns 1 if the motion area was successfully defined, 0 otherwise.
 */
EXPORT int securt_set_motion_area(int const handle, float const* coords, int const numPoints);

/**
 * @brief Defines multiple areas to be ignored within a specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param coords The array should contain (x, y) coordinates in the following format: (x1, y1, x2, y2, ...). An entry with (-1, -1) is used to separate the different areas.
 * @param numPoints The total size of the coordinates array.
 * @return Returns 1 if the motion area was successfully defined, 0 otherwise.
 */
EXPORT int securt_set_exclusion_areas(int const handle, float const* coords, int const numPoints);

/**
 * @brief Deletes an existing area within a specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param areaId The unique identifier for the area to be deleted. This must be a null-terminated string.
 * @return Returns 1 if the area was successfully deleted, 0 otherwise.
 */
EXPORT int securt_delete_area(int const handle, char const* areaId);

/**
 * @brief Deletes an existing line within a specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param lineId The unique identifier for the line to be deleted. This must be a null-terminated string.
 * @return Returns 1 if the line was successfully deleted, 0 otherwise.
 */
EXPORT int securt_delete_line(int const handle, char const* lineId);

/**
 * @brief Sets the appearance search mode of the specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 * @param mode The appearance search mode to be set (0 = Off, 1 = Person, 2 = Vehicle, 3 = Person & Vehicle).
 *
 * @note The default appearance search mode is Off.
 */
EXPORT int securt_set_appearance_search(int const handle, int const mode);

/**
 * @brief Calculates motion mask for specified SecuRT instance.
 *
 * @param handle The context handle of the SecuRT instance.
 *
 */
EXPORT int securt_calculate_motion_mask(int const handle);

#ifdef __cplusplus
}
#endif
