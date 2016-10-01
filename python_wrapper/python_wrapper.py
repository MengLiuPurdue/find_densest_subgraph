import ctypes

_python_wrapper = ctypes.CDLL('./libdensest_subgraph.so')
densest_subgraph = _python_wrapper.densest_subgraph
densest_subgraph.argtypes = (ctypes.c_uint64,ctypes.c_uint64,ctypes.POINTER(ctypes.c_uint64),ctypes.POINTER(ctypes.c_uint64),ctypes.POINTER(ctypes.c_double),ctypes.POINTER(ctypes.c_uint64),ctypes.POINTER(ctypes.c_size_t))
densest_subgraph.restype = ctypes.c_double

def densest_subgraph(input_file, output_file):
    global _python_wrapper
    f = open(input_file)
    first_line = f.readline().strip()
    import re
    first_line = re.split(" ",first_line)
    n = long(first_line[0])
    m = long(first_line[2])
    ei_type = ctypes.c_uint64 * m
    ej_type = ctypes.c_uint64 * m
    w_type = ctypes.c_double * m
    outputlen = (ctypes.c_size_t * 1)()
    output = (ctypes.c_uint64 * n)()
    ei = []
    ej = []
    w = []

    data = f.read()
    data = data.split()
    for i in range(m):
        ei += [long(data[3 * i])]
        ej += [long(data[3 * i + 1])]
        w += [float(data[3 * i + 2])]

    f.close()
    result = _python_wrapper.densest_subgraph(ctypes.c_uint64(n),ctypes.c_uint64(m),ei_type(*ei),ej_type(*ej),w_type(*w),output,outputlen)
    out = file(output_file, "w")
    out.write('%f\n' % result);
    print result
    out.write('%d\n' % outputlen[0]);
    print outputlen[0]
    for j in range(outputlen[0]):
	out.write('%d\n' % output[j])
	print(output[j])
    out.close()
    return result

import python_wrapper
import sys
python_wrapper.densest_subgraph(sys.argv[1], sys.argv[2])
