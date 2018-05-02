# install cmake:
mkdir -p ${HUNTER_ROOT}
mkdir -p ${DEPS_DIR} && cd ${DEPS_DIR}
if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
  CMAKE_URL="https://cmake.org/files/v3.11/cmake-3.11.1-Linux-x86_64.tar.gz"
  mkdir cmake && travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
  export PATH=${DEPS_DIR}/cmake/bin:${PATH}
elif [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then
  CMAKE_URL="https://cmake.org/files/v3.11/cmake-3.11.1-Darwin-x86_64.tar.gz"
  mkdir cmake && travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=3 -xz -C cmake
  export PATH=${DEPS_DIR}/cmake/bin:${PATH}
fi

# compile
export CXX=${COMPILER}
cmake -H. -BBuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -Wdev
cd Build
make -j2

# test
ctest -V