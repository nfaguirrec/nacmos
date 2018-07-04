#!/bin/bash

INPUT_FILE=$1

HEAD="
nodeTypes [\n
\t        Adenine [\n
\t\t                a  3.000000\n
\t\t                b  3.000000\n
\t        ]\n
\t        Citosine [\n
\t\t                a  4.000000\n
\t\t                b  4.000000\n
\t        ]\n
\t        Default [\n
\t\t                a  1.000000\n
\t\t                b  1.000000\n
\t        ]\n
\t        Guanine [\n
\t\t                a  2.000000\n
\t\t                b  2.000000\n
\t        ]\n
\t          Uracil [\n
\t\t                a  5.000000\n
\t\t                b  5.000000\n
\t          ]\n
]\n
edgeTypes [\n
\t          Default [\n
\t\t                a  1.000000\n
\t\t                b  1.000000\n
\t          ]\n
\t          Hydrogen [\n
\t\t                a  2.000000\n
\t\t                b  2.000000\n
\t          ]\n
\t          Hydrogen2 [\n
\t\t                a  2.000000\n
\t\t                b  3.000000\n
\t          ]\n
\t          Phosphate [\n
\t\t                a  10.000000\n
\t\t                b  10.000000\n
\t          ]\n
]\n
"

gawk '
	{if($0~/graph /) print $0 '$HEAD' ;
	}
' $INPUT_FILE