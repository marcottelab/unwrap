require 'rubygems'
require 'mkmf-rice'
#require 'mkmf'


have_library("stdc++")
have_library("pqxx")
have_library("boost_filesystem")
have_library("boost_regex")
# have_library("phenomatrix")
if RUBY_VERSION =~ /1.9/ then
  $CPPFLAGS += " -DRUBY_19"
end

$CPPFLAGS += " -DRICE"
$CXXFLAGS += " -O0"

$objs = [
#  "distance_matrix.o",
  "phenomatrix_pair.o",
  "hypergeometric.o"]
#  "fusion_phenomatrix.o"]

create_makefile('distance_matrix')