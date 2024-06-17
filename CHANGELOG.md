
# Change Log
All notable changes to the CVEDIA-RT SDK will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).
 
## [Unreleased] - 2023-08-22
 
### Added
- [SecuRT] Implemented `restart()` method that restarts an instance preserving the current configuration.
### Changed
 
### Fixed
- [SecuRT] Fixed a problem where a Lua scripting error would cause an instance to hang indefinitely. Instances are now automatically restarted internally.

   
## [0.1.0] - 2023-08-01
  
Initial internal release