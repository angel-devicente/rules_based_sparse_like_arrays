#!/usr/bin/env python
# -*- coding: utf-8 -*-

import h5py
import numpy as np

# d1 rules: d1_s, d1_e, val
# d2 rules: d1_s, d1_e, d2_s, d2_e, val
# d3 rules: d1_s, d1_e, d2_s, d2_e, d3_s, d3_e, val
# d4 rules: d1_s, d1_e, d2_s, d2_e, d3_s, d3_e, d4_s, d4_e, val

def main():

    # 3D tests
    
    # Test 1 sample
    # -------------
    dims = [4,100,100]
    print(f'Creating a {len(dims)}D rules-based file for test1 ({dims})')
    test1(dims,"test1_rules.h5")
    print(f'Creating a {len(dims)}D standard file for test1 ({dims}: {(np.prod(dims)*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test1_rules.h5","test1_std.h5")
    
    # Test 2 sample
    # -------------
    dims = [300,1200,400]
    print(f'Creating a {len(dims)}D rules-based file for test2 ({dims})')
    test2(dims,"test2_rules.h5")
    print(f'Creating a {len(dims)}D standard file for test2 ({dims}: {(np.prod(dims)*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test2_rules.h5","test2_std.h5")
    
    # Test 3 sample
    # -------------
    dims = [100,500,100]
    print(f'Creating a {len(dims)}D rules-based file for test3 ({dims})')
    test3(dims,"test3_rules.h5")
    print(f'Creating a {len(dims)}D standard file for test3 ({dims}: {(np.prod(dims)*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test3_rules.h5","test3_std.h5")
    
    # Test 4 sample
    # -------------
    dims = [100,500,100]
    print(f'Creating a {len(dims)}D rules-based file for test4 ({dims})')
    test4(dims,"test4_rules.h5")
    print(f'Creating a {len(dims)}D standard file for test4 ({dims}: {(np.prod(dims)*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test4_rules.h5","test4_std.h5")

    # 5D tests
    
    # Test 5 sample 
    # -------------
    dims = [4,20,10,15,25]
    print(f'Creating a {len(dims)}D rules-based file for test5 ({dims})')
    test5(dims,"test5_rules.h5")
    print(f'Creating a {len(dims)}D standard file for test5 ({dims}: {(np.prod(dims)*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test5_rules.h5","test5_std.h5")

    # Test 6 sample 
    # -------------
    dims = [4,100,36,150,150]
    print(f'Creating a {len(dims)}D rules-based file for test6 ({dims})')
    test6(dims,"test6_rules.h5")
    print(f'Creating a {len(dims)}D standard file for test6 ({dims}: {(np.prod(dims)*8)/(1024*1024):5.2f} MB)')
    write_regular_file("test6_rules.h5","test6_std.h5")
    


def test1(dims,fout):
    """
    a[1:,:,:] = 0
    a[0,0:d2//2,:] = linspace(5,1)
    a[0,d2//2:,:] =  1
    """

    d1,d2,d3 = dims
    ndims = 3
    
    f = h5py.File(fout,'w')
    f.create_group("dsets")
    f.attrs['ndims'] = ndims
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    f.attrs['order'] = [0,1,2]
        
    f['rules/d1'] = [(1,d1-1,0.0)]

    c = d2//2
    d2_attrs = [(0,0,c,d2-1,1.0)]
    degrad_p = np.linspace(1.0,5.0,c)
    degrad_attrs = [(0,0,i,i,degrad_p[len(degrad_p)-1 - i]) for i in range(c)]
    f['rules/d2'] = d2_attrs + degrad_attrs

    f.close()


def test2(dims,fout):
    """
    a[0:,0:2/3*d2,:] = 0
    a[0:,2/3*d2:,:]  = sin
    """

    d1,d2,d3 = dims
    ndims = 3
    
    f = h5py.File(fout,'w')
    f.create_group("dsets")
    f.attrs['ndims'] = ndims
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    f.attrs['order'] = [0,1,2]
    
    f['rules/d1'] = []

    c = 2*d2//3 ; rest = d2 - (c+1)
    d2_attrs = [(0,d1-1,0,c,0.0)]
    
    sin = [np.sin(x) for x in np.linspace(0,2*np.pi,rest)]
    degrad_attrs = [(0,d1-1,c+1+i,c+1+i,sin[i]) for i in range(rest)]
    f['rules/d2'] = d2_attrs + degrad_attrs

    f.close()


def test3(dims,fout):
    """
    Cylinder defined as a dataset in an otherwise empty box.
    """

    d1,d2,d3 = dims
    ndims = 3
    
    f = h5py.File(fout,'w')
    f.create_group("dsets")
    f.attrs['ndims'] = ndims
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    f.attrs['order'] = [0,1,2]
    
    f['rules/d1'] = []

    centre = d2//2 ; r = (d2//10)//2 ; low = centre - r ; high = centre + r 
    f['rules/d2'] = [(0,d1-1,0,low-1,0.0), \
                     (0,d1-1,high+1,d2-1,0.0)]

    h = high-low+1 ; c2 = h // 2 ; c3 = d3 // 2
    f['dsets/cylinder'] = np.full((d1,h,d3),0.0)
    for idx1 in range(d1):
        for idx2 in range(h):
            for idx3 in range(d3):
                d = np.sqrt(abs(idx2-c2)**2 + abs(idx3-c3)**2)
                if d < c2:
                    f['dsets/cylinder'][idx1,idx2,idx3] = 1.0 - (d/c2)
                else:
                    f['dsets/cylinder'][idx1,idx2,idx3] = 0.0

    f['dsets/cylinder'].attrs['d1'] = (0,d1-1)
    f['dsets/cylinder'].attrs['d2'] = (low,high)

    f.close()


def test4(dims,fout):
    """
    Cylinder defined as a dataset in an otherwise empty box.
    As per test 3, but the cylinder is along the third dimension instead, 
    to be able to store it more efficiently
    """
    def circle(idx1,idx2,cd1,cd2,r) :
        d = np.sqrt(abs(idx2-cd2)**2 + abs(idx1-cd1)**2)
        if d < r:
            return 1.0 - (d/r)
        else:
            return 0.0


    d1,d2,d3 = dims
    ndims = 3
            
    f = h5py.File(fout,'w')
    f.create_group("dsets")
    f.attrs['ndims'] = ndims
    f.attrs['dims'] = np.array([d1,d2,d3],dtype=np.int32)
    f.attrs['order'] = [2,1,0]
    
    f['rules/d1'] = []

    cen_d2 = d2//2 ; r = (d2//10)//2 ; low = cen_d2 - r ; high = cen_d2 + r 
    d2_attrs = [(0,d1-1,0,low-1,0.0), \
                (0,d1-1,high+1,d2-1,0.0)]
    cen_d1 = d1//2
    degrad_attrs = [(idx1,idx1,idx2,idx2,circle(idx1,idx2,cen_d1,cen_d2,r))  \
                    for idx1 in range(d1) for idx2 in range(low,high+1) ]
    f['rules/d2'] = d2_attrs + degrad_attrs
    
    f.close()



def test5(dims,fout):
    """
    a[1:,:,:,:,:] = 0
    a[0,d2//2:,:,:,:] =  1
    a[0,0:d2//2,0:d3//3,:,:] = linspace(5,1)
    a[0,0:d2//2,d3//3:,:,:] = linspace(1,5)
    """

    d1,d2,d3,d4,d5 = dims
    ndims = 5
    
    f = h5py.File(fout,'w')
    f.create_group("dsets")
    f.attrs['ndims'] = ndims
    f.attrs['dims'] = np.array([d1,d2,d3,d4,d5],dtype=np.int32)
    f.attrs['order'] = [0,1,2,3,4]
        
    f['rules/d1'] = [(1,d1-1,0.0)]

    c2 = d2//2
    d2_attrs = [(0,0,c2,d2-1,1.0)]
    f['rules/d2'] = d2_attrs

    c3 = d3//2
    degrad_low = np.linspace(5.0,1.0,c3)
    degrad_high = np.linspace(1.0,5.0,d3-c3)
        
    degrad_attrs_low = [(0,0,0,c2-1,i,i,degrad_low[i]) for i in range(c3)]
    degrad_attrs_high = [(0,0,0,c2-1,c3+i,c3+i,degrad_high[i]) for i in range(d3-c3)]
    f['rules/d3'] = degrad_attrs_low + degrad_attrs_high

    f['rules/d4'] = []
    
    f.close()


def test6(dims,fout):
    """
    As test5 but with some random data specified as a dataset

    a[1:,:,:,:,:] = 0
    a[0,d2//2:,:,:,:] =  1
    a[0,0:d2//2,0:d3//3,:,:] = linspace(5,1)
    a[0,0:d2//2,d3//3:d3-1,:,:] = linspace(1,5)
    a[0,0:d2//2,d3,:,:] = random_data
    """

    d1,d2,d3,d4,d5 = dims
    ndims = 5
    
    f = h5py.File(fout,'w')
    f.create_group("dsets")
    f.attrs['ndims'] = ndims
    f.attrs['dims'] = np.array([d1,d2,d3,d4,d5],dtype=np.int32)
    f.attrs['order'] = [0,1,2,3,4]
        
    f['rules/d1'] = [(1,d1-1,0.0)]

    c2 = d2//2
    d2_attrs = [(0,0,c2,d2-1,1.0)]
    f['rules/d2'] = d2_attrs

    c3 = d3//2
    degrad_low = np.linspace(5.0,1.0,c3)
    degrad_high = np.linspace(1.0,5.0,d3-c3-1)
        
    degrad_attrs_low = [(0,0,0,c2-1,i,i,degrad_low[i]) for i in range(c3)]
    degrad_attrs_high = [(0,0,0,c2-1,c3+i,c3+i,degrad_high[i]) for i in range(d3-c3-1)]
    f['rules/d3'] = degrad_attrs_low + degrad_attrs_high

    f['rules/d4'] = []

    f['dsets/random_data'] = np.random.randn(1,c2,1,d4,d5)
    f['dsets/random_data'].attrs['d1'] = (0,0)
    f['dsets/random_data'].attrs['d2'] = (0,c2-1)
    f['dsets/random_data'].attrs['d3'] = (d3-1,d3-1)
    f['dsets/random_data'].attrs['d4'] = (0,d4-1)
    f['dsets/random_data'].attrs['d5'] = (0,d5-1)
    
    f.close()
    
    
def write_regular_file(frules,fstd):
    f = h5py.File(frules,'r')
    fc = h5py.File(fstd,'w')

    ndims = f.attrs['ndims'] 

    if ndims == 3:
        write_regular_file_3D(f,fc)
    elif ndims == 5:
        write_regular_file_5D(f,fc)
                
    f.close()
    fc.close()


def write_regular_file_3D(f,fc):
    (d1,d2,d3) = f.attrs['dims']
    fc['data'] = np.full((d1,d2,d3),-1.0)

    for d1r in f['rules/d1']:
        d1_s, d1_e, d1_v = int(d1r[0]), int(d1r[1]), d1r[2]
        for d1i in range(d1_s,d1_e+1):
            fc['data'][d1i,:,:] = d1_v

    for d2r in f['rules/d2']:
        d1_s, d1_e, d2_s, d2_e, d2_v = int(d2r[0]), int(d2r[1]), int(d2r[2]), int(d2r[3]), d2r[4] 
        for d1i in range(d1_s,d1_e+1):
            for d2i in range(d2_s,d2_e+1):
                fc['data'][d1i,d2i,:] = d2_v

    for ds in f['dsets'].keys():
        d1_s, d1_e = f['dsets/'+ds].attrs['d1']
        d2_s, d2_e = f['dsets/'+ds].attrs['d2']
        fc['data'][d1_s:d1_e+1,d2_s:d2_e+1,:] = f['dsets/'+ds]


def write_regular_file_5D(f,fc):
    (d1,d2,d3,d4,d5) = f.attrs['dims']
    fc['data'] = np.full((d1,d2,d3,d4,d5),-1.0)

    for d1r in f['rules/d1']:
        d1_s, d1_e, d1_v = int(d1r[0]), int(d1r[1]), d1r[2]
        for d1i in range(d1_s,d1_e+1):
            fc['data'][d1i,:,:,:,:] = d1_v

    for d2r in f['rules/d2']:
        d1_s, d1_e, d2_s, d2_e, d2_v = int(d2r[0]), int(d2r[1]), int(d2r[2]), int(d2r[3]), d2r[4] 
        for d1i in range(d1_s,d1_e+1):
            for d2i in range(d2_s,d2_e+1):
                fc['data'][d1i,d2i,:,:,:] = d2_v

    for d3r in f['rules/d3']:
        d1_s, d1_e, d2_s, d2_e, d3_s, d3_e, d3_v = \
            int(d3r[0]), int(d3r[1]), int(d3r[2]), int(d3r[3]), \
            int(d3r[4]), int(d3r[5]), d3r[6] 
        for d1i in range(d1_s,d1_e+1):
            for d2i in range(d2_s,d2_e+1):
                for d3i in range(d3_s,d3_e+1):
                    fc['data'][d1i,d2i,d3i,:,:] = d3_v

    for d4r in f['rules/d4']:
        d1_s, d1_e, d2_s, d2_e, d3_s, d3_e, d4_s, d4_e, d4_v = \
            int(d4r[0]), int(d4r[1]), int(d4r[2]), int(d4r[3]), \
            int(d4r[4]), int(d4r[5]), int(d4r[6]), int(d4r[7]), d4r[8] 
        for d1i in range(d1_s,d1_e+1):
            for d2i in range(d2_s,d2_e+1):
                for d3i in range(d3_s,d3_e+1):
                    for d4i in range(d4_s,d4_e+1):
                        fc['data'][d1i,d2i,d3i,d4i,:] = d4_v

                    
                
    for ds in f['dsets'].keys():
        d1_s, d1_e = f['dsets/'+ds].attrs['d1']
        d2_s, d2_e = f['dsets/'+ds].attrs['d2']
        d3_s, d3_e = f['dsets/'+ds].attrs['d3']
        d4_s, d4_e = f['dsets/'+ds].attrs['d4']        
        fc['data'][d1_s:d1_e+1,d2_s:d2_e+1,d3_s:d3_e+1,d4_s:d4_e+1,:] = f['dsets/'+ds]
        
    
if __name__ == "__main__":
    main()
