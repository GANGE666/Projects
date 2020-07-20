#!/usr/bin/perl -Tw

use strict;
use CGI;

my($cgi) = new CGI;

print $cgi->header;
my($name) = "admin";
$name = $cgi->param('name') if defined $cgi->param('name');

print $cgi->h1("Welcome $name !");
print $cgi->end_html;
