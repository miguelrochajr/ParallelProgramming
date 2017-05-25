#!/bin/bash

if [[ $# -eq 0 ]]
then
  echo "When you wanto to send <MyFILE> to the <USERNAME> in the super computer, do the following:"
  echo "./send2NPAD.sh <MyFILE> <USERNAME>"
else
  scp -r -P4422 "${PWD}"/"$1" "$2"@sc.npad.imd.ufrn.br:~/
fi

exit 0
