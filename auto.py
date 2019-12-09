#! /usr/bin/env python3
#-*- coding: utf-8 -*-

import sys
import csv

def CheckArgs(args):
  if len(args) != 3:
    print('./auto.py infile outfile')
    print('[USAGE]')
    print('    infile: select a file cosisting of binary sequence (with the deliminator ",") ')
    print('    outfile: indicate a file name for saving the result')

    if len(args) == 2:
      print('[CAUTION]')
      print('  !!! outfile is not set')
      print('      Do you agree to use the file name "auto.csv" for the output?')
      print('      [y (yes)/n (no)]')
      consent = input('       >> ')
      if ((consent == 'y') or (consent == 'y'.upper()) or (consent == 'yes') or (consent == 'yes'.upper())):
        return args[1],'auto.csv'
      else:
        exit(1)
    else:
      exit(1)
  return args[1],args[2]


def SetSequence(filename):
  with open(filename) as f:
    data = f.read()
  seq = data.replace('\n','').replace(',','')
  return seq


def AutoCorrelation(seq,outfile):
  with open(outfile,'w') as f:
    writer = csv.writer(f,lineterminator="\n")

    for x in range(0,len(seq)):
      R = 0
      data = []
      for i in range(0,len(seq)):
        if int(seq[i]) ^ int(seq[(i+x)%len(seq)]) == 0:
          R = R + 1
        elif int(seq[i]) ^ int(seq[(i+x)%len(seq)]) == 1:
          R = R - 1
      data.append(x)
      data.append(R)
      writer.writerow(data)


if __name__ == '__main__':
  args = sys.argv
  infile,outfile = CheckArgs(args)

  print('infile: ' + infile)
  print('outfile: ' + outfile)

  seq = SetSequence(infile)
  AutoCorrelation(seq,outfile)
