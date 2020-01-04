REM - This script assumes it's run from the root directory of the project.

SET root_dir=%CD%

SET PATH=%PATH%;%PYTHON%\Scripts\
SET LP3_ROOT_PATH=%root_dir%\ci\media

REM - TODO: Test other configurations

md output
md output/windows

SET build_dir="output/windows"
cd %build_dir%

echo 'Oh Windows, how I once loved thee. But behind that mask of lies'
echo ''
echo '  O O_-    A spider'
echo ' //;; \\\\   whom wilst strick'
echo '

echo "What version of Python is this?"
python --version

mkdir output
cd output

conan install %root_dir% -pr=..\ci\profiles/msvc --build missing

echo "What is in current directory?"
dir

cat "conan_paths.cmake"

conan build %root_dir%
