/*
    SPDX-FileCopyrightText: 2023 CVEDIA LTD

    SPDX-License-Identifier: LicenseRef-CVEDIA
*/

#pragma once
#include "cvalue.h"
#include "rterror.h"

#include <unordered_map>
#include <unordered_set>

namespace cvedia
{
namespace rt
{
namespace iface
{
	/**
	 *  A pair of strings describing a solution that can be updated.
	 *  First string contains the name of solution folder inside solutionsDir to be updated,
	 *  second string contains the uri of the solution from which the update can be performed.
	 *
	 *  Example: "SecuRT", "https://gitlab.com/cvedia/securt"
	 */
	using SolutionUpdateDescriptor = std::pair<std::string, std::string>;

	/**
	 *  A vector containing one or more solution update descriptors
	 */
	using SolutionUpdateDescriptors = std::vector<SolutionUpdateDescriptor>;

    /**
     *  A structure containing all possible information regarding solution's update process.     
	 */
    struct SolutionUpdateInfo
    {
        std::string name_;                                                          // name of the folder containing the solution (e.g. "securt")
        std::string uri_;                                                           // uri of git repository for the solution (e.g. "https://github.com/cvedia/securt.git")
        std::string path_;                                                          // path to the folder containing the solution (e.g. "/home/user/cvediart/solutions/securt")
        
        bool exists_ = false;                                                       // whether the solution exists in the specified directory
        bool git_ = false;                                                          // whether the solution is a valid git repository
        bool isRemote_ = false;                                                     // whether the solution is a remote repository
        bool isDirty_ = false;                                                      // whether the solution has uncommitted changes
        bool isConflict_ = false;                                                   // whether the solution has merge conflicts

        std::string currentTag_;                                                    // currently checked out solution version tag (if there is one)
        std::string targetTag_;                                                     // version tag to update to (optional)

        std::unordered_map<std::string, std::unordered_set<std::string>> tags_;     // git tags for this solution retrieved from repository
        std::unordered_map<std::string, std::string> tagTargets_;                   // git branches for this solution retrieved from repository
        std::unordered_map<std::string, std::vector<std::string>> versions_;        // available version tags for this solution, grouped by RT version
    };

	/**
	 *  A vector of solution update info structures
	 */
	using SolutionUpdateInfos = std::vector<SolutionUpdateInfo>;

    /**
     *  Generic solution updater interface, used as a base class for all functionality related to solution updating
     */
    class EXPORT_CLASS SolutionUpdater
    {
        public:
            SolutionUpdater() = default;
            virtual ~SolutionUpdater() = default;

            // copy constructor and assignment operators are disabled        
            SolutionUpdater(const SolutionUpdater&) = delete;
            SolutionUpdater& operator=(const SolutionUpdater&) = delete;
            SolutionUpdater(SolutionUpdater&&) = delete;
            SolutionUpdater& operator=(SolutionUpdater&&) = delete;

            /**
             * @brief Retrieves ID of updater implementation
             * @details Can be used to point updater API to correct implementation to use
             * @return Updater implementation ID
             */
            virtual std::string getId() = 0;

            /**
             * @brief Used to initialize the solution updater before the update
             * @details This can be used to prepare the updater for the update phase, e.g. by providing
             *          necessary input data (list of solutions that will be updated), or by performing some required initialization steps.
             * @param solutionsToCheck optional parameter containing a vector of solution name/uri pairs. If provided, only solutions from this vector will be checked
             *        to verify they're available for update. By default you can leave this empty and initialize() will automatically retrieve this info from rtconfig.json
             * @param solutionsDir optional path to directory to search for solutions, by default this is retrieved automatically from RTCORE solutions API              
             * @returns Output data as a vector of solution update info structures or an error if it occurs.
             */
            virtual expected<SolutionUpdateInfos> initialize(const SolutionUpdateDescriptors& solutionsToCheck = {}, const std::string& solutionsDir = "") = 0;

            /**
             * @brief Used to prepare for update and perform necessary pre-update operations
             * @details This can be used to perform necessary operations before the actual update is executed, e.g. a batch of git commands or some automatic cleanup.
             * @param solutionUpdateInfo a structure specifying a single solution to update. Usually this is one of the structures returned by initialize()
             * @param solutionsDir optional path to directory to search for solutions, by default this is retrieved automatically from RTCORE solutions API  
             * @returns Output data as a single solution update info structure or an error if it occurs.
             */
            virtual expected<SolutionUpdateInfo> prepareUpdate(const SolutionUpdateInfo& solutionUpdateInfo, const std::string& solutionsDir = "") = 0;

            /**
             * @brief Used to perform the actual update operation
             * @details This is the main function of the updater, where the actual update is performed.
             * @param solutionUpdateInfo a structure specifying a single solution to update. Usually this is one of the structures returned by initialize()
             * @param solutionsDir optional path to directory to search for solutions, by default this is retrieved automatically from RTCORE solutions API  
             * @returns Output data as a single solution update info structure or an error if it occurs.
             */
            virtual expected<SolutionUpdateInfo> performUpdate(const SolutionUpdateInfo& solutionUpdateInfo, const std::string& solutionsDir = "") = 0;

            /**
             * @brief Used to finish the update or perform additional operations
             * @details This is used to perform required operations after the update, e.g. to fix the issue or retry the update with different options.
             * @param solutionUpdateInfo a structure specifying a single solution to update. Usually this is one of the structures returned by initialize()
             * @param solutionsDir optional path to directory to search for solutions, by default this is retrieved automatically from RTCORE solutions API  
             * @returns Output data as a single solution update info structure or an error if it occurs.
             */
            virtual expected<SolutionUpdateInfo> finishUpdate(const SolutionUpdateInfo& solutionUpdateInfo, const std::string& solutionsDir = "") = 0;

            /**
             * @brief Used to cancel and rollback the update
             * @details This can be used to perform some cleanup/rollback after the update is cancelled.
             * @param solutionUpdateInfo a structure specifying a single solution to update. Usually this is one of the structures returned by initialize()
             * @param solutionsDir optional path to directory to search for solutions, by default this is retrieved automatically from RTCORE solutions API  
             * @returns Output data as a single solution update info structure or an error if it occurs.
             */
            virtual expected<SolutionUpdateInfo> cancelUpdate(const SolutionUpdateInfo& solutionUpdateInfo, const std::string& solutionsDir = "") = 0;

            /**
             * @brief Used to shut down and clean after the updater
             * @details This can be used to perform some cleanup before the updater is destroyed.
             * @returns Nothing or an error if it occurs.
             */
            virtual expected<void> shutdown() = 0;                
    };
}
}
}
