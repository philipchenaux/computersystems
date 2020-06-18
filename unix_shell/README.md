In this project, you will write a cache simulator incsim.cthat takes avalgrindmemory trace as input,simulates the hit/miss behavior of a cache memory (with different E, B, and S parameters) on this trace, andoutputs the total number of hits, misses, and evictions. Forthis project, use the allocate-on-write policy tohandle write misses in your implementation of the simulator.We have provided you with the binary executable of areference cache simulator, calledcsim-ref, thatsimulates the behavior of a cache with arbitrary size and associativity on avalgrindtrace file. It uses theLRU (least-recently used) replacement policy when choosing which cache line to evict.

The command-line arguments are based on the notation (s,E, andb) from page 597 of the CS:APP2etextbook.
For example:
linux> ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace
hits:4 misses:5 evictions:3