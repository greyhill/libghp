#!/usr/bin/perl -w
use strict;

open(I, "<cl_device_extensions") or die("couldn't open cl_device_extensions");
open(O, ">cl_device_extensions_methods") 
  or die("couldn't open cl_device_extensions_methods");

while(my $line = <I>) {
  if($line =~ /DEVICE_INFO_RETVAL\(\s+([^\s]+),\s+([^\s]+)\s+\);/) {
    my $param = $1;
    my $type = $2;
    my $name = $param;
    $name =~ s/CL_DEVICE_//;
    $name = lc $name;
    print O
"$type $name() const { return device_get_info_<$type >(id_, $param); }\n"
  }
}

close(I);
close(O);

