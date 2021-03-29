mfinder1.21 contains the source code for mfinder

mtf contains my own implementation for motif sampler, it currently supports sampling 3 or 4 motifs on undirected graph.

network folder contains 5 types of networks with 2 to 3 networks for each type. the ones that are prefixed with motif_ are sampling results from motif finder and processed_ are for mfinder. My own finder should be able to run on any version of the graph.

motif_find.sh runs my motif finder through all networks and output them with output_ prefix added to the network name in the same folder.

all other scripts were for converting graphs to mfinder friendly format and are no longer in use.
