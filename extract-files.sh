#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017 The LineageOS Project
#           (C) 2017 The CMrience Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

DEVICE=ef78s
VENDOR=pantech

# Load extractutils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

CM_ROOT="$MY_DIR"/../../..

HELPER="$CM_ROOT"/vendor/cm/build/tools/extract_utils.sh
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

if [ $# -gt 3 ]; then
    echo "usage: $0 [PATH_TO_EXPANDED_ROM] [PATH_TO_QC_BLOBS] [PATH_TO_BULLHEAD_BLOBS]"
    echo ""
    echo "PATH_TO_EXPANDED_ROM defaults to adb. PATH_TO_QC_BLOBS defaults to the"
    echo "extracted LG G4 blobs in vendor/lge/g4-common/proprietary."
    exit 1
fi

SRC="${1:-adb}"
SRC_QC="${2:-$CM_ROOT/vendor/lge/g4-common/proprietary}"
SRC_BULLHEAD="${3:-$CM_ROOT/vendor/lge/bullhead/proprietary}"

setup_vendor "$DEVICE" "$VENDOR" "$CM_ROOT"

# Each section of proprietary-files-qc.txt names the device its blobs come from
extract_qc() {
    local list
    list=$(mktemp)
    awk -v re="- from ($1) " -v invert="$3" \
        '/^#/ { on = (($0 ~ re) != (invert != "")) } on && !/^#/' \
        "$MY_DIR"/proprietary-files-qc.txt > "$list"
    extract "$list" "$2"
    rm -f "$list"
}

extract "$MY_DIR"/proprietary-files.txt "$SRC"
extract_qc "bullhead" "$SRC_QC" invert
extract_qc "bullhead" "$SRC_BULLHEAD"

"$MY_DIR"/setup-makefiles.sh
