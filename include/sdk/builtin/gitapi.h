/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/

#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <git2/types.h>
#include <git2/oid.h>

#include "defines.h"
#include "rterror.h"

namespace cvedia
{
namespace rt
{
    /**
     * @brief Represents a git repository
     * @details Note: remember to call free() when done using the structure
     */
    struct GitRepository 
    {
        GitRepository() = default;            
        ~GitRepository() = default;

        EXPORT void free();

        GitRepository(GitRepository const&) = default;
        GitRepository& operator=(GitRepository const&) = default;
        GitRepository(GitRepository&&) = default;
        GitRepository& operator=(GitRepository&&) = default;
                    
        git_repository* repository_ = nullptr;                                          // libgit2 repository object         
        std::unordered_map<std::string, std::unordered_set<std::string>> tags_;         // list of all tags in the repository, grouped by tag pattern
        std::unordered_map<std::string, std::string> tagTargets_;                       // git branches for this solution retrieved from repository
        std::string path_;                                                              // path to the repository (e.g. "/home/user/cvediart/solutions/securt")
        std::string uri_;                                                               // uri of the repository (e.g. "https://github.com/cvedia/securt.git")
        bool isRemote_ = false;                                                         // true if the repository has remote origin (i.e. was cloned)
    };

    /**
     * Thin wrapper around libgit2
     */
    class GitAPI
    {
        public:
            EXPORT GitAPI();
            EXPORT ~GitAPI();

            // copy constructor and assignment operators are disabled        
            GitAPI(GitAPI const&) = delete;
            GitAPI& operator=(GitAPI const&) = delete;
            GitAPI(GitAPI&&) = delete;
            GitAPI& operator=(GitAPI&&) = delete;

            EXPORT NODISCARD static std::string GITAPI_USER();
            EXPORT NODISCARD static std::string GITAPI_EMAIL();

            EXPORT NODISCARD static std::string GIT_TAG_PATTERN_ALL();
            EXPORT NODISCARD static std::string GIT_TAG_PATTERN_SOLUTION_VERSIONS();
            EXPORT NODISCARD static std::string GIT_TAG_PATTERN_RT_VERSIONS();

            /**
             * @brief Initialize a new git repository.
             * @details Note: this function does not create any directories.
             * @param path Path to the repository. The directory must exist.
             * @return valid GitRepository structure if successful, error if not
             */                
            EXPORT NODISCARD expected<GitRepository> initRepository(std::string const& path) const;

            /**
             * @brief Initialize a new git repository.
             * @details Note: this function does not create any directories.
             * @param repo GitRepository structure with path_ pointing to valid git repository
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> initRepository(GitRepository const& repo) const;

            /**
             * @brief Opens an existing git repository.             
             * @param path Path to the repository. The directory must exist.
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> openRepository(std::string const& path) const;

            /**
             * @brief Opens an existing git repository.
             * @param repo GitRepository structure with path_ pointing to valid git repository
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> openRepository(GitRepository const& repo) const;

            /**
             * @brief Clones a git repository.
             * @param repo GitRepository structure with path_ and uri_ fields correctly populated             
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> cloneRepository(GitRepository const& repo) const;

            /**
             * @brief Fetches the latest changes from remote repository.
             * @param repo GitRepository structure with valid repository_ pointer
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> fetchRepository(GitRepository const& repo) const;

            /**
             * @brief Reads all tags from the repository.
             * @param repo GitRepository structure with valid repository_ pointer
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> readRepositoryTags(GitRepository const& repo) const;

            /**
             * @brief Reads tags from the repository, matching given pattern.
             * @param repo GitRepository structure with valid repository_ pointer
             * @param pattern Pattern to match tags against
             * @return valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<GitRepository> readRepositoryTagsMatching(GitRepository const& repo, std::string const& pattern) const;

            /**
             * @brief Adds a file to the repository.
             * @param repo GitRepository structure with valid repository_ pointer
             * @param path Path to the file to add
             * @return error if unsuccessful
             */
            EXPORT NODISCARD expected<void> addFile(GitRepository const& repo, std::string const& path) const;

            /**
             * @brief Creates initial (empty) commit
             * @param repo GitRepository structure with valid repository_ pointer
             * @param message Commit message
             * @return error if unsuccessful
             */
            EXPORT NODISCARD expected<void> createInitialCommit(GitRepository const& repo, std::string const& message) const;

            /**
             * @brief Commit current changes
             * @param repo GitRepository structure with valid repository_ pointer
             * @param message Commit message
             * @param parentOid OID of the parent commit
             * @return valid OID if successful, error if not
             */
            EXPORT NODISCARD expected<std::string> commitChanges(GitRepository const& repo, std::string const& message, std::string const& parentOid) const;

            /**
             * @brief Retrieves oid of last commit in HEAD of given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @return valid OID if successful, error if not
             */
            EXPORT NODISCARD expected<std::string> getLastCommitOid(GitRepository const& repo) const;

            /**
             * @brief Creates tag with given name for the latest commit in HEAD of given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @param tag Name of the tag to create
             * @return error if unsuccessful
             */
            EXPORT NODISCARD expected<void> createTagForLatestCommit(GitRepository const& repo, std::string const& tag) const;

