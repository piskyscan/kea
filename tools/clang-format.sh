#!/bin/sh

# Copyright (C) 2020 Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Usage:
#
# ./tools/clang-format.sh [-d|--debug] [-h|--help] [$directory|$file ...]
#
# Run from the root of the repository to format all C++ files under specified
# directories or specified files or current directory in case of no arguments.

#------------------------------------------------------------------------------#

set -eu

# Customizations
extensions_regex='(\.cpp|\.cc|\.C|\.cxx|\.m|\.hpp|\.hh|\.h|\.H|\.hxx|\.tpp)$'

# Print usage.
print_usage() {
  printf \
'Usage: %s {{options}}
Options:
  [-d|--debug]                 enable debug mode, showing every executed command
  [-h|--help]                  print usage (this text)
' \
    "$(basename "${0}")"
}

# Parse parameters.
while test ${#} -gt 0; do
  case "${1}" in
    # [-d|--debug]             enable debug mode, showing every executed command
    '-d'|'--debug') set -vx ;;

    # [-h|--help]              print usage (this text).
    '-h'|'--help') print_usage ;;

    # Allow extra arguments, they should be directories or files to be formatted.
    *) break ;;
  esac; shift
done

# Get script path.
script_path=$(cd "$(dirname "${0}")" && pwd)

# Use current directory when called without an argument.
if test ${#} = 0; then
  set -- .
fi

# Generated files will be filtered out. Hardcoded list
filtered_out=$(cat "${script_path}/.generated-files.txt")

# For all arguments...
while test ${#} -gt 0; do
  file=${1}

  # Get absolute path.
  if test "$(printf '%s' "${file}" | grep -Eo '^.')" != '/'; then
    basename=$(basename "${file}")
    if test "${basename}" = .; then
      basename=
    fi
    file="$(cd "$(dirname "${file}")" && pwd)/${basename}"
  fi
  printf '%s\n' "${file}"

  if test -f "${file}"; then
    # Format file.
    clang-format --style=file -i "${file}"
  elif test -d "${file}"; then
    # Get list of files to format.
    cd "$(git rev-parse --show-toplevel)"
    files=$(git ls-files | xargs -n1 printf "${PWD}/%s\\n" | grep -F "${file}" \
      | grep -E "${extensions_regex}")

    # Filter out generated files.
    for file in ${filtered_out}; do
      files=$(printf '%s\n' "${files}" | grep -Fv "${file}" | sed '/^$/d')
    done

    # For all files...
    for i in ${files}; do
      "${0}" "${i}"
    done
  fi

  shift
done
