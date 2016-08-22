if OS_NAME == :Linux
    const libpath = string(pwd(), "/libdensest_subgraph.so")
else
    const libpath = string(pwd(), "/libdensest_subgraph.dylib")
end

# n is the number of nodes
# m is the number of edges
# ei and ej start indexing from 0
# w is the weights vector
# A is the vector that contains the nodes that are in the densest subgraph. Initialize it to be of size n
# sz can be initialized as follows sz = ones(Csize_t,1);

function densest_subgraph_raw(n::Int64,m::Int64,ei::Array{Int64},ej::Array{Int64},w::Array{Float64})
    @assert length(ei) >= m
    @assert length(ej) >= m
    @assert length(w) >= m
    @assert minimum(ei) >= 0
    @assert maximum(ei) < n
    @assert minimum(ej) >= 0
    @assert maximum(ej) < n
    @assert minimum(w) >= 0.
    
    set = Array(Int64,n)
    sz = Ref{Csize_t}(length(set))
    
    r = ccall( (:densest_subgraph, libpath), # function + library
                           Float64, # return type
                           (Int64,Int64,Ptr{Int64},Ptr{Int64},Ptr{Float64},Ptr{Int64},Ref{Csize_t}),
                           n,m,ei,ej,w,set,sz)

    @show r
    @show sz
    @show set[1:sz.x] 
    
    s = Set{Int64}(set[1:sz.x]+1)
                           
    return r, s
end

function densest_subgraph(A::SparseMatrixCSC{Float64,Int64})
    @assert issym(A)
    
    n = size(A,1)
    
    ei,ej,w = findnz(A)
    m = length(ei)
    ei = ei - 1
    ej = ej - 1
    
    return densest_subgraph_raw(n, m, ei, ej, w)
end

using DGFun
A = readSMAT("Erdos02-cc.smat")
densest_subgraph(A)
