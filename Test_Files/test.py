#!/usr/bin/env python
# -*- coding: utf-8 -*-

import h5py
import numpy as np

# d1 rules: d1_s, d1_e, val
# d2 rules: d1_s, d1_e, d2_s, d2_e, val

def main():

    # Test 1 sample
    # -------------
    d1=4; d2=100; d3=100
    print(f'Creating rules-based file for test1 ({d1}x{d2}x{d3})')
    test1(d1,d2,d3,"test1_rules.h5")

    print(f'Creating starndard file for test1 ({d1}x{d2}x{d3}: {(d1*d2*d3*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test1_rules.h5","test1_std.h5")


    # Test 2 sample
    # -------------
    d1=300; d2=1200; d3=400
    print(f'Creating rules-based file for test2 ({d1}x{d2}x{d3})')
    test2(d1,d2,d3,"test2_rules.h5")

    print(f'Creating starndard file for test2 ({d1}x{d2}x{d3}: {(d1*d2*d3*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test2_rules.h5","test2_std.h5")

    # Test 3 sample
    # -------------
    d1=100; d2=500; d3=100
    print(f'Creating rules-based file for test3 ({d1}x{d2}x{d3})')
    test3(d1,d2,d3,"test3_rules.h5")

    print(f'Creating starndard file for test3 ({d1}x{d2}x{d3}: {(d1*d2*d3*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test3_rules.h5","test3_std.h5")



def test1(d1,d2,d3,fout):
    """
    a[1:,:,:] = 0
    a[0,0:d2//2,:] = linspace(5,1)
    a[0,d2//2:,:] =  1
    """
    f = h5py.File(fout,'w')
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    
    f.attrs['d1'] = [(1,d1-1,0.0)]

    c = d2//2
    d2_attrs = [(0,0,c,d2-1,1.0)]
    degrad_p = np.linspace(1.0,5.0,c)
    degrad_attrs = [(0,0,i,i,degrad_p[len(degrad_p)-1 - i]) for i in range(c)]
    f.attrs['d2'] = d2_attrs + degrad_attrs

    f.close()


def test2(d1,d2,d3,fout):
    """
    a[0:,0:2/3*d2,:] = 0
    a[0:,2/3*d2:,:]  = sin
    """
    f = h5py.File(fout,'w')
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    
    f.attrs['d1'] = []

    c = 2*d2//3 ; rest = d2 - (c+1)
    d2_attrs = [(0,d1-1,0,c,0.0)]
    
    sin = [np.sin(x) for x in np.linspace(0,2*np.pi,rest)]
    degrad_attrs = [(0,d1-1,c+1+i,c+1+i,sin[i]) for i in range(rest)]
    f.attrs['d2'] = d2_attrs + degrad_attrs

    f.close()


def test3(d1,d2,d3,fout):
    """
    Cylinder defined as a dataset in an otherwise empty box.
    """
    f = h5py.File(fout,'w')
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    
    f.attrs['d1'] = []

    centre = d2//2 ; r = (d2//10)//2 ; low = centre - r ; high = centre + r 
    f.attrs['d2'] = [(0,d1-1,0,low-1,0.0), \
                     (0,d1-1,high+1,d2-1,0.0)]

    h = high-low+1 ; c2 = h // 2 ; c3 = d3 // 2
    f['cylinder'] = np.full((d1,h,d3),0.0)
    for idx1 in range(d1):
        for idx2 in range(h):
            for idx3 in range(d3):
                d = np.sqrt(abs(idx2-c2)**2 + abs(idx3-c3)**2)
                if d < c2:
                    f['cylinder'][idx1,idx2,idx3] = 1.0 - (d/c2)
                else:
                    f['cylinder'][idx1,idx2,idx3] = 0.0

    f['cylinder'].attrs['d1'] = (0,d1-1)
    f['cylinder'].attrs['d2'] = (low,high)

                    
    f.close()


def write_regular_file(frules,fstd):
    f = h5py.File(frules,'r')
    fc = h5py.File(fstd,'w')

    (d1,d2,d3) = f.attrs['dims']
    fc['data'] = np.full((d1,d2,d3),-1.0)

    for d1r in f.attrs['d1']:
        d1_s, d1_e, d1_v = int(d1r[0]), int(d1r[1]), d1r[2]
        for d1i in range(d1_s,d1_e+1):
            fc['data'][d1i,:,:] = d1_v

    for d2r in f.attrs['d2']:
        d1_s, d1_e, d2_s, d2_e, d2_v = int(d2r[0]), int(d2r[1]), int(d2r[2]), int(d2r[3]), d2r[4] 
        for d1i in range(d1_s,d1_e+1):
            for d2i in range(d2_s,d2_e+1):
                fc['data'][d1i,d2i,:] = d2_v

    for ds in f.keys():
        d1_s, d1_e = f[ds].attrs['d1']
        d2_s, d2_e = f[ds].attrs['d2']
        fc['data'][d1_s:d1_e+1,d2_s:d2_e+1,:] = f[ds]
                
    f.close()
    fc.close()

    
if __name__ == "__main__":
    main()
