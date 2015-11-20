#!/bin/bash
if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
  echo "osx system" 
else
  add-apt-repository -y ppa:ubuntu-toolchain-r/test
  apt-get update -qq
fi

