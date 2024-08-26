/*
    SPDX-FileCopyrightText: 2023 CVEDIA LTD

    SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <iostream>

namespace cvedia {
    namespace rt {
        typedef std::array<uint8_t, 16> UuidBytes;

        class Uuid : public UuidBytes {
        public:
            // Default constructor
            Uuid() {
                std::fill(begin(), end(), 0);
            }

            // Constructor to create a UUID from an array of bytes
            explicit Uuid(UuidBytes const& bytes) : UuidBytes(bytes) {
            }

            // Constructor to create a UUID from a string
            explicit Uuid(std::string const& str) {
                if (str.size() != 36 || !isHexString(str)) {
                    std::fill(begin(), end(), 0);
                    return;
                }

                std::stringstream ss;
                for (size_t i = 0, j = 0; i < str.size(); ++i) {
                    if (str[i] != '-') {
                        ss << std::hex << str.substr(i, 2);
                        unsigned int byteValue;
                        ss >> byteValue;
                        (*this)[j++] = static_cast<uint8_t>(byteValue);
                        ss.clear();
                        i++;  // Skip next char as we took two chars
                    }
                }
            }

            // Method to generate a random UUID
            static Uuid randomUuid() {
                Uuid randUuid;
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, 255);
                for (auto& byte : randUuid) {
                    byte = static_cast<unsigned char>(dis(gen));
                }
                return randUuid;
            }

            // Method to export the UUID back to a string
            std::string toString() const {
                std::stringstream ss;
                ss << std::hex << std::setfill('0');
                for (size_t i = 0; i < size(); ++i) {
                    ss << std::setw(2) << static_cast<int>((*this)[i]);
                    if (i == 3 || i == 5 || i == 7 || i == 9)
                        ss << '-';
                }
                return ss.str();
            }

            // Accessor method for the internal data
            UuidBytes const& getData() const {
                return *this;
            }

            // Method to check if the UUID is valid
            bool isValid() const {
                return std::any_of(begin(), end(), [](uint8_t byte) { return byte != 0; });
            }

            // Equality operator
            bool operator==(Uuid const& other) const {
                return static_cast<UuidBytes>(*this) == static_cast<UuidBytes>(other);
            }

            // Inequality operator
            bool operator!=(Uuid const& other) const {
                return !(*this == other);
            }

        private:
            // Method to check if a string is a valid hex string
            bool isHexString(std::string const& str) {
                static std::string const hexChars = "0123456789abcdefABCDEF-";
                for (char c : str) {
                    if (hexChars.find(c) == std::string::npos)
                        return false;
                }
                return true;
            }
        };
    }
}

namespace std {
    template<>
    struct hash<cvedia::rt::Uuid> {
        hash() = default;  // add this line

        size_t operator()(cvedia::rt::Uuid const& uuid) const {
	        auto const& data = uuid.getData();
            size_t hashValue = 0;
            for (auto const& byte : data) {
                // combine the hash of each byte into the final hash
                hashValue ^= hash<uint8_t>{}(byte)+0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
            return hashValue;
        }
    };

    // Overload the << operator for easy printing
    inline std::ostream& operator<<(std::ostream& os, cvedia::rt::Uuid const& uuid) {
        os << uuid.toString();
        return os;
    }

    inline std::wostream& operator<<(std::wostream& wos, cvedia::rt::Uuid const& uuid) {
        wos << uuid.toString().c_str();
        return wos;
    }
}
