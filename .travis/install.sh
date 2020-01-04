#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    brew update || brew update
    brew outdated pyenv || brew upgrade pyenv
    brew install pyenv-virtualenv
    brew install cmake || true

    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi

    pyenv install 3.6.1
    pyenv virtualenv 3.6.1 conan
    pyenv rehash
    pyenv activate conan
fi

pip install aws
pip install conan --upgrade
pip install conan_package_tools

conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan

conan remote add richter https://api.bintray.com/conan/timsimpson/richter
conan user
