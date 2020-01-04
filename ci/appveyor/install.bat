set PYTHON="C:\Python37"
set PATH=%PYTHON%\Scripts;%PATH%

pip.exe install conan --upgrade

conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan user
