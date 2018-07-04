#!/bin/gawk
BEGIN{
HEAD="\
nodeTypes [\n\
	Adenine [\n\
		a  3.000000\n\
		b  3.000000\n\
	]\n\
	Citosine [\n\
		a  4.000000\n\
		b  4.000000\n\
	]\n\
	Default [\n\
		a  1.000000\n\
		b  1.000000\n\
	]\n\
	Guanine [\n\
		a  2.000000\n\
		b  2.000000\n\
	]\n\
	Uracil [\n\
		a  5.000000\n\
		b  5.000000\n\
	]\n\
]\n\
edgeTypes [\n\
	Default [\n\
		a  1.000000\n\
		b  1.000000\n\
	]\n\
	Hydrogen [\n\
		a  2.000000\n\
		b  2.000000\n\
	]\n\
	Hydrogen2 [\n\
		a  2.000000\n\
		b  3.000000\n\
	]\n\
	Phosphate [\n\
		a  10.000000\n\
		b  10.000000\n\
	]\n\
]\n\
"

NAME_TO_TYPE["A"] = "Adenine" ;
NAME_TO_TYPE["C"] = "Citosine" ;
NAME_TO_TYPE["G"] = "Guanine" ;
NAME_TO_TYPE["U"] = "Uracil" ;

}

($0~/Name: /) {
	NAME=substr($3,1,length($3)-4);
}

{
	if($0~/graph /) {
		print $0 ;
		print "label \"" NAME "\"";
		print "directed 1" ;
		print HEAD
	}
	
	else if($0~/directed /) {
	}
	
	else if($0~/node /) {
		id = $4 ;
		label = substr($6, 2, 1) ;
		
		print "node [ " ;
		print "\t id " id ;
		
		if( id==1 )
			print "\t isFirst 1";
			
		print "\t name \"" label "\"" ;
		print "\t type \"" NAME_TO_TYPE[label] "\"" ;
		print "\t neighborRange 12.00000" ;
	}
	
	else if($0~/graphics /) {
		x = $4 ;
		y = $6 ;
		
		print "\t graphics [ " ;
		print "\t\t x " 1.7*x  ;
		print "\t\t y " 1.7*y  ;
		print "\t\t w 22" ;
		print "\t\t h 23" ;
		print "\t ] " ;
	}
	
	else if($0~/edge /) {
		source = $4 ;
		target = $6 ;
		
		print "edge [" ;
		print "\t source " source ;
		print "\t target " target ;
		print "\t name \" \""  ;
		
		if( $6 == $4+1 )
			print "\t type \"Phosphate\"" ;
		else
			print "\t type \"Hydrogen\"" ;
			
		print "\t graphics [ " ;
		print "\t\t w 2"   ;
		print "\t ] " ;
		
		print "]" ;
	}
	
	else{
		print $0
	}
}
