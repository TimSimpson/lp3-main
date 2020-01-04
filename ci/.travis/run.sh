#!/bin/bash
set -e
set -x

readonly scripts_dir="$(readlink -f "$(dirname "${BASH_SOURCE[0]}")")"

if [[ "$(uname -s)" == 'Darwin' ]]; then
    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

conan install
"${scripts_dir}"/../run.sh
