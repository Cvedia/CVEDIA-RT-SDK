#pragma once

#include <map>
#include <string>
#include <vector>

#include "cvalue.h"
#include "defines.h"

namespace cvedia {
    namespace rt {
        std::string nChar(std::string ch, size_t nRepeats);

        enum FindingCat { generalError, missingKey, danglingKey, invalidValue };
        static std::map<FindingCat, std::string> FindingCatToStr{
            {FindingCat::generalError, "GENERAL_ERROR"},
            {FindingCat::missingKey, "MISSING_KEY"},
            {FindingCat::danglingKey, "DANGLING_KEY"},
            {FindingCat::invalidValue, "INVALID_VALUE"}
        };

        enum FindingSeverity { info, warning, error };
        static std::map<FindingSeverity, std::string> FindingSeverityToStr{
            {FindingSeverity::info, "INFO"},
            {FindingSeverity::warning, "WARNING"},
            {FindingSeverity::error, "ERROR"}
        };

        class ConfigCheckFinding {
        public:
            EXPORT ConfigCheckFinding(
                std::string key,
                FindingSeverity severity,
                std::string plugin_name,
                std::string configBasePath,
                std::string reason,
                FindingCat cat
            );

            std::string EXPORT composeFindingString();

            FindingSeverity EXPORT getSeverity();

        private:
            std::string _key;
            FindingSeverity _severity;
            std::string _plugin_name;
            std::string _configBasePath;
            std::string _reason;
            FindingCat _cat;

        };

        struct ConfigCheckMeta {
            pCValue pluginConfig{};
            std::string key = "UNKNOWN_KEY";
            std::string pluginName = "UNKNOWN_PLUGIN";
            std::string configBasePath = "UNKNOWN_PATH";
            std::string errorMessage = "UNKNOWN_MESSAGE";
            FindingSeverity severity = FindingSeverity::error;
            FindingCat category = FindingCat::invalidValue;
        };

        class ConfigCheckFunctions {
        public:
            template <typename T>
            static EXPORT bool greater(T value, T thold);

            template <typename T>
            static EXPORT bool greaterEqual(T value, T thold);

            template <typename T>
            static EXPORT bool less(T value, T thold);

            template <typename T>
            static EXPORT bool lessEqual(T value, T thold);

            template <typename T>
            static EXPORT bool inOpenInterval(T value, T min, T max);

            template <typename T>
            static EXPORT bool inLeftOpenInterval(T value, T min, T max);

            template <typename T>
            static EXPORT bool inRightOpenInterval(T value, T min, T max);

            template <typename T>
            static EXPORT bool inClosedInterval(T value, T min, T max);

            template <typename T>
            static EXPORT bool equal(T value, T gt);

            template <typename T>
            static EXPORT bool notEqual(T value, T gt);

            static EXPORT bool startsWith(std::string value, std::string pattern);

            static EXPORT bool endsWith(std::string value, std::string pattern);

            static EXPORT bool contains(std::string value, std::string pattern);

            static EXPORT bool fileDirExists(std::string value);

            static EXPORT bool fileDirExists(std::string value, std::string pathPrefix, std::string pathPostfix);

        };
        class ConfigCheckResults {
        public:
            EXPORT ~ConfigCheckResults();
            EXPORT void addFinding(ConfigCheckFinding finding);
            EXPORT void clear();
            EXPORT size_t warningCount();
            EXPORT size_t errorCount();
            EXPORT size_t findingCount();
            EXPORT std::string prepareReport();
            EXPORT bool warningFound();
            EXPORT bool errorFound();
            EXPORT void checkMissingKeys(STRVECT configKeys, STRVECT groundTruthKeys, std::string pluginName, std::string configBasePath, FindingSeverity severity = FindingSeverity::warning);
            EXPORT void checkDanglingKeys(STRVECT configKeys, STRVECT groundTruthKeys, std::string pluginName, std::string configBasePath, FindingSeverity severity = FindingSeverity::error);

            template <typename T>
            EXPORT void checkValueExists(const ConfigCheckMeta& meta);

            template <typename T>
            EXPORT void checkValue(const ConfigCheckMeta& meta, bool(*checkingFunc)(T));
            template <typename T>
            EXPORT void checkValue(const ConfigCheckMeta& meta, bool(*checkingFunc)(T, T), T param1);
            template <typename T>
            EXPORT void checkValue(const ConfigCheckMeta& meta, bool(*checkingFunc)(T, T, T), T param1, T param2);
            template <typename T>
            EXPORT void checkValue(const ConfigCheckMeta& meta, bool(*checkingFunc)(T, T, T, T), T param1, T param2, T param3);

        private:

            template <typename ... Ts, typename T>
            auto _checkValue(const ConfigCheckMeta& meta, bool(&func)(T value, Ts...), Ts... params) {
                T value{};
                bool result = meta.pluginConfig->getValueIfSet<T>(meta.key, value);
                if (!result) {
                    addFinding(ConfigCheckFinding(
                        meta.key,
                        meta.severity,
                        meta.pluginName,
                        meta.configBasePath,
                        "Cannot access value in config.",
                        FindingCat::invalidValue
                    ));
                    return;
                }
                result = func(value, params...);
                if (!result) {
                    addFinding(ConfigCheckFinding(
                        meta.key,
                        meta.severity,
                        meta.pluginName,
                        meta.configBasePath,
                        meta.errorMessage,
                        meta.category
                    ));
                }
            };

            std::vector<ConfigCheckFinding> _findings{};
            size_t _warningCount = 0;
            size_t _errorCount = 0;
        };
}
}