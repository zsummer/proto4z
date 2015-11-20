#!/bin/bash
if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
  echo "osx system" 
else
  echo "linux system" `whoami`
  sudo --user `whoami` add-apt-repository -y ppa:ubuntu-toolchain-r/test 
  sudo --user `whoami` apt-get update -qq
fi

