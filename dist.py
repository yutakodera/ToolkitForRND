#!/usr/bin/env python3
#coding -*- utf-8 -*-

import sys

def CheckArgs(args):
  if len(args) != 3:
    print('[USAGE]')
    print('  ./dist.py filename max_bits')
    print('    filename: select a file cosisting of binary sequence (with the deliminator ",") ')
    print('    max_bits: it denotes the maximum length of bits used for counting up the number of appearances of "max_bits"-bits')
    exit(1)


def SetSequence(filename):
  with open(filename) as f:
    data = f.read()
  seq = data.replace('\n','').replace(',','')
  return seq 


def CountPattern(m,data):
  for n in range(1,m+1):
    print('[{}-bit pattern]'.format(n))
    for i in range(0,2**n):
      counter = 0
      target = bin(i)

    # generate a binary pattern
      target = target[2:]   
      target = target.zfill(n) 
      print(target + ":\t",end='')

    # count up the number of bit patterns 
      j = 0
      temp = data + data[0:n-1]
      while j+n <= len(temp):
        pattern = temp[j:j+n]
        if target == pattern:
          counter += 1
        j += 1 
      print(counter)
  

if __name__ == '__main__':
  args = sys.argv
  CheckArgs(args)

  filename = str(args[1])
  m = int(args[2])

  seq = SetSequence(filename)
  CountPattern(m,seq)

