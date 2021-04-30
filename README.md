
# Table of Contents

1.  [Rules Based Sparse Like Arrays](#org7075bc2)
    1.  [Rules for 3D arrays](#org63e520b)
        1.  [d1 rules](#orgb5f4c3a)
        2.  [d2 rules](#orgaacf7ce)
        3.  [Explicit data values](#org8fc4e5b)
    2.  [Rules for 5D arrays](#orgb569b5d)
    3.  [Creating test arrays (\`Test\_Files/test.py\`)](#org12ad440)
        1.  [Test 1 (3D)](#orga489bfb)
        2.  [Test 2 (3D)](#org12f5c07)
        3.  [Test 3 (3D)](#org0f5d88a)
        4.  [Test 4 (3D)](#orgcf2b1c0)
        5.  [Test 5 (5D)](#org63426ad)
        6.  [Test 6 (5D)](#orgc5747f9)
    4.  [In-memory RBSLA (C code)](#org8f5d905)
        1.  [Verification of correct implementation.](#orgd31ff92)
        2.  [Verification of reduced memory size of RBSLA in-memory structure](#org38fda0f)
        3.  [Performance of RBSLA](#org97945c2)



<a id="org7075bc2"></a>

# Rules Based Sparse Like Arrays

Arrays are usually stored in memory as dense arrays (where every element of the
array is stored in memory) or as sparse arrays (where only non-zero values are
stored. These are usually used for arrays that have a small fraction of their
values as non-zero values, usually in or near the diagonals. Sparse arrays can
be implemented in different ways).

For some applications, when we might have non-zero values anywhere in the array,
but many of the array positions have the same value, we might approach the
storage of these arrays in a different way. This is what we do here for 3D
arrays with what we call Rules Based Sparse Like Arrays, which in certain cases
can be used to store the arrays (both in disk and in memory) in a much more
compact way than its equivalent dense array representation (using **orders of
magnitude less space and with no significant performance penalty**).

(graphical explanation of the building up of the RBSLA to be added)


<a id="org63e520b"></a>

## Rules for 3D arrays

For 3D arrays, the available rules to define RBSLA data are only two and are
very simple. The test arrays created with the \`test.py\` script should help to
clarify how these rules are used.


<a id="orgb5f4c3a"></a>

### d1 rules

With these rules we restrict the range in the first dimension. In pseudo-code
this would represent statements such as:

    a[1,:,:]   = value
    a[2:4,:,:] = value

so that a range in the first dimension is given, but the second and third
dimensions cannot be sub-ranged.

These rules are given by just three values: 

    d1_s d1_e val

where d1\_s is the beginning of the range in the first dimension, d1\_e is the
end of the range in the first dimension, and val is the value to be set for
that sub-array.

Thus, the pseudo-code

    a[1,:,:]   = 0.0
    a[2:5,:,:] = 2.0

would be given by the d1 rules

    1 1 0.0
    2 5 2.0


<a id="orgaacf7ce"></a>

### d2 rules

These rules are very similar to d1 rules, but in this case, we can give
sub-ranges for both the first and the second dimensions. These rules are given
by five values (similar to d1 rules, but now we also provide beginning and end
ranges for the second dimension):

    d1_s d1_e d2_s d2_e val

Thus, the pseudo-code

    a[1,2:4,:]   = 0.0
    a[2:5,3:9,:] = 2.0

would be given by the d2 rules

    1 1 2 4 0.0
    2 5 3 9 2.0


<a id="org8fc4e5b"></a>

### Explicit data values

For those values that cannot be represented with the above rules, we provide
as many 3D dense sub-arrays as necessary, where we also specify the location where
the sub-array fits in the main array. 


<a id="orgb569b5d"></a>

## Rules for 5D arrays

For 5D arrays, the available rules to define RBSLA data are four and follow the
same idea as for the 3D arrays above, but the rules extend to four levels
(i.e. d1, d2, d3 and d4 rules), while the explicit data values are represented
by 5D dense sub-arrays. 


<a id="org12ad440"></a>

## Creating test arrays (\`Test\_Files/test.py\`)

The Python script \`test.py\` creates six "rules-based" and "standard" arrays
(four in 3D, two in 5D), storing them in HDF5 format. As we can see below, the
size of the "rules-based" files (\`testX\_rules.h5\`) is much smaller than the
"standard" files (\`testX\_std.h5\`). How much smaller depends on how efficiently
the rules format (explained below) can represent the data of interest.

    $ ./test.py
    Creating a 3D rules-based file for test1 ([4, 100, 100])
    Creating a 3D standard file for test1 ([4, 100, 100]:  0.31 MB)
    Creating a 3D rules-based file for test2 ([300, 1200, 400])
    Creating a 3D standard file for test2 ([300, 1200, 400]: 1098.63 MB)
    Creating a 3D rules-based file for test3 ([100, 500, 100])
    Creating a 3D standard file for test3 ([100, 500, 100]: 38.15 MB)
    Creating a 3D rules-based file for test4 ([100, 500, 100])
    Creating a 3D standard file for test4 ([100, 500, 100]: 38.15 MB)
    Creating a 5D rules-based file for test5 ([4, 20, 10, 15, 25])
    Creating a 5D standard file for test5 ([4, 20, 10, 15, 25]:  2.29 MB)
    Creating a 5D rules-based file for test6 ([4, 100, 36, 150, 150])
    Creating a 5D standard file for test6 ([4, 100, 36, 150, 150]: 2471.92 MB)
    
    $ ls -ltrh *h5
    -rw-r--r-- 1 angelv angelv 8.4K May  1 08:18 test1_rules.h5
    -rw-r--r-- 1 angelv angelv 315K May  1 08:18 test1_std.h5
    -rw-r--r-- 1 angelv angelv  20K May  1 08:18 test2_rules.h5
    -rw-r--r-- 1 angelv angelv 1.1G May  1 08:19 test2_std.h5
    -rw-r--r-- 1 angelv angelv 3.9M May  1 08:21 test3_rules.h5
    -rw-r--r-- 1 angelv angelv  39M May  1 08:21 test3_std.h5
    -rw-r--r-- 1 angelv angelv 203K May  1 08:21 test4_rules.h5
    -rw-r--r-- 1 angelv angelv  39M May  1 08:21 test4_std.h5
    -rw-r--r-- 1 angelv angelv 6.2K May  1 08:21 test5_rules.h5
    -rw-r--r-- 1 angelv angelv 2.3M May  1 08:21 test5_std.h5
    -rw-r--r-- 1 angelv angelv 8.6M May  1 08:21 test6_rules.h5
    -rw-r--r-- 1 angelv angelv 2.5G May  1 08:22 test6_std.h5


<a id="orga489bfb"></a>

### Test 1 (3D)

In this first test, all the planes (except for the first one) in the array first
dimension (Z axis in the image below) are set to 0. In the first plane, the
values in the second dimension (Y axis in the image) range from 5 at the bottom
to 1 at the center, and are set to 1 in the top half, while being constant for
the third dimension (X axis in the image).

In pseudo-code, this array could be created as (\*\*note that the ranges below
represent inclusive indices, so for example a (0,0) range means the index 0, not
as in the Python way\*\*):

    a[1:,:,:] = 0
    a[0,0:half_d2,:] = linspace(5,1)
    a[0,half_d2:,:] =  1

The \`test.py\` array creates for this test an array with dimensions 4x100x100,
and we can see the rules with the following:

    $ h5dump -A test1_rules.h5 
    HDF5 "test1_rules.h5" {
    GROUP "/" {
       ATTRIBUTE "dims" {
          DATATYPE  H5T_STD_I32LE
          DATASPACE  SIMPLE { ( 3 ) / ( 3 ) }
          DATA {
          (0): 4, 100, 100
          }
       }
       ATTRIBUTE "order" {
          DATATYPE  H5T_STD_I64LE
          DATASPACE  SIMPLE { ( 3 ) / ( 3 ) }
          DATA {
          (0): 0, 1, 2
          }
       }
       GROUP "dsets" {
       }
       GROUP "rules" {
          DATASET "d1" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 1, 3 ) / ( 1, 3 ) }
          }
          DATASET "d2" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 51, 5 ) / ( 51, 5 ) }
          }
       }
    }
    }
    
    $ h5dump -d rules/d2 test1_rules.h5                                             
    HDF5 "test1_rules.h5" { 
    DATASET "rules/d2" {
       DATATYPE  H5T_IEEE_F64LE
       DATASPACE  SIMPLE { ( 51, 5 ) / ( 51, 5 ) }
       DATA {                                                                                      
       (0,0): 0, 0, 50, 99, 1,
       (1,0): 0, 0, 0, 0, 5,
       (2,0): 0, 0, 1, 1, 4.91837,
       [...]
       (49,0): 0, 0, 48, 48, 1.08163,
       (50,0): 0, 0, 49, 49, 1       
       }                                           
    }                                              
    }      

We can see that this array can be expressed only with d1 and d2 rules, without
explicit sub-array datasets, so the rules-based file uses only 8 KB, while the
standard file takes 322 KB.

![img](Test_Files/test1.png "Test 1")


<a id="org12f5c07"></a>

### Test 2 (3D)

In the second test, the bottom 2/3 of the domain is set to 0 for all depths,
while the top 1/3 of the domain is set to a sin wave. In pseudo-code:

    a[0:,0:2/3*d2,:] = 0
    a[0:,2/3*d2:,:]  = sin

By looking at the \`test.py\` script or the \`test2\_rules.h5\` file created by it,
we can see that this file can be created only with d2 rules, and since the
created domain is very large (300x1200x400), the space saved is huge (20 KB as
compared to ~1 GB).

![img](Test_Files/test2.png "Test 2")


<a id="org0f5d88a"></a>

### Test 3 (3D)

In the third test, all the domain is set to 0, except for a **cylinder** that goes
from the front to the back of the domain, with values for the **cylinder** ranging
from 0 to 1 (at the centre of cylinder axis). 

This file cannot be created only with d1 or d2 rules, since we have variation
along the third dimension (X axis in the image) in some part of the domain. So,
for this test we can provide rules for the values outside of the region where
the **cylinder** is located, but for the sub-array where the cylinder is, we need
to provide a dense sub-array. In the HDF5 file we give this explicit dense
sub-array as the dataset "cylinder", giving also the location where the
sub-array **fits** within the large array (by giving the beginning and ending
positions in the second dimension as the "d2" attribute).

    GROUP "dsets" {
       DATASET "cylinder" {
          DATATYPE  H5T_IEEE_F64LE
          DATASPACE  SIMPLE { ( 100, 51, 100 ) / ( 100, 51, 100 ) }
          ATTRIBUTE "d1" {
    	 DATATYPE  H5T_STD_I64LE
    	 DATASPACE  SIMPLE { ( 2 ) / ( 2 ) }
    	 DATA {
    	 (0): 0, 99
    	 }
          }
          ATTRIBUTE "d2" {
    	 DATATYPE  H5T_STD_I64LE
    	 DATASPACE  SIMPLE { ( 2 ) / ( 2 ) }
    	 DATA {
    	 (0): 225, 275
    	 }
          }
       }
    }

We can provide as many dense sub-arrays as needed to provide values for all the
values which could not be specified with d1 or d2 rules.

In this case, since we have to provide a dense sub-array covering 1/10 of the
whole domain, the saving in space is not as large as in Test 1 or Test2 (4 MB as
compared to 40 MB).

![img](Test_Files/test3.png "Test 3")


<a id="orgcf2b1c0"></a>

### Test 4 (3D)

This test is the same as Test 3, but in this case the **cylinder**'s orientation
is changed, so that there is no variation in the third dimension (X axis in the
image), and therefore it can be represented using only d2 rules, and therefore
it is much more efficient memory-wise (200 KB for the rules-based file, as
opposed to 4 MB in Test 3).

This test illustrates the **order** option, which can be used to store the file
(both in memory and in disk) more efficiently, but then the dimensions can be
swapped at run-time, so that this files for Test 4 could be identical to having
the file in Test 3 but using considerably less space. (NOTE: the Python script
'test.py' does not do this run-time swapping, so that the file is saved as
defined in the test4\_rules.h5 file, with the cylinder oriented along the X
Axis). 

![img](Test_Files/test4.png "Test 4")

We can see that this file only has "rules" and not "dsets" (i.e. dense
sub-arrays), but its "order" attribute is [2,1,0], meaning that dimensions 0 and
2 (X and Z in the image) are going to be swapped at run-time, so that this file
will, in practice, represent the same file as Test 3.

    $ h5dump -A test4_rules.h5 
    HDF5 "test4_rules.h5" {
    GROUP "/" {
       ATTRIBUTE "dims" {
          DATATYPE  H5T_STD_I32LE
          DATASPACE  SIMPLE { ( 3 ) / ( 3 ) }
          DATA {
          (0): 100, 500, 100
          }
       }
       ATTRIBUTE "order" {
          DATATYPE  H5T_STD_I64LE
          DATASPACE  SIMPLE { ( 3 ) / ( 3 ) }
          DATA {
          (0): 2, 1, 0
          }
       }
       GROUP "dsets" {
       }
       GROUP "rules" {
          DATASET "d1" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 0 ) / ( 0 ) }
          }
          DATASET "d2" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 5102, 5 ) / ( 5102, 5 ) }
          }
       }
    }
    }


<a id="org63426ad"></a>

### Test 5 (5D)

This test is similar to Test #1, but extended to 5D. All the "planes" (except
for the first one) in the array first dimension are set to 0. In the first
"plane", the values in the second dimension are set to 1 in the top half. 

For the points in the first "plane", and for values in the second dimension in
the bottom half, the values vary along the third dimension from 5 to 1 (in the
first half of the third dimension) and from 1 to 5 (in the second half of the
third dimension). It is probably easier to see it in pseudo-code:

    a[1:,:,:,:,:] = 0
    a[0,d2//2:,:,:,:] =  1
    a[0,0:d2//2,0:d3//3,:,:] = linspace(5,1)
    a[0,0:d2//2,d3//3:,:,:] = linspace(1,5)

The \`test.py\` array creates for this test an array with dimensions 4x20x10x15x25,
and we can see the rules with the following:

    h5dump -A test5_rules.h5 
    HDF5 "test5_rules.h5" {
    GROUP "/" {
       ATTRIBUTE "dims" {
          DATATYPE  H5T_STD_I32LE
          DATASPACE  SIMPLE { ( 5 ) / ( 5 ) }
          DATA {
          (0): 4, 20, 10, 15, 25
          }
       }
       ATTRIBUTE "ndims" {
          DATATYPE  H5T_STD_I64LE
          DATASPACE  SCALAR
          DATA {
          (0): 5
          }
       }
       ATTRIBUTE "order" {
          DATATYPE  H5T_STD_I64LE
          DATASPACE  SIMPLE { ( 5 ) / ( 5 ) }
          DATA {
          (0): 0, 1, 2, 3, 4
          }
       }
       GROUP "dsets" {
       }
       GROUP "rules" {
          DATASET "d1" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 1, 3 ) / ( 1, 3 ) }
          }
          DATASET "d2" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 1, 5 ) / ( 1, 5 ) }
          }
          DATASET "d3" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 10, 7 ) / ( 10, 7 ) }
          }
          DATASET "d4" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 0 ) / ( 0 ) }
          }
       }
    }
    }
    
    h5dump -d rules/d3 test5_rules.h5  
    HDF5 "test5_rules.h5" {
    DATASET "rules/d3" {
       DATATYPE  H5T_IEEE_F64LE
       DATASPACE  SIMPLE { ( 10, 7 ) / ( 10, 7 ) }
       DATA {
       (0,0): 0, 0, 0, 9, 0, 0, 5,
       (1,0): 0, 0, 0, 9, 1, 1, 4,
       (2,0): 0, 0, 0, 9, 2, 2, 3,
       [...]
       (8,0): 0, 0, 0, 9, 8, 8, 4,
       (9,0): 0, 0, 0, 9, 9, 9, 5
       }
    }
    }


<a id="orgc5747f9"></a>

### Test 6 (5D)

This test is the same as Test #5, but much larger in size, and in the last
points in dimension #3 where we previously had the values degradation, we now
have some random data, specified with the "random\_data" 5D dense dataset.

Thus, in pseudo-code, this would be:

    a[1:,:,:,:,:] = 0
    a[0,d2//2:,:,:,:] =  1
    a[0,0:d2//2,0:d3//3,:,:] = linspace(5,1)
    a[0,0:d2//2,d3//3:d3-1,:,:] = linspace(1,5)
    a[0,0:d2//2,d3,:,:] = random_data

We can now see that the \`test.py\` array creates for this test an array with
dimensions 4x100x36x150x150, and we can see that a 1x50x1x150x150 5D dense
dataset \`random\_data\` was created, with the attributes \`d1\`, \`d2\`,
etc. specifying where this sub-array fits in the global array:

    h5dump -A test6_rules.h5
    HDF5 "test6_rules.h5" {
    GROUP "/" {
       ATTRIBUTE "dims" {
          DATATYPE  H5T_STD_I32LE
          DATASPACE  SIMPLE { ( 5 ) / ( 5 ) }
          DATA {
          (0): 4, 100, 36, 150, 150
          }
       }
    
    [...]
    
       GROUP "dsets" {                                                                      
          DATASET "random_data" {
    	 DATATYPE  H5T_IEEE_F64LE
    	 DATASPACE  SIMPLE { ( 1, 50, 1, 150, 150 ) / ( 1, 50, 1, 150, 150 ) }
    	 ATTRIBUTE "d1" {
    	    DATATYPE  H5T_STD_I64LE                                                     
    	    DATASPACE  SIMPLE { ( 2 ) / ( 2 ) }
    	    DATA {
    	    (0): 0, 0
    	    }
    	 }
    
    [...]


<a id="org8f5d905"></a>

## In-memory RBSLA (C code)

In directory \`C\` we have the C code to read these rule-based arrays and create
an efficient in-memory representation of these RBSLA arrays.


<a id="orgd31ff92"></a>

### Verification of correct implementation.

To make sure that the C code can correctly read the rules-based files and store them in
the RBSLA in-memory structure, run the following (it assumes that you have run
\`test.py\` in the \`Test\_Files\` directory, so that the files \`testX\_rules.h5\`
exist):

    $ make clean
    $ make PP=-DVERF
    $ ./rbsla 

In this mode, \`rbsla\` will (for each test) read the rules-based file and after
storing the data in the RBSLA in-memory structure, it will create also in
\`Test\_Files\` the standard (expanded) files, as \`testX\_std\_C.h5\`. We can verify
that the C created files are bit-to-bit identical to the ones generated with
Python by doing:

    for i in `seq 1 6` ; do echo "Comparing $i" ; h5diff ../Test_Files/test${i}_std.h5 ../Test_Files/test${i}_std_C.h5 ; done
    Comparing 1
    Comparing 2
    Comparing 3
    Comparing 4
    dataset: </data> and </data>
    302334 differences found
    Comparing 5
    Comparing 6

We can see that the output files for Test 4 are not identical, since as pointed
above, the Python script ignores the 'order' attribute and writes the standard
file AS-IS in the rules description, but by changing the order in Test 4 to
[2,1,0], we are, in essence, dealing with the same file as in Test 3. The C code
does take into account the 'order' attribute, and as such, the output from Test
3 and Test 4 should be identical:

    $ h5diff ../Test_Files/test3_std_C.h5 ../Test_Files/test4_std_C.h5 
    $ 

[If we want to ignore the 'order' attribute in the C code, we can compile it
with make PP=VERFPython, in which case the output from the C code and the Python
script are bit-to-bit identical in all cases].  THIS SEGFFAULTs now, CHECK!

In order to create the standard array in the file, we allocate a complete dense
array in memory, so if we profile the memory used by this code, we will not see
the memory savings as compared to a standard dense array implementation.


<a id="org38fda0f"></a>

### Verification of reduced memory size of RBSLA in-memory structure

An easy way to compare the memory saved when using the RBSLA in-memory structure
is using the \`time\` GNU command (not the Bash built-in one), usually installed
in \`/usr/bin/time\`:

    $ /usr/bin/time --version
    time (GNU Time) UNKNOWN
    Copyright (C) 2019 Free Software Foundation, Inc.

If we compile the code with \`make PP=-DVERF\` as above, we call the
\`write\_regular\_file\` function, which allocates space to hold the complete dense
arrays before writing them to a file. We can see that the maximum resident size (in
KB) taken by this code is 2556780 (or ~2.5 GB),  which is roughly the size
required for the large array in Test 6:

    $ /usr/bin/time -f "%M" ./rbsla
    [...]
    2556780

If we compile the code with only \`make\`, the code also (for each test) reads the
rules-based file and stores the data in the RBSLA in-memory structure, but does
not call \`write\_regular\_file\`, and thus the space for the complete dense array
is not allocated. We can see that in this case the maximum resident size is only
~34MB, saving ~2.5 GB (basically the full size needed for the large Test 6, as
in this case, the rules-based representation can be done almost completely with
rules, except for a small dense sub-array).

    $ /usr/bin/time -f "%M" ./rbsla
    [...]
    34392


<a id="org97945c2"></a>

### Performance of RBSLA

To find out the performance penalty incurred when using the RBSLA in-memory
structure, compile and run the code as follows:

    $ make clean
    $ make PP=-DPERF
    $ ./rbsla 

In this mode, \`rbsla\` will read the rules-based files, store the data in the
RBSLA in-memory structure and then create the equivalent in-memory dense array
representation. Then, for each test, it will randomly select 100 million
positions in the array and compute the sum of all those data points, timing
it. As we can see below, for all the tests the "Dense" or "RBSLA" sum should be
identical, and the penalty for storing these arrays as a RBSLA depends on the
individual array and how they are represented with the "dX" rules. For
these tests, the performance for RBSLA (as compared to a dense array
representation) is: worse by ~3.3X in the Test #1; better by ~3.3X in Test #6;
and about the same for the other tests.

    $ ./rbsla 
    ## Starting Test #1.
       Dimensions: 4 100 100
       Order: 0 1 2
    Dense array sum: 50015937.775508 (0.150176 seconds)
    RBSLA array sum: 50015937.775508 (0.444583 seconds)
    
    ## Starting Test #2.
       Dimensions: 300 1200 400
       Order: 0 1 2
    Dense array sum: 429.716147 (1.564981 seconds)
    RBSLA array sum: 429.716147 (1.622543 seconds)
    
    ## Starting Test #3.
       Dimensions: 100 500 100
       Order: 0 1 2
    Reading dataset: cylinder
    Dense array sum: 1309124.994801 (0.933819 seconds)
    RBSLA array sum: 1309124.994801 (1.135102 seconds)
    
    ## Starting Test #4. 
       Dimensions: 100 500 100
       Order: 2 1 0
    Dense array sum: 1307520.575575 (0.948381 seconds)
    RBSLA array sum: 1307520.575575 (1.035728 seconds)
    
    ## Starting Test #5.
       Dimensions: 4 20 10 15 25
       Order: 0 1 2 3 4
    Dense array sum: 50016408.000000 (0.662880 seconds)
    RBSLA array sum: 50016408.000000 (0.591477 seconds)
    
    ## Starting Test #6.
       Dimensions: 4 100 36 150 150
       Order: 0 1 2 3 4
    Reading dataset: random_data
    Dense array sum: 48967706.129324 (2.027363 seconds)
    RBSLA array sum: 48967706.129324 (0.612702 seconds)

