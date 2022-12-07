#!/bin/bash

# SPDX-FileCopyrightText: 2022 CVEDIA LTD
#
# SPDX-License-Identifier: LicenseRef-CVEDIA

NO_COLOR=${NO_COLOR:-0}
HOSTNAME="${HOSTNAME:-$(hostname)}"
MUTE=${MUTE-0}
BUILD_OUTPUT_PATH=${BUILD_OUTPUT_PATH-out}

PRESET="${1-linux64-RelWithDebInfo}"
BUILD_PARAMS=${@:2}

log () {
    if [ "$MUTE" -eq "1" ] ; then
        return
    fi
    if [ "$NO_COLOR" = "1" ] ; then
        ERROR=
        INFO=
        WARN=
        SQL=
        C_N=
    else
        ERROR='\033[1;31m'
        INFO='\033[1;32m'
        WARN='\033[1;33m'
        TRACE='\033[1;35m'
        DEBUG='\033[1;36m'
        SQL='\033[1;34m'
        C_N='\033[0m'
    fi
    level=INFO
    
    if [ -n "$1" ] ; then
        level=$1
        IN="${@:2}"
    else
        read -r IN
    fi

    local TS
    TS=$(date +%Y-%m-%d\ %H:%M:%S.%N)
    MM=
    
    if [ -n "$ME" ]; then
        MM=":$ME"
    fi

    echo -e "${C_N}${!level}${TS::-6} $HOSTNAME $level [$$$MM] $IN${C_N}"
}

check_exit () {
    RT=$1
    ERROR_MESSAGE=${@:2}
    if [ "$RT" -ne "0" ]; then
        log WARN "-- Exit code: $RT"
        if [ -n "$ERROR_MESSAGE" ] ; then
            ERROR_MESSAGE="- $ERROR_MESSAGE"
        fi
        log ERROR "Error, aborting $ERROR_MESSAGE"
        if [ -n "$PID" ]; then
            kill $PID
        fi
        exit 1
    fi
}

for i in PRESET BUILD_PARAMS BUILD_OUTPUT_PATH; do
    log INFO "-- ${i}: ${!i}"
done

mkdir -p "$BUILD_OUTPUT_PATH" &> /dev/null

log INFO "-- Generating..."
cmake -S ./cvedia-rt/src/ --preset=${PRESET} ${BUILD_PARAMS}
check_exit $?

log INFO "-- Building..."
cmake --build ${BUILD_OUTPUT_PATH}/${PRESET}/build/ -- -j$(nproc)
check_exit $?

log INFO "-- Installing..."
cmake --install ${BUILD_OUTPUT_PATH}/${PRESET}/build/ --strip --prefix ${BUILD_OUTPUT_PATH}/${PRESET}/redist
check_exit $?

log INFO "-- Completed, ${SECONDS}s wasted"