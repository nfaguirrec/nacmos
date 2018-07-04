# NAcMoS

NAcMoS (nucleic acid modeling system) is a software package which can classify RNAs naturally, representing them as weighted graphs by using two approaches: self-organising maps (SOMs) [[Kohonen, T. (2001)](https://www.springer.com/us/book/9783540679219)] and classification into equivalence classes from pretopological spaces [[C. Largeron, and S. Bonnevay, *Information Sci.*, **144**, 169 (2002)](https://www.sciencedirect.com/science/article/pii/S0020025502001895)] induced by a particular metric.

Quantum mechanics approaches have been widely during recent years for characterising molecular systems’ properties. However,
this methodology is almost impossible to use directly with large systems where several atoms interact with many degrees of freedom (i.e. bio-polymers, macromolecules) due to difficulties in solving the corresponding equations. New models are thus being developed.

Our alternative RNA molecule representations are based on weighted di-graphs where fundamental blocks (i.e. vertices and arcs) are nucleotides and phosphodiester and hydrogen bonds. The weighting factor arises from properties describing interactions between nucleotides; such interactions can be obtained from quantum mechanics’ calculations on fragments, including nucleotides residing in neighbouring positions and influencing a particular nucleotide [[Galindo, J. et al., *J. theor. Biol.*, **240**, 574 (2006)](https://linkinghub.elsevier.com/retrieve/pii/S0022519305004698)]. The software developed to date allows a choice of different criteria for weighting vertices and arcs; end-users can thus select their own properties or descriptors for characterising each molecular motif into which the RNA molecule is divided.

Comparing different kinds of molecules and classifying them by similarity groups is an active research topic which is particularly useful because such classifications may help predict biomolecule behaviour and some of their properties. We have therefore implemented two different methods for clustering RNA molecules within this representation: one of them is a self-organising map and the other defines pre-topological for finding equivalence classes. As these methods are included in NacMoS software, they are user-friendly.

# Authors
* Néstor F. Aguirre ( nfaguirrec@unal.edu.co, nfaguirrec@gmail.com )
* Johan F. Galindo ( jfgalindoc@unal.edu.co )
* Edgar E. Daza C. ( eedazac@unal.edu.co )
