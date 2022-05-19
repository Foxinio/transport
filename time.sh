#!/bin/bash

MY_EXEC='./cmake-build-debug/src/transport'
FAST_EXEC='../../transport-binaries/transport-client-fast'
SLOW_EXEC='../../transport-binaries/transport-client-slow'


LOCALHOST="127.0.0.1 40001"

function first() {
  echo "FIRST"

  MY_OUT='my.out'
  CLIENT_OUT='client.out'

  if [[ ( -e $MY_OUT ) || ( -e $CLIENT_OUT ) ]]; then
    echo "out files detected"
    exit 1
  fi

  printf My:
  time $MY_EXEC 127.0.0.1 40001 $MY_OUT 15055 > /dev/null
  echo
  printf Slow:
  time $SLOW_EXEC 127.0.0.1 40001 $CLIENT_OUT 15055 > /dev/null

  if cmp --silent -- $MY_OUT $CLIENT_OUT; then
    echo files downloaded correctly
  else
    echo files differ
  fi

  rm -f $CLIENT_OUT $MY_OUT

}
function second() {
  echo "SECOND"
  echo "Use wireshark"
}
function third() {
  echo "THIRD"
  printf My:
  time $MY_EXEC 127.0.0.1 40001 /dev/null 1000000 > /dev/null
  echo
  printf Fast:
  time $FAST_EXEC 127.0.0.1 40001 /dev/null 1000000 > /dev/null

}
function fourth() {
  echo "FOURTH"
  printf My:
  time $MY_EXEC 127.0.0.1 40001 /dev/null 9000000 > /dev/null
  echo
  printf Fast:
  time $FAST_EXEC 127.0.0.1 40001 /dev/null 9000000 > /dev/null

}

if [[ $1 == 1 ]]; then
  first
elif [[ $1 == 2 ]]; then
  second
elif [[ $1 == 3 ]]; then
  third
elif [[ $1 == 4 ]]; then
  fourth
else
  first
  echo "-----------------"
  second
  echo "-----------------"
  third
  echo "-----------------"
  fourth
fi