            /**
             * @brief Retrieves commit for specified tag
             * @param repo GitRepository structure with valid repository_ pointer
             * @param tag Name of the tag
             * @return valid 40-byte OID hex string of the commit or error if unsuccessful
             */
            EXPORT NODISCARD expected<std::string> getCommitForTag(GitRepository const& repo, std::string const& tag) const;

            /**
             * @brief Checks out specified commit, tag or branch
             * @param repo GitRepository structure with valid repository_ pointer
             * @param spec a textual specification of an object
             * @return error if unsuccessful
             */
            EXPORT NODISCARD expected<void> checkoutBySpec(GitRepository const& repo, std::string const& spec) const;

            /**
             * @brief Saves current changes to stash
             * @param repo GitRepository structure with valid repository_ pointer
             * @param message Optional message to add to the stashed state
             * @returns error if unsuccessful
             */
            EXPORT NODISCARD expected<void> stashSave(GitRepository const& repo, std::string const& message) const;

            /**
             * @brief Apply latest stashed state from the stash list and remove it from the list if successful.
             * @param repo GitRepository structure with valid repository_ pointer
             * @returns error if unsuccessful
             */
            EXPORT NODISCARD expected<void> stashPop(GitRepository const& repo) const;

            /**
             * @brief Drop latest stashed state from the stash list.
             * @param repo GitRepository structure with valid repository_ pointer
             * @returns error if unsuccessful
             */
            EXPORT NODISCARD expected<void> stashDrop(GitRepository const& repo) const;

            /**
             * @brief Resets repository to the specified commit or tag
             * @details Equivalent to `git reset --hard <commitOrTag>`
             * @param repo GitRepository structure with valid repository_ pointer
             * @param commitOrTag commit or tag to reset to
             * @returns valid GitRepository structure if successful, error if not
             */
            EXPORT NODISCARD expected<void> resetHardToCommitOrTag(GitRepository const& repo, std::string const& commitOrTag) const;

            /**
             * @brief Checks if repository has any uncommitted changes
             * @param repo GitRepository structure with valid repository_ pointer
             * @returns true if repository has uncommitted changes, false otherwise
             */
            EXPORT NODISCARD expected<bool> isRepositoryDirty(GitRepository const& repo) const;

            /**
             * @brief Checks if repository has any conflicts
             * @param repo GitRepository structure with valid repository_ pointer
             * @returns true if repository has conflicts, false otherwise
             */
            EXPORT NODISCARD expected<bool> isRepositoryConflict(GitRepository const& repo) const;

            // in the future, this might cover all libgit2 functions if necessary
            // for now we focus only on functionality needed to update solutions

        protected:

            // These are internal helper functions, not meant to be used directly
            // They are exposed for testing purposes only

            /**
             * @brief Gets default signature of given repository
             * @return valid git_signature pointer if successful, error if not
             */
            EXPORT NODISCARD expected<git_signature*> getDefaultSignature() const;

            /**
             * @brief Opens index of given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @return valid git_index pointer if successful, error if not
             */
            EXPORT NODISCARD expected<git_index*> openIndex(GitRepository const& repo) const;

            /**
             * @brief Retrieves OID of reference specified by name from given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @param name Name of the reference
             * @return valid git_oid if successful, error if not
             */
            EXPORT NODISCARD expected<git_oid> getReferenceOid(GitRepository const& repo, std::string const& name) const;

            /**
             * @brief Retrieves OID specified by OID hex string
             * @param hexString valid 40-byte OID hex string
             * @return valid git_oid if successful, error if not
             */
            EXPORT NODISCARD expected<git_oid> getOidFromString(std::string const& hexString) const;

            /**
             * @brief Retrieves OID hex string of given OID              
             * @param oid valid OID from repo
             * @return std::string containing 40-byte OID hex string, error if not
             */
            EXPORT NODISCARD expected<std::string> getStringFromOid(git_oid const& oid) const;

            /**
             * @brief Retrieves commit specified by OID hex string from given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @param hexString valid 40-byte OID hex string of the commit
             * @return valid git_commit pointer if successful, error if not
             */
            EXPORT NODISCARD expected<git_commit*> getCommit(GitRepository const& repo, std::string const& hexString) const;

            /**
             * @brief Retrieves commit specified by OID from given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @param oid OID of the commit
             * @return valid git_commit pointer if successful, error if not
             */
            EXPORT NODISCARD expected<git_commit*> getCommit(GitRepository const& repo, git_oid const& oid) const;

            /**
             * @brief Retrieves git commit specified by name from given repository
             * @param repo GitRepository structure with valid repository_ pointer
             * @param name name of the tag
             * @return valid git_commit pointer if successful, error if not
             */
            EXPORT NODISCARD expected<git_commit*> getCommitByTag(GitRepository const& repo, std::string const& name) const;

            /**
             * @brief Retrieves git commit specified by name from given repository
             * @param commit valid git_commit pointer to a existing commit
             * @return git_oid of the commit if successful, error if not
             */
            EXPORT NODISCARD expected<git_oid const*> getOidFromCommit(git_commit const* commit) const;            
    };

}
}
