#!/bin/bash
if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
  echo "osx system" 
else
  sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  sudo apt-get update -qq
fi

